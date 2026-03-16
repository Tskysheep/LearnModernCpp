/**
 * ============================================================
 * 特性名称: =default 与 =delete
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: =default显式要求编译器生成特殊成员函数的默认实现；
 *          =delete显式禁止某个函数（包括特殊成员函数和普通重载）。
 *
 * [为什么引入]: C++03中，阻止拷贝需要将拷贝构造/赋值声明为private
 *   且不实现，这不够直观且错误信息不友好。=default和=delete提供了
 *   清晰、声明式的方式来控制特殊成员函数的生成。
 *
 * [关键要点]:
 *   - =default可用于6个特殊成员函数（默认构造、析构、拷贝构造/赋值、移动构造/赋值）
 *   - =delete可用于任何函数，不仅限于特殊成员函数
 *   - =delete的函数仍参与重载决议，选中后报编译错误
 *   - 用=default明确意图优于依赖编译器隐式生成
 * ============================================================
 */

#include <iostream>
#include <string>
#include <memory>

// ============================================================
// 基础用法
// ============================================================

// 示例1: 使用=default恢复被抑制的默认构造函数
class MyClass {
public:
    // 定义了有参构造函数后，默认构造函数不再自动生成
    MyClass(int value) : value_(value) {
        std::cout << "  MyClass(int): " << value_ << std::endl;
    }

    // 用=default显式恢复默认构造函数
    MyClass() = default;

    int getValue() const { return value_; }

private:
    int value_ = 0; // C++11类内初始化
};

// 示例2: 使用=delete禁止拷贝（不可拷贝类）
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const std::string& name) : name_(name) {}

    // 禁止拷贝
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    // 允许移动
    NonCopyable(NonCopyable&& other) : name_(std::move(other.name_)) {
        std::cout << "  移动构造: " << name_ << std::endl;
    }
    NonCopyable& operator=(NonCopyable&& other) {
        name_ = std::move(other.name_);
        std::cout << "  移动赋值: " << name_ << std::endl;
        return *this;
    }

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. =default 恢复默认构造函数
    MyClass obj1;        // 默认构造（=default）
    MyClass obj2(42);    // 有参构造
    std::cout << "obj1: " << obj1.getValue() << std::endl;
    std::cout << "obj2: " << obj2.getValue() << std::endl;

    // 2. =delete 禁止拷贝
    NonCopyable a("资源A");
    // NonCopyable b = a;            // 编译错误! 拷贝构造被删除
    // NonCopyable c; c = a;         // 编译错误! 拷贝赋值被删除
    NonCopyable d = std::move(a);    // OK，移动构造
    std::cout << "d.name = " << d.name() << std::endl;

    // 3. C++03中禁止拷贝的旧写法对比
    // class OldWay {
    // private:
    //     OldWay(const OldWay&);            // 声明但不实现
    //     OldWay& operator=(const OldWay&); // 声明但不实现
    // };
    std::cout << "\n=delete 比 C++03的private技巧更清晰、错误信息更友好" << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================

// 使用=delete禁止特定的函数重载
void processInt(int value) {
    std::cout << "  处理int: " << value << std::endl;
}
void processInt(double) = delete;   // 禁止double隐式转换为int
void processInt(bool) = delete;     // 禁止bool隐式转换为int
void processInt(char) = delete;     // 禁止char隐式转换为int

// 单例模式：利用=delete
class Singleton {
public:
    static Singleton& instance() {
        static Singleton inst;
        return inst;
    }

    void doWork() const {
        std::cout << "  Singleton正在工作" << std::endl;
    }

    // 禁止所有复制和移动
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

private:
    Singleton() { std::cout << "  Singleton创建" << std::endl; }
};

// 明确声明所有特殊成员函数（Rule of Five）
class ResourceHolder {
public:
    ResourceHolder() = default;
    ~ResourceHolder() = default;
    ResourceHolder(const ResourceHolder&) = default;
    ResourceHolder& operator=(const ResourceHolder&) = default;
    ResourceHolder(ResourceHolder&&) = default;
    ResourceHolder& operator=(ResourceHolder&&) = default;

