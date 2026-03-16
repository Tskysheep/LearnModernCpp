/**
 * ============================================================
 * 特性名称: 智能指针 (unique_ptr、shared_ptr、weak_ptr)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: C++11引入的三种智能指针，通过RAII机制自动管理动态内存的生命周期，
 *           分别提供独占所有权、共享所有权和弱引用语义。
 *
 * [为什么引入]: 手动new/delete容易导致内存泄漏、悬空指针和双重释放等问题。
 *               智能指针使资源管理自动化，大幅提升代码安全性和可维护性。
 *
 * [关键要点]:
 *   - unique_ptr: 独占所有权，不可复制但可移动，零开销抽象
 *   - shared_ptr: 共享所有权，内部引用计数，引用归零时自动释放
 *   - weak_ptr: 不增加引用计数，用于观测shared_ptr管理的对象，可打破循环引用
 *   - make_shared/make_unique(C++14) 优于直接new，因为更安全且可能更高效
 *   - 支持自定义删除器，可管理任意资源（文件、网络连接等）
 * ============================================================
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <functional>

// ============================================================
// 辅助类
// ============================================================
class Resource {
public:
    std::string name_;
    Resource(const std::string& name) : name_(name) {
        std::cout << "  [构造] Resource: " << name_ << std::endl;
    }
    ~Resource() {
        std::cout << "  [析构] Resource: " << name_ << std::endl;
    }
    void use() const {
        std::cout << "  [使用] Resource: " << name_ << std::endl;
    }
};

// 用于演示循环引用的类
class Node {
public:
    std::string name_;
    // 如果用 shared_ptr 会导致循环引用
    std::weak_ptr<Node> partner_;  // 用 weak_ptr 打破循环

    Node(const std::string& name) : name_(name) {
        std::cout << "  [构造] Node: " << name_ << std::endl;
    }
    ~Node() {
        std::cout << "  [析构] Node: " << name_ << std::endl;
    }
};

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. unique_ptr: 独占所有权
    std::cout << "[1] unique_ptr 独占所有权:\n";
    {
        std::unique_ptr<Resource> p1(new Resource("独占资源A"));
        p1->use();

        // 使用 make_unique 更安全（注意：make_unique 是C++14，这里用new演示）
        // C++11中推荐用 std::unique_ptr<T>(new T(...))
        std::unique_ptr<Resource> p2(new Resource("独占资源B"));

        // 不可复制
        // std::unique_ptr<Resource> p3 = p1;  // 编译错误！

        // 可以移动
        std::unique_ptr<Resource> p3 = std::move(p1);
        std::cout << "  移动后 p1 是否为空: " << (p1 == nullptr ? "是" : "否") << std::endl;
        p3->use();
    } // 离开作用域，自动释放
    std::cout << "  (离开作用域，资源已自动释放)\n\n";

    // 2. shared_ptr: 共享所有权
    std::cout << "[2] shared_ptr 共享所有权和引用计数:\n";
    {
        std::shared_ptr<Resource> sp1 = std::make_shared<Resource>("共享资源C");
        std::cout << "  引用计数: " << sp1.use_count() << std::endl;

        {
            std::shared_ptr<Resource> sp2 = sp1;  // 共享所有权
            std::cout << "  复制后引用计数: " << sp1.use_count() << std::endl;
            sp2->use();
        } // sp2 析构，引用计数减1

        std::cout << "  sp2析构后引用计数: " << sp1.use_count() << std::endl;
    } // sp1析构，引用计数归零，释放资源
    std::cout << "  (最后一个shared_ptr析构，资源释放)\n\n";

    // 3. weak_ptr: 弱引用
    std::cout << "[3] weak_ptr 弱引用:\n";
    {
        std::weak_ptr<Resource> wp;
        {
            std::shared_ptr<Resource> sp = std::make_shared<Resource>("弱引用观测D");
            wp = sp;
            std::cout << "  wp.expired(): " << (wp.expired() ? "是" : "否") << std::endl;

            // lock() 获取 shared_ptr
            if (auto locked = wp.lock()) {
                locked->use();
                std::cout << "  lock后引用计数: " << locked.use_count() << std::endl;
            }
        } // sp析构，对象被释放
        std::cout << "  sp析构后 wp.expired(): " << (wp.expired() ? "是" : "否") << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. 自定义删除器
    std::cout << "[1] 自定义删除器:\n";
    {
        // unique_ptr 使用函数对象作为删除器
        auto fileDeleter = [](FILE* fp) {
            if (fp) {
                std::cout << "  [自定义删除器] 关闭文件指针" << std::endl;
                fclose(fp);
            }
        };
        // 注意：自定义删除器的类型是 unique_ptr 模板参数的一部分
        std::unique_ptr<FILE, decltype(fileDeleter)> filePtr(
            fopen("test_smart_ptr_tmp.txt", "w"), fileDeleter);
        if (filePtr) {
            fputs("Hello Smart Pointer!", filePtr.get());
            std::cout << "  文件写入成功" << std::endl;
        }
    } // 离开作用域，自动调用自定义删除器关闭文件
    // 清理临时文件
    std::remove("test_smart_ptr_tmp.txt");

    // shared_ptr 自定义删除器更灵活（不需要在类型中指定）
    {
        std::shared_ptr<Resource> sp(new Resource("自定义删除"),
            [](Resource* r) {
                std::cout << "  [自定义删除器] 即将删除: " << r->name_ << std::endl;
                delete r;
            });
        sp->use();
    }
    std::cout << std::endl;

    // 2. weak_ptr 打破循环引用
    std::cout << "[2] weak_ptr 打破循环引用:\n";
    {
        auto nodeA = std::make_shared<Node>("节点A");
        auto nodeB = std::make_shared<Node>("节点B");

        // 使用 weak_ptr 互相引用，不会循环引用
        nodeA->partner_ = nodeB;
        nodeB->partner_ = nodeA;

        std::cout << "  nodeA 引用计数: " << nodeA.use_count() << std::endl;
        std::cout << "  nodeB 引用计数: " << nodeB.use_count() << std::endl;

        // 通过 weak_ptr 访问 partner
        if (auto partner = nodeA->partner_.lock()) {
            std::cout << "  节点A的伙伴: " << partner->name_ << std::endl;
        }
    } // 两个Node都能正常析构
    std::cout << std::endl;

    // 3. unique_ptr 管理动态数组
    std::cout << "[3] unique_ptr 管理动态数组:\n";
    {
        std::unique_ptr<int[]> arr(new int[5]);
        for (int i = 0; i < 5; ++i) {
            arr[i] = i * 10;
        }
        std::cout << "  数组内容: ";
        for (int i = 0; i < 5; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 4. make_shared 的优势
    std::cout << "[4] make_shared 的优势:\n";
    {
        // 推荐写法：一次内存分配（对象 + 控制块）
        auto sp1 = std::make_shared<Resource>("make_shared创建");
        std::cout << "  make_shared: 更安全、更高效（单次内存分配）" << std::endl;

        // 不推荐写法：两次内存分配（对象 + 控制块分开）
        // std::shared_ptr<Resource> sp2(new Resource("new创建"));
    }
    std::cout << std::endl;

    // 5. 在容器中存储智能指针
    std::cout << "[5] 容器中存储智能指针:\n";
    {
        std::vector<std::unique_ptr<Resource>> vec;
        vec.push_back(std::unique_ptr<Resource>(new Resource("容器元素1")));
        vec.push_back(std::unique_ptr<Resource>(new Resource("容器元素2")));

        for (const auto& p : vec) {
            p->use();
        }
        std::cout << "  容器销毁时自动释放所有资源\n";
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. 不要用同一个裸指针创建多个 shared_ptr
    std::cout << "[1] 错误：同一裸指针创建多个shared_ptr（导致双重释放）:\n";
    std::cout << "  // Resource* raw = new Resource(\"危险\");\n";
    std::cout << "  // std::shared_ptr<Resource> sp1(raw);\n";
    std::cout << "  // std::shared_ptr<Resource> sp2(raw);  // 危险！双重释放！\n";
    std::cout << "  正确做法：用 make_shared 或从已有 shared_ptr 复制\n\n";

    // 2. 不要对栈对象使用智能指针
    std::cout << "[2] 错误：对栈对象使用智能指针:\n";
    std::cout << "  // Resource r(\"栈对象\");\n";
    std::cout << "  // std::shared_ptr<Resource> sp(&r);  // 危险！会delete栈对象！\n\n";

    // 3. unique_ptr 不可复制
    std::cout << "[3] unique_ptr 不可复制，只能移动:\n";
    {
        std::unique_ptr<int> p1(new int(42));
        // std::unique_ptr<int> p2 = p1;  // 编译错误！
        std::unique_ptr<int> p2 = std::move(p1);  // 正确：移动
        std::cout << "  移动后 p1 == nullptr: " << (p1 == nullptr ? "是" : "否") << std::endl;
        std::cout << "  p2 的值: " << *p2 << std::endl;
    }
    std::cout << std::endl;

    // 4. shared_ptr 循环引用导致内存泄漏
    std::cout << "[4] shared_ptr 循环引用（已用 weak_ptr 解决，见高级用法）:\n";
    std::cout << "  // 如果两个对象用 shared_ptr 互相引用，\n";
    std::cout << "  // 引用计数永远不会归零，导致内存泄漏。\n";
    std::cout << "  // 解决方案：其中一方改用 weak_ptr。\n\n";

    // 5. get() 返回的裸指针不要 delete
    std::cout << "[5] 不要delete智能指针get()返回的裸指针:\n";
    std::cout << "  // auto sp = std::make_shared<int>(42);\n";
    std::cout << "  // int* raw = sp.get();\n";
    std::cout << "  // delete raw;  // 危险！智能指针析构时会再次释放！\n\n";

    // 6. weak_ptr 使用前必须 lock
    std::cout << "[6] weak_ptr 使用前必须检查/lock:\n";
    {
        std::weak_ptr<int> wp;
        {
            auto sp = std::make_shared<int>(100);
            wp = sp;
        }
        // sp 已销毁
        if (wp.expired()) {
            std::cout << "  对象已销毁，weak_ptr 已失效\n";
        }
        auto locked = wp.lock();
        if (!locked) {
            std::cout << "  lock() 返回空 shared_ptr，必须检查！\n";
        }
    }
    std::cout << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 智能指针 (unique_ptr/shared_ptr/weak_ptr) - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
