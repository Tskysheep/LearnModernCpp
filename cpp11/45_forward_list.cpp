/**
 * ============================================================
 * 特性名称: std::forward_list (单向链表)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: std::forward_list 是单向链表容器，每个节点只有一个指向下一个节点
 *           的指针，相比 std::list（双向链表）更节省内存。
 *
 * [为什么引入]: std::list 是双向链表，每个节点有两个指针（前驱和后继），
 *               当只需要单向遍历时，forward_list 减少一半的指针开销，
 *               提供与C语言手写单向链表相当的性能。
 *
 * [关键要点]:
 *   - 单向链表，只支持前向迭代（forward_iterator）
 *   - 没有 size() 方法（为了O(1)的 splice_after）
 *   - 插入/删除操作使用 _after 后缀版本（insert_after, erase_after等）
 *   - before_begin() 返回头节点之前的迭代器
 *   - 内存占用比 list 小（每节点少一个指针）
 * ============================================================
 */

#include <iostream>
#include <forward_list>
#include <list>
#include <algorithm>
#include <string>
#include <iterator>

// ============================================================
// 辅助函数：打印 forward_list
// ============================================================
template<typename T>
void print_flist(const std::string& label, const std::forward_list<T>& fl) {
    std::cout << "  " << label << ": ";
    for (const auto& v : fl) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. 创建和初始化
    std::cout << "[1] 创建和初始化:\n";
    {
        std::forward_list<int> fl1;                          // 空链表
        std::forward_list<int> fl2 = {1, 2, 3, 4, 5};       // 初始化列表
        std::forward_list<int> fl3(5, 10);                   // 5个10
        std::forward_list<int> fl4(fl2);                     // 拷贝构造
        std::forward_list<int> fl5(fl2.begin(), fl2.end());  // 范围构造

        print_flist("fl2 (初始化列表)", fl2);
        print_flist("fl3 (5个10)", fl3);
        print_flist("fl4 (拷贝)", fl4);
    }
    std::cout << std::endl;

    // 2. 前端操作
    std::cout << "[2] 前端操作 (push_front / pop_front):\n";
    {
        std::forward_list<int> fl;
        fl.push_front(3);
        fl.push_front(2);
        fl.push_front(1);
        print_flist("push_front 1,2,3", fl);

        std::cout << "  front(): " << fl.front() << std::endl;

        fl.pop_front();
        print_flist("pop_front后", fl);
    }
    std::cout << std::endl;

    // 3. insert_after 在指定位置后插入
    std::cout << "[3] insert_after (在指定位置之后插入):\n";
    {
        std::forward_list<int> fl = {1, 3, 5};
        print_flist("原始", fl);

        // 在头部之前插入（使用 before_begin）
        fl.insert_after(fl.before_begin(), 0);
        print_flist("before_begin后插入0", fl);

        // 在第一个元素后插入
        auto it = fl.begin();
        fl.insert_after(it, 99);
        print_flist("第一个元素后插入99", fl);

        // 在指定位置后插入多个元素
        it = fl.begin();
        std::advance(it, 2);
        fl.insert_after(it, {10, 20});
        print_flist("第3个元素后插入{10,20}", fl);
    }
    std::cout << std::endl;

    // 4. erase_after 删除
    std::cout << "[4] erase_after (删除指定位置之后的元素):\n";
    {
        std::forward_list<int> fl = {1, 2, 3, 4, 5};
        print_flist("原始", fl);

        // 删除第一个元素后的元素（即删除2）
        fl.erase_after(fl.begin());
        print_flist("删除begin()之后", fl);

        // 删除范围
        auto it1 = fl.begin();
        auto it2 = it1;
        std::advance(it2, 3);
        fl.erase_after(it1, it2);
        print_flist("删除范围后", fl);
    }
    std::cout << std::endl;

    // 5. 判空和清空
    std::cout << "[5] 判空和清空:\n";
    {
        std::forward_list<int> fl = {1, 2, 3};
        std::cout << "  empty(): " << (fl.empty() ? "是" : "否") << std::endl;

        // 注意：forward_list 没有 size() 方法！
        // 获取大小需要使用 std::distance
        auto sz = std::distance(fl.begin(), fl.end());
        std::cout << "  元素个数(distance): " << sz << std::endl;

        fl.clear();
        std::cout << "  clear后 empty(): " << (fl.empty() ? "是" : "否") << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. splice_after 拼接链表
    std::cout << "[1] splice_after (拼接链表):\n";
    {
        std::forward_list<int> fl1 = {1, 2, 3};
        std::forward_list<int> fl2 = {10, 20, 30};

        print_flist("fl1", fl1);
        print_flist("fl2", fl2);

        // 将 fl2 的所有元素拼接到 fl1 的第一个元素之后
        fl1.splice_after(fl1.begin(), fl2);
        print_flist("splice后 fl1", fl1);
        print_flist("splice后 fl2", fl2);  // fl2 变空
    }
    std::cout << std::endl;

    // 2. 链表特有操作：sort、merge、unique、reverse
    std::cout << "[2] 链表特有操作:\n";
    {
        // sort（链表不能用 std::sort，必须用成员函数）
        std::forward_list<int> fl = {5, 3, 1, 4, 2};
        fl.sort();
        print_flist("sort后", fl);

        // reverse
        fl.reverse();
        print_flist("reverse后", fl);

        // unique（去除连续重复元素，需先排序）
        std::forward_list<int> fl2 = {1, 1, 2, 3, 3, 3, 4};
        fl2.unique();
        print_flist("unique后", fl2);

        // merge（合并两个已排序链表）
        std::forward_list<int> flA = {1, 3, 5, 7};
        std::forward_list<int> flB = {2, 4, 6, 8};
        flA.merge(flB);
        print_flist("merge后 flA", flA);
    }
    std::cout << std::endl;

    // 3. remove 和 remove_if
    std::cout << "[3] remove / remove_if:\n";
    {
        std::forward_list<int> fl = {1, 2, 3, 4, 5, 2, 3, 2};

        fl.remove(2);
        print_flist("remove(2)后", fl);

        fl = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        fl.remove_if([](int n) { return n % 2 == 0; });
        print_flist("remove_if(偶数)后", fl);
    }
    std::cout << std::endl;

    // 4. 与 std::list 对比
    std::cout << "[4] 与 std::list 对比:\n";
    {
        std::cout << "  特性              forward_list    list\n";
        std::cout << "  ─────────────────────────────────────\n";
        std::cout << "  迭代方向          单向(前向)      双向\n";
        std::cout << "  每节点指针数      1个             2个\n";
        std::cout << "  size()方法        无              有\n";
        std::cout << "  push_back()       无              有\n";
        std::cout << "  插入操作          insert_after    insert\n";
        std::cout << "  反向迭代器        无              有\n";
        std::cout << "  内存开销          更小            更大\n";
    }
    std::cout << std::endl;

    // 5. emplace_front 和 emplace_after
    std::cout << "[5] emplace_front 和 emplace_after (原地构造):\n";
    {
        struct Person {
            std::string name;
            int age;
            Person(const std::string& n, int a) : name(n), age(a) {}
        };

        std::forward_list<Person> fl;
        fl.emplace_front("张三", 25);
        fl.emplace_front("李四", 30);

        // 在第一个元素后原地构造
        fl.emplace_after(fl.begin(), "王五", 28);

        std::cout << "  人员列表:\n";
        for (const auto& p : fl) {
            std::cout << "    " << p.name << ", " << p.age << "岁" << std::endl;
        }
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. 没有 size() 方法
    std::cout << "[1] forward_list 没有 size() 方法:\n";
    std::cout << "  // fl.size();  // 编译错误！\n";
    std::cout << "  // 获取大小: std::distance(fl.begin(), fl.end())  // O(n)\n";
    std::cout << "  // 设计原因: 不存储size可以保证splice_after为O(1)\n\n";

    // 2. 没有 push_back 和 back
    std::cout << "[2] 没有 push_back() 和 back():\n";
    std::cout << "  // fl.push_back(1);  // 编译错误！\n";
    std::cout << "  // fl.back();        // 编译错误！\n";
    std::cout << "  // 单向链表无法高效访问尾部\n\n";

    // 3. 插入/删除用 _after 版本
    std::cout << "[3] 插入和删除必须用 _after 后缀版本:\n";
    {
        std::forward_list<int> fl = {1, 2, 3};
        std::cout << "  // fl.insert(fl.begin(), 0);  // 编译错误！\n";
        std::cout << "  // 正确: fl.insert_after(fl.before_begin(), 0);\n";

        // 在头部插入需要 before_begin()
        fl.insert_after(fl.before_begin(), 0);
        print_flist("头部插入0", fl);
    }
    std::cout << std::endl;

    // 4. before_begin() 不可解引用
    std::cout << "[4] before_begin() 不可解引用:\n";
    std::cout << "  auto it = fl.before_begin();\n";
    std::cout << "  // *it;  // 未定义行为！只能用于 insert_after/erase_after\n\n";

    // 5. 不能用 std::sort
    std::cout << "[5] 不能用 std::sort (需要随机访问迭代器):\n";
    std::cout << "  // std::sort(fl.begin(), fl.end());  // 编译错误！\n";
    std::cout << "  // 必须用成员函数: fl.sort();\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::forward_list (单向链表) - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