    explicit ResourceHolder(const std::string& name) : name_(name) {}
    const std::string& name() const { return name_; }

private:
    std::string name_ = "默认资源";
};

// 禁止在堆上分配（删除operator new）
class StackOnly {
public:
    StackOnly() = default;
    void show() const { std::cout << "  StackOnly对象" << std::endl; }

    // 禁止堆上分配
    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;
};

void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 禁止特定重载
    std::cout << "禁止特定类型的重载:" << std::endl;
    processInt(42);        // OK
    // processInt(3.14);   // 编译错误! double版本被删除
    // processInt(true);   // 编译错误! bool版本被删除
    // processInt('A');    // 编译错误! char版本被删除

    // 2. 单例模式
    std::cout << "\n单例模式:" << std::endl;
    Singleton& s1 = Singleton::instance();
    Singleton& s2 = Singleton::instance();
    s1.doWork();
    std::cout << "  同一实例: " << (&s1 == &s2 ? "是" : "否") << std::endl;

    // 3. Rule of Five —— 明确声明所有特殊成员函数
    std::cout << "\nRule of Five:" << std::endl;
    ResourceHolder r1("资源1");
    ResourceHolder r2 = r1;  // 拷贝构造
    ResourceHolder r3 = std::move(r1); // 移动构造
    std::cout << "  r2: " << r2.name() << std::endl;
    std::cout << "  r3: " << r3.name() << std::endl;

    // 4. 禁止堆分配
    std::cout << "\n禁止堆分配:" << std::endl;
    StackOnly local_obj;
    local_obj.show();
    // StackOnly* heap_obj = new StackOnly; // 编译错误! operator new被删除
    std::cout << "  StackOnly只能在栈上创建" << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================

class Tricky {
public:
    Tricky(int x) : x_(x) {}
    // 注意: 一旦定义了任何构造函数，默认构造函数就不再隐式生成
    // Tricky t; // 如果不加=default，这会编译错误

    // 如果定义了析构函数，移动操作不再隐式生成（C++11规则）
    ~Tricky() { std::cout << "  Tricky析构" << std::endl; }

    int x_;
};

void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: =default的析构函数会影响移动操作的生成
    std::cout << "陷阱1: 定义析构函数会阻止移动操作的隐式生成" << std::endl;
    Tricky t1(10);
    Tricky t2 = t1;              // 拷贝构造（可能仍可用）
    // Tricky t3 = std::move(t1); // 会退化为拷贝（移动构造未生成）
    std::cout << "  定义了析构函数 -> 移动构造/赋值不再自动生成" << std::endl;

    // 陷阱2: =delete的函数仍参与重载决议
    // 这意味着如果被选中，会报"使用了删除的函数"而非"找不到匹配函数"
    std::cout << "\n陷阱2: =delete函数仍参与重载决议" << std::endl;
    std::cout << "  processInt(3.14) 不会退而求其次调用processInt(int)" << std::endl;
    std::cout << "  而是直接报错：使用了已删除的函数" << std::endl;

    // 陷阱3: 不能在类外部=default或=delete非特殊成员函数
    // void foo() = default; // 编译错误! 只有特殊成员函数能=default
    // 但=delete可以用于任何函数
    std::cout << "\n陷阱3: =default只能用于特殊成员函数" << std::endl;
    std::cout << "  =delete可用于任何函数" << std::endl;

    // 最佳实践
    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 如果需要默认行为，用=default明确表示意图" << std::endl;
    std::cout << "  2. 禁止拷贝用=delete，不要用private技巧" << std::endl;
    std::cout << "  3. 遵循Rule of Five/Zero原则" << std::endl;
    std::cout << "  4. 用=delete阻止隐式类型转换" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: =default 与 =delete" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
