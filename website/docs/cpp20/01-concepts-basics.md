---
sidebar_position: 2
title: "Concepts 基础 (concept、requires子句)"
description: "C++20 Concepts 基础 (concept、requires子句) 教学示例"
tags: [cpp20, concepts_basics]
---

# Concepts 基础 (concept、requires子句)

C++20 标准引入的 Concepts 基础 (concept、requires子句) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/01_concepts_basics.cpp"
/**
 * ============================================================
 * 特性名称: Concepts 基础 (concept、requires子句)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: Concepts 是一种对模板参数进行约束的机制，
 *          使用 requires 子句可以明确指定模板参数需要满足的条件。
 *
 * [为什么引入]: 在 C++20 之前，模板参数约束依赖 SFINAE，
 *             导致错误信息晦涩难懂、代码可读性差。
 *             Concepts 提供了简洁、直观的约束语法。
 *
 * [关键要点]:
 *   - 标准库提供了 std::integral、std::floating_point 等内置 concept
 *   - requires 子句可以直接约束模板参数
 *   - 与 SFINAE 相比，错误信息更清晰、语法更简洁
 *   - concept 约束可以出现在多个位置（模板参数、返回类型前、尾部）
 * ============================================================
 */

#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>

// ===================== basic_usage() =====================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. 使用内置 concept: std::integral
    // 约束模板参数 T 必须是整数类型
    auto add_integers = []<std::integral T>(T a, T b) -> T {
        return a + b;
    };

    std::cout << "整数加法: 3 + 5 = " << add_integers(3, 5) << "\n";
    std::cout << "长整数加法: 100L + 200L = " << add_integers(100L, 200L) << "\n";
    // add_integers(3.14, 2.71);  // 编译错误！double 不满足 std::integral

    // 2. 使用内置 concept: std::floating_point
    auto multiply_floats = []<std::floating_point T>(T a, T b) -> T {
        return a * b;
    };

    std::cout << "浮点乘法: 3.14 * 2.0 = " << multiply_floats(3.14, 2.0) << "\n";
    // multiply_floats(3, 5);  // 编译错误！int 不满足 std::floating_point

    // 3. requires 子句约束模板函数
    // 方式一：尾部 requires 子句
    auto safe_divide = []<typename T>(T a, T b) -> T
        requires std::floating_point<T>
    {
        if (b == T{0}) {
            std::cout << "  [警告] 除数为零!\n";
            return T{0};
        }
        return a / b;
    };

    std::cout << "安全除法: 10.0 / 3.0 = " << safe_divide(10.0, 3.0) << "\n";
    std::cout << "安全除法: 1.0 / 0.0 = " << safe_divide(1.0, 0.0) << "\n";
}

// ===================== advanced_usage() =====================

// 方式二：在模板参数列表中使用 concept
template<std::integral T>
T factorial(T n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// 方式三：在返回类型前使用 requires
template<typename T>
requires std::integral<T>
T gcd(T a, T b) {
    while (b != 0) {
        T temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 方式四：使用 auto 简写 (abbreviated template)
// 等价于 template<std::integral T> void print_integral(T value)
void print_integral(std::integral auto value) {
    std::cout << "整数值: " << value << "\n";
}

// ----- 与 SFINAE 对比 -----

// C++17 SFINAE 方式（繁琐且错误信息不友好）
template<typename T,
         typename = std::enable_if_t<std::is_arithmetic_v<T>>>
T old_style_abs(T value) {
    return value < 0 ? -value : value;
}

// C++20 Concepts 方式（简洁且错误信息友好）
template<typename T>
requires std::is_arithmetic_v<T>
T new_style_abs(T value) {
    return value < 0 ? -value : value;
}

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 各种约束位置的演示
    std::cout << "阶乘 5! = " << factorial(5) << "\n";
    std::cout << "最大公约数 gcd(12, 8) = " << gcd(12, 8) << "\n";
    print_integral(42);
    print_integral(static_cast<short>(7));

    // SFINAE vs Concepts 对比
    std::cout << "\nSFINAE方式 abs(-3.14) = " << old_style_abs(-3.14) << "\n";
    std::cout << "Concepts方式 abs(-42) = " << new_style_abs(-42) << "\n";

    // 多个 concept 约束
    auto convert = []<typename From, typename To>(From value)
        requires std::integral<From> && std::floating_point<To>
    {
        return static_cast<To>(value);
    };

    double result = convert.template operator()<int, double>(42);
    std::cout << "整数转浮点: " << result << "\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：concept 约束不是运行时检查，是编译期检查
    std::cout << "1. concept 是编译期检查，不是运行时检查\n";
    std::cout << "   以下代码会编译失败（不会到运行时）：\n";
    std::cout << "   // std::integral auto x = 3.14;  // 编译错误!\n\n";

    // 陷阱2：requires 子句中的表达式必须是布尔类型常量表达式
    std::cout << "2. requires 子句中的表达式必须是编译期常量表达式\n";
    std::cout << "   不能使用运行时变量作为约束条件\n\n";

    // 陷阱3：约束与重载的交互
    std::cout << "3. 当有多个重载满足时，编译器选择约束更严格（更特化）的版本\n";
    std::cout << "   这叫做 subsumption（包含关系）\n\n";

    // 演示：不同约束级别的重载选择
    auto describe = [](std::integral auto val) {
        std::cout << "   整数版本: " << val << "\n";
    };

    // 注意：如果有一个更宽泛的重载（如接受 auto），
    // 编译器会优先选择约束更严格的版本
    describe(42);
    describe(static_cast<char>('A'));
}

// ===================== main() =====================
int main() {
    std::cout << "===== Concepts 基础 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: Concepts 基础 (concept、requires子句)](https://en.cppreference.com/w/cpp/language/constraints)
