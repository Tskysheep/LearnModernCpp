/**
 * ============================================================
 * 特性名称: 可变参数模板 (Variadic Templates)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: 允许模板接受任意数量和类型的参数，通过参数包(parameter pack)
 *          和包展开(pack expansion)实现。
 *
 * [为什么引入]: C++03中要支持多个参数的模板，需要为每种参数数量写
 *   一个特化版本（如tuple需要N个版本）。可变参数模板让一个模板
 *   就能处理任意数量的参数，是实现tuple、make_shared等的基础。
 *
 * [关键要点]:
 *   - typename... Args 声明模板参数包
 *   - Args... args 声明函数参数包
 *   - sizeof...(Args) 获取参数包中的参数数量
 *   - 递归展开是最常用的包展开方式
 *   - 参数包可以在多种上下文中展开
 * ============================================================
 */

#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>
#include <memory>
#include <type_traits>

// ============================================================
// 基础用法
// ============================================================

// 1. 最简单的可变参数函数模板 —— 递归展开
// 递归终止条件（基础情况）
void print() {
    std::cout << std::endl; // 无参数时结束递归
}

// 递归展开
template <typename T, typename... Args>
void print(const T& first, const Args&... rest) {
    std::cout << first;
    if (sizeof...(rest) > 0) {
        std::cout << ", ";
    }
    print(rest...); // 递归调用，参数包每次少一个
}

// 2. sizeof... 获取参数数量
template <typename... Args>
void count_args(const Args&... args) {
    std::cout << "  参数数量: " << sizeof...(Args)
              << " (类型包), " << sizeof...(args)
              << " (值包)" << std::endl;
}

void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. print可以接受任意数量、任意类型的参数
    std::cout << "print(): ";
    print();

    std::cout << "print(1): ";
    print(1);

    std::cout << "print(1, 2.5, \"你好\"): ";
    print(1, 2.5, "你好");

    std::cout << "print(true, 'A', 3.14, \"世界\", 42): ";
    print(true, 'A', 3.14, "世界", 42);

    // 2. sizeof...
    std::cout << "\nsizeof...:" << std::endl;
    count_args();
    count_args(1);
    count_args(1, 2.0, "三", '4');
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 类型安全的printf（替代C的printf）
void safe_printf(const char* format) {
    // 基础情况：只剩格式字符串
    while (*format) {
        if (*format == '%') {
            throw std::runtime_error("safe_printf: 格式参数多于实际参数!");
        }
        std::cout << *format++;
    }
}

template <typename T, typename... Args>
void safe_printf(const char* format, const T& value, const Args&... args) {
    while (*format) {
        if (*format == '%') {
            std::cout << value;
            safe_printf(format + 1, args...); // 递归处理剩余参数
            return;
        }
        std::cout << *format++;
    }
    throw std::runtime_error("safe_printf: 实际参数多于格式参数!");
}

// 2. 求和（任意数量的数值参数）
template <typename T>
T sum(T value) {
    return value;
}

template <typename T, typename... Args>
T sum(T first, Args... rest) {
    return first + sum(rest...);
}

// 3. 简化版make_unique（C++14才正式加入标准库）
template <typename T, typename... Args>
std::unique_ptr<T> my_make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 4. 简化版tuple（展示可变参数类模板）
// 空tuple（基础情况）
template <typename... Types>
struct MyTuple {};

// 递归定义
template <typename Head, typename... Tail>
struct MyTuple<Head, Tail...> : MyTuple<Tail...> {
    Head value;

    MyTuple() = default;
    MyTuple(const Head& h, const Tail&... t)
        : MyTuple<Tail...>(t...), value(h) {}
};

// 5. 参数包展开的其他上下文
template <typename... Args>
void expand_demo(const Args&... args) {
    // 展开为逗号分隔的表达式（每个参数加1后存入vector）
    // 使用初始化列表技巧进行展开
    std::vector<int> v = { (std::cout << args << " ", 0)... };
    std::cout << std::endl;
    (void)v;
}

