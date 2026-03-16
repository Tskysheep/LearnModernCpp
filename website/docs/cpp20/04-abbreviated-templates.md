---
sidebar_position: 5
slug: 04-abbreviated-templates
title: "简写函数模板 (Abbreviated Function Templates)"
description: "C++20 简写函数模板 (Abbreviated Function Templates) 教学示例"
tags: [cpp20, abbreviated_templates]
---

# 简写函数模板 (Abbreviated Function Templates)

C++20 标准引入的 简写函数模板 (Abbreviated Function Templates) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/04_abbreviated_templates.cpp"
/**
 * ============================================================
 * 特性名称: 简写函数模板 (Abbreviated Function Templates)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: C++20 允许在普通函数参数中使用 auto，
 *          使函数自动成为模板函数（简写函数模板）。
 *
 * [为什么引入]: 简化模板函数的编写，不需要显式写 template<typename T>，
 *             使代码更简洁、更接近泛型编程的直觉。
 *
 * [关键要点]:
 *   - void f(auto x) 等价于 template<typename T> void f(T x)
 *   - 可以使用 concept 约束 auto（constrained auto）
 *   - 每个 auto 参数引入独立的模板参数
 *   - 与 Lambda 中的 auto 参数类似，但用于普通函数
 * ============================================================
 */

#include <concepts>
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <numeric>

// ===================== basic_usage() =====================

// C++20 简写函数模板：auto 参数使函数变成模板
void print_value(auto value) {
    std::cout << "值: " << value << " (类型大小: " << sizeof(value) << " 字节)\n";
}

// 等价的传统模板写法:
// template<typename T>
// void print_value(T value) {
//     std::cout << "值: " << value << " (类型大小: " << sizeof(value) << " 字节)\n";
// }

// 多个 auto 参数 —— 每个 auto 是独立的模板参数
auto add(auto a, auto b) {
    return a + b;
}
// 等价于:
// template<typename T1, typename T2>
// auto add(T1 a, T2 b) { return a + b; }

// 返回类型也可以用 auto（与 C++14 一致）
auto multiply(auto x, auto y) -> decltype(x * y) {
    return x * y;
}

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 简写模板自动推导类型
    std::cout << "-- auto 参数自动成为模板 --\n";
    print_value(42);
    print_value(3.14);
    print_value(std::string("Hello C++20"));
    print_value('A');

    // 不同类型参数的混合调用
    std::cout << "\n-- 多个 auto 参数（独立模板参数） --\n";
    std::cout << "add(1, 2) = " << add(1, 2) << "\n";
    std::cout << "add(1, 2.5) = " << add(1, 2.5) << "\n";  // int + double → double
    std::cout << "add(string, string) = "
              << add(std::string("Hello "), std::string("World")) << "\n";

    std::cout << "\n-- 返回类型推导 --\n";
    std::cout << "multiply(3, 4) = " << multiply(3, 4) << "\n";
    std::cout << "multiply(2.5, 4) = " << multiply(2.5, 4) << "\n";
}

// ===================== advanced_usage() =====================

// 使用 concept 约束的 auto（constrained auto）
void print_number(std::integral auto n) {
    std::cout << "整数: " << n << "\n";
}

void print_number(std::floating_point auto n) {
    std::cout << "浮点数: " << n << "\n";
}

// constrained auto 用于返回类型
std::integral auto compute_factorial(std::integral auto n) {
    decltype(n) result = 1;
    for (decltype(n) i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// 混合：部分参数使用 auto，部分使用具体类型
void mixed_function(int count, auto value) {
    for (int i = 0; i < count; ++i) {
        std::cout << value << " ";
    }
    std::cout << "\n";
}

// 与 const auto& 和 auto&& 结合
void inspect(const auto& value) {
    std::cout << "const 引用, 类型大小: " << sizeof(value) << "\n";
}

void forward_and_print(auto&& value) {
    // auto&& 是转发引用（forwarding reference）
    if constexpr (std::is_lvalue_reference_v<decltype(value)>) {
        std::cout << "接收到左值引用\n";
    } else {
        std::cout << "接收到右值引用\n";
    }
}

// 简写模板与显式模板语法对比
// 简写版本：
auto max_of(auto a, auto b) {
    return (a > b) ? a : b;
}

// 当需要两个参数类型相同时，简写版本做不到——需要显式模板
template<typename T>
T max_same_type(T a, T b) {
    return (a > b) ? a : b;
}

// 使用 concept 约束来实现"相同类型"效果
void print_pair(auto a, std::same_as<decltype(a)> auto b) {
    std::cout << "配对: (" << a << ", " << b << ")\n";
}

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // constrained auto 重载选择
    std::cout << "-- Constrained auto（约束 auto） --\n";
    print_number(42);       // 调用整数版本
    print_number(3.14);     // 调用浮点版本
    print_number(100L);     // 调用整数版本（long 也是 integral）

    std::cout << "\n-- constrained auto 返回类型 --\n";
    std::cout << "5! = " << compute_factorial(5) << "\n";
    std::cout << "10! = " << compute_factorial(10) << "\n";

    // 混合参数
    std::cout << "\n-- 混合具体类型和 auto --\n";
    mixed_function(3, "hello");
    mixed_function(2, 42);

    // const auto& 和 auto&&
    std::cout << "\n-- const auto& 和 auto&& --\n";
    int x = 10;
    inspect(x);
    inspect(std::string("test"));

    forward_and_print(x);          // 左值
    forward_and_print(42);         // 右值
    forward_and_print(std::string("temp"));  // 右值

    // 约束相同类型
    std::cout << "\n-- 确保参数类型相同 --\n";
    print_pair(1, 2);
    print_pair(std::string("a"), std::string("b"));
    // print_pair(1, 2.0);  // 编译错误！类型不同
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：每个 auto 是独立的模板参数
    std::cout << "1. 每个 auto 引入独立的模板参数:\n";
    std::cout << "   void f(auto a, auto b) 中 a 和 b 可以是不同类型\n";
    std::cout << "   如果需要相同类型，使用显式 template<typename T> void f(T a, T b)\n";
    std::cout << "   示例: add(1, 2.5) 合法，因为 auto a=int, auto b=double\n\n";

    // 陷阱2：简写模板不能指定模板参数
    std::cout << "2. 简写模板无法显式指定模板参数:\n";
    std::cout << "   void f(auto x) 不能写成 f<int>(42)\n";
    std::cout << "   如需显式指定，必须用传统 template<typename T> 语法\n\n";

    // 陷阱3：auto 参数的类型由调用者决定
    std::cout << "3. 每个不同类型的调用都会实例化不同版本:\n";
    auto show_type = [](auto x) {
        std::cout << "   值: " << x << "\n";
    };
    show_type(42);
    show_type(3.14);

    // 陷阱4：在类成员函数中使用 auto 参数
    std::cout << "\n4. 类的非静态成员函数中，auto 参数使其成为成员函数模板\n";
    std::cout << "   注意：虚函数不能是模板，所以虚函数不能使用 auto 参数\n";

    // 演示：简写模板与 Lambda 的区别
    std::cout << "\n5. 简写函数模板 vs Lambda:\n";
    std::cout << "   Lambda: auto f = [](auto x) {};  // C++14 就支持\n";
    std::cout << "   函数:   void f(auto x) {}        // C++20 新增\n";
    std::cout << "   两者都会生成模板，但函数可以重载，Lambda 不行\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== 简写函数模板 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 简写函数模板 (Abbreviated Function Templates)](https://en.cppreference.com/w/cpp/language/function_template#Abbreviated_function_template)
