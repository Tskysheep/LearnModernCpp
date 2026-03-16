/**
 * ============================================================
 * 特性名称: noexcept 说明符与运算符
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: noexcept说明符声明函数不会抛出异常；noexcept运算符
 *          在编译期检查表达式是否声明为不抛异常。
 *
 * [为什么引入]: C++03的动态异常规范(throw(...))被证明设计失败，
 *   运行时检查开销大且无法优化。noexcept提供编译期的异常保证，
 *   让编译器生成更高效的代码，尤其对移动操作和容器至关重要。
 *
 * [关键要点]:
 *   - noexcept函数如果抛出异常，程序直接terminate()
 *   - 移动构造/赋值标记noexcept后，容器可以安全使用移动而非拷贝
 *   - noexcept(表达式) 是条件noexcept，根据表达式结果决定
 *   - noexcept运算符返回bool，可用于模板元编程
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <utility>
#include <stdexcept>

// ============================================================
// 基础用法
// ============================================================

// 标记为不抛异常的函数
int safe_add(int a, int b) noexcept {
    return a + b;
}

// 可能抛异常的函数
int risky_divide(int a, int b) {
    if (b == 0) throw std::runtime_error("除以零!");
    return a / b;
}

// noexcept(true) 等价于 noexcept
void definitely_safe() noexcept(true) {
    // 保证不抛异常
}

// noexcept(false) 等价于不写noexcept
void maybe_throws() noexcept(false) {
    // 可能抛异常
}

void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. 使用noexcept函数
    std::cout << "safe_add(3, 5) = " << safe_add(3, 5) << std::endl;

    try {
        std::cout << "risky_divide(10, 2) = " << risky_divide(10, 2) << std::endl;
        std::cout << "risky_divide(10, 0) = ";
        std::cout << risky_divide(10, 0) << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "异常: " << e.what() << std::endl;
    }

    // 2. noexcept运算符 —— 编译期检查
    std::cout << "\nnoexcept运算符检查:" << std::endl;
    std::cout << "  safe_add是noexcept: "
              << (noexcept(safe_add(1, 2)) ? "是" : "否") << std::endl;
    std::cout << "  risky_divide是noexcept: "
              << (noexcept(risky_divide(1, 2)) ? "是" : "否") << std::endl;
    std::cout << "  definitely_safe是noexcept: "
              << (noexcept(definitely_safe()) ? "是" : "否") << std::endl;

    // 3. 标准库中的noexcept
    std::cout << "\n标准库类型的noexcept属性:" << std::endl;
    std::cout << "  int默认构造: "
              << (noexcept(int()) ? "noexcept" : "可能抛") << std::endl;
    std::cout << "  string默认构造: "
              << (noexcept(std::string()) ? "noexcept" : "可能抛") << std::endl;
    std::cout << "  vector<int>默认构造: "
              << (noexcept(std::vector<int>()) ? "noexcept" : "可能抛") << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 移动操作的noexcept对容器性能的影响

// 没有noexcept的移动构造
class BadMover {
public:
    BadMover() : data_("BadMover") {}
    BadMover(const BadMover& other) : data_(other.data_) {
        std::cout << "  BadMover 拷贝!" << std::endl;
    }
    // 移动构造没有noexcept!
    BadMover(BadMover&& other) : data_(std::move(other.data_)) {
        std::cout << "  BadMover 移动!" << std::endl;
    }
    std::string data_;
};

// 有noexcept的移动构造
class GoodMover {
public:
    GoodMover() : data_("GoodMover") {}
    GoodMover(const GoodMover& other) : data_(other.data_) {
        std::cout << "  GoodMover 拷贝!" << std::endl;
    }
    // 移动构造标记为noexcept
    GoodMover(GoodMover&& other) noexcept : data_(std::move(other.data_)) {
        std::cout << "  GoodMover 移动!" << std::endl;
    }
    std::string data_;
};

// 2. 条件noexcept
template <typename T>
class Wrapper {
public:
    Wrapper() = default;

    // 条件noexcept: 如果T的移动构造是noexcept，则这个函数也是
    Wrapper(Wrapper&& other) noexcept(std::is_nothrow_move_constructible<T>::value)
        : data_(std::move(other.data_)) {}

    // 条件noexcept的swap
    void swap(Wrapper& other) noexcept(noexcept(std::swap(data_, other.data_))) {
        std::swap(data_, other.data_);
    }

    T data_;
};

// 3. noexcept在模板中的使用
template <typename T>
void checked_swap(T& a, T& b)
    noexcept(noexcept(T(std::move(a))) && noexcept(a = std::move(b))) {
    T temp(std::move(a));
    a = std::move(b);
    b = std::move(temp);
}

void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 容器重新分配时的行为对比
    std::cout << "--- BadMover (无noexcept) ---" << std::endl;
    std::cout << "vector重新分配时:" << std::endl;
    {
        std::vector<BadMover> vec;
        vec.reserve(2);
        vec.emplace_back();
        vec.emplace_back();
        std::cout << "  触发重新分配:" << std::endl;
        vec.emplace_back(); // 重新分配时可能使用拷贝而非移动!
    }

    std::cout << "\n--- GoodMover (有noexcept) ---" << std::endl;
    std::cout << "vector重新分配时:" << std::endl;
    {
        std::vector<GoodMover> vec;
        vec.reserve(2);
        vec.emplace_back();
        vec.emplace_back();
        std::cout << "  触发重新分配:" << std::endl;
        vec.emplace_back(); // 重新分配时使用移动!
    }

    // 2. 条件noexcept
    std::cout << "\n--- 条件noexcept ---" << std::endl;
    Wrapper<int> w1, w2;
    w1.data_ = 42;
    w2.data_ = 99;
    std::cout << "Wrapper<int>::swap是noexcept: "
              << (noexcept(w1.swap(w2)) ? "是" : "否") << std::endl;

    Wrapper<std::string> ws1, ws2;
    ws1.data_ = "甲";
    ws2.data_ = "乙";
    std::cout << "Wrapper<string>::swap是noexcept: "
              << (noexcept(ws1.swap(ws2)) ? "是" : "否") << std::endl;

    // 3. type_traits中的noexcept检查
    std::cout << "\n--- type_traits ---" << std::endl;
    std::cout << "int是nothrow_move_constructible: "
              << std::is_nothrow_move_constructible<int>::value << std::endl;
    std::cout << "GoodMover是nothrow_move_constructible: "
              << std::is_nothrow_move_constructible<GoodMover>::value << std::endl;
    std::cout << "BadMover是nothrow_move_constructible: "
              << std::is_nothrow_move_constructible<BadMover>::value << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: noexcept函数抛出异常 = 程序终止
    std::cout << "陷阱1: noexcept函数抛出异常会调用std::terminate()" << std::endl;
    std::cout << "  编译器不会检查noexcept函数体是否真的不抛异常" << std::endl;
    std::cout << "  这是程序员的承诺，违反后果严重" << std::endl;

    // 演示（不实际执行，否则程序终止）
    // auto bad = []() noexcept {
    //     throw std::runtime_error("boom"); // 调用terminate()!
    // };
    // bad(); // 程序终止!

    // 陷阱2: 析构函数默认是noexcept的
    std::cout << "\n陷阱2: 析构函数默认是noexcept" << std::endl;
    struct Test {
        ~Test() {} // 隐式noexcept
    };
    std::cout << "  Test析构函数是noexcept: "
              << noexcept(Test().~Test()) << std::endl;
    std::cout << "  如果析构函数抛出异常，程序terminate()" << std::endl;

    // 陷阱3: noexcept不是函数签名的一部分（C++11/14中）
    // 但在C++17中它成为了函数类型的一部分
    std::cout << "\n陷阱3: C++11中noexcept不是函数签名的一部分" << std::endl;
    std::cout << "  (C++17中已改变)" << std::endl;

    // 陷阱4: 不要滥用noexcept
    std::cout << "\n陷阱4: 何时该用noexcept?" << std::endl;
    std::cout << "  应该用: 移动构造/赋值、swap、析构函数、简单的getter" << std::endl;
    std::cout << "  谨慎用: 可能分配内存的函数、调用可能抛异常的函数" << std::endl;
    std::cout << "  不要用: 不确定是否会抛异常的函数" << std::endl;

    // 陷阱5: noexcept和move_if_noexcept
    std::cout << "\n陷阱5: std::move_if_noexcept" << std::endl;
    std::cout << "  如果移动构造是noexcept，返回右值引用" << std::endl;
    std::cout << "  否则返回const左值引用（退化为拷贝）" << std::endl;
    GoodMover gm;
    BadMover bm;
    auto&& ref1 = std::move_if_noexcept(gm); // 右值引用（移动）
    auto&& ref2 = std::move_if_noexcept(bm); // const左值引用（拷贝）
    (void)ref1; (void)ref2;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: noexcept" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