void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 类型安全的printf
    std::cout << "safe_printf:" << std::endl;
    std::cout << "  ";
    safe_printf("姓名: %, 年龄: %, 成绩: %\n", "张三", 25, 95.5);

    // 2. 任意参数求和
    std::cout << "\n求和:" << std::endl;
    std::cout << "  sum(1,2,3,4,5) = " << sum(1, 2, 3, 4, 5) << std::endl;
    std::cout << "  sum(1.1, 2.2, 3.3) = " << sum(1.1, 2.2, 3.3) << std::endl;

    // 3. my_make_unique
    struct Person {
        std::string name;
        int age;
        Person(const std::string& n, int a) : name(n), age(a) {}
    };
    auto p = my_make_unique<Person>("李四", 30);
    std::cout << "\nmy_make_unique: " << p->name << ", " << p->age << std::endl;

    // 4. MyTuple
    MyTuple<int, double, std::string> t(42, 3.14, "hello");
    std::cout << "\nMyTuple第一个元素: " << t.value << std::endl;

    // 5. 包展开演示
    std::cout << "\n包展开: ";
    expand_demo(1, "你好", 3.14, true);
}

// ============================================================
// 易错点与陷阱
// ============================================================

// 编译时递归的深度限制
template <int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static const int value = 1;
};

// 展示参数包的正确展开位置
template <typename... Args>
void expand_patterns(const Args&... args) {
    // 正确的展开模式:
    // args...           展开为: arg1, arg2, arg3
    // func(args)...     展开为: func(arg1), func(arg2), func(arg3)
    // func(args...)     展开为: func(arg1, arg2, arg3)  注意区别!

    // 以下是展开为vector的技巧
    auto to_string = [](const auto& v) -> std::string {
        std::ostringstream oss;
        oss << v;
        return oss.str();
    };

    // func(args)... 对每个参数分别调用func
    std::vector<std::string> strs = { to_string(args)... };
    for (const auto& s : strs) {
        std::cout << "  [" << s << "]" << std::endl;
    }
}

void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: 忘记递归终止条件
    std::cout << "陷阱1: 递归展开必须有终止条件(基础情况)" << std::endl;
    std::cout << "  否则编译器会无限递归实例化模板直到达到深度限制" << std::endl;

    // 陷阱2: ...的位置很重要
    std::cout << "\n陷阱2: '...'的位置决定展开方式" << std::endl;
    std::cout << "  f(args...)  = f(a1, a2, a3)       // 参数包展开为多个参数" << std::endl;
    std::cout << "  f(args)...  = f(a1), f(a2), f(a3) // 对每个参数分别调用f" << std::endl;

    // 演示
    std::cout << "\n展开模式演示:" << std::endl;
    expand_patterns(1, "hello", 3.14);

    // 陷阱3: sizeof...返回的是参数数量，不是字节大小
    std::cout << "\n陷阱3: sizeof...返回参数数量，不是字节数" << std::endl;
    auto check = [](auto... args) {
        std::cout << "  sizeof...(args) = " << sizeof...(args) << " (个数)" << std::endl;
    };
    check(1, 2.0, "三");

    // 陷阱4: 编译时递归深度限制
    std::cout << "\n陷阱4: 模板递归有深度限制(通常256-1024)" << std::endl;
    std::cout << "  Factorial<10> = " << Factorial<10>::value << std::endl;
    // Factorial<10000> 可能导致编译错误

    // 陷阱5: 可变参数模板的调试困难
    std::cout << "\n陷阱5: 可变参数模板的错误信息可能很长" << std::endl;
    std::cout << "  建议: 用static_assert在早期检查类型约束" << std::endl;

    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 总是提供递归终止的基础情况" << std::endl;
    std::cout << "  2. 用sizeof...检查参数数量" << std::endl;
    std::cout << "  3. 配合std::forward实现完美转发" << std::endl;
    std::cout << "  4. 考虑使用初始化列表技巧替代递归展开" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: 可变参数模板" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
