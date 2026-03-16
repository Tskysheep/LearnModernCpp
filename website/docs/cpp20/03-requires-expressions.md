---
sidebar_position: 4
title: "requires 表达式"
description: "C++20 requires 表达式 教学示例"
tags: [cpp20, requires_expressions]
---

# requires 表达式

C++20 标准引入的 requires 表达式 特性完整教学示例。

## 完整源码

```cpp title="cpp20/03_requires_expressions.cpp"
/**
 * ============================================================
 * 特性名称: requires 表达式
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: requires 表达式是一种编译期布尔表达式，
 *          用于检测一组表达式是否合法（即类型是否满足某些要求）。
 *
 * [为什么引入]: 提供一种直观、结构化的方式来表达类型约束，
 *             替代 C++17 中复杂的 SFINAE 检测技巧（如 void_t、detection idiom）。
 *
 * [关键要点]:
 *   - 简单需求（simple requirement）：检测表达式是否合法
 *   - 类型需求（type requirement）：检测类型是否存在
 *   - 复合需求（compound requirement）：检测表达式的返回类型
 *   - 嵌套需求（nested requirement）：在 requires 中嵌套额外的约束
 * ============================================================
 */

#include <concepts>
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <iterator>
#include <ranges>

// ===================== basic_usage() =====================

// 1. 简单需求 (Simple Requirement)
// 检测表达式是否合法，不关心返回类型
template<typename T>
concept Swappable = requires(T a, T b) {
    std::swap(a, b);  // 只检查这个表达式是否能编译
};

// 2. 类型需求 (Type Requirement)
// 检测某个类型名是否存在
template<typename T>
concept HasValueType = requires {
    typename T::value_type;        // 要求 T 有 value_type 成员类型
    typename T::iterator;          // 要求 T 有 iterator 成员类型
    typename T::size_type;         // 要求 T 有 size_type 成员类型
};

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 简单需求演示
    std::cout << "-- 简单需求 (Simple Requirement) --\n";
    std::cout << "int 可交换: " << std::boolalpha << Swappable<int> << "\n";
    std::cout << "string 可交换: " << Swappable<std::string> << "\n";

    // 类型需求演示
    std::cout << "\n-- 类型需求 (Type Requirement) --\n";
    std::cout << "vector<int> 有 value_type/iterator/size_type: "
              << HasValueType<std::vector<int>> << "\n";
    std::cout << "int 有 value_type/iterator/size_type: "
              << HasValueType<int> << "\n";
    std::cout << "string 有 value_type/iterator/size_type: "
              << HasValueType<std::string> << "\n";

    // 直接在函数中使用 requires 表达式作为布尔值
    std::cout << "\n-- requires 表达式作为布尔值 --\n";

    constexpr bool int_has_plus = requires(int a, int b) { a + b; };
    std::cout << "int 支持 +: " << int_has_plus << "\n";

    constexpr bool int_is_range = std::ranges::range<int>;
    std::cout << "int 是 range: " << int_is_range << "\n";

    constexpr bool vec_is_range = std::ranges::range<std::vector<int>>;
    std::cout << "vector<int> 是 range: " << vec_is_range << "\n";
}

// ===================== advanced_usage() =====================

// 3. 复合需求 (Compound Requirement)
// 检测表达式是否合法，并约束其返回类型
template<typename T>
concept Indexable = requires(T container, std::size_t idx) {
    // 复合需求语法: { 表达式 } -> 类型约束;
    { container[idx] } -> std::convertible_to<typename T::value_type&>;
    { container.size() } -> std::same_as<std::size_t>;
};

// 更复杂的复合需求
template<typename T>
concept Arithmetic = requires(T a, T b) {
    { a + b } -> std::same_as<T>;     // 加法返回相同类型
    { a - b } -> std::same_as<T>;     // 减法返回相同类型
    { a * b } -> std::same_as<T>;     // 乘法返回相同类型
    { a / b } -> std::same_as<T>;     // 除法返回相同类型
    { -a }    -> std::same_as<T>;     // 取负返回相同类型
};

// 4. 嵌套需求 (Nested Requirement)
// 在 requires 表达式内部添加额外的编译期约束
template<typename T>
concept SmallType = requires {
    // 嵌套需求使用 requires 关键字 + 编译期布尔表达式
    requires sizeof(T) <= 8;          // 大小不超过8字节
    requires std::is_trivially_copyable_v<T>;  // 可平凡复制
};

// 综合示例：Stack concept
template<typename T>
concept StackLike = requires(T stack, typename T::value_type val) {
    // 简单需求
    stack.push(val);
    stack.pop();

    // 复合需求
    { stack.top() } -> std::same_as<typename T::value_type&>;
    { stack.empty() } -> std::convertible_to<bool>;
    { stack.size() } -> std::convertible_to<std::size_t>;

    // 类型需求
    typename T::value_type;
    typename T::size_type;

    // 嵌套需求
    requires std::default_initializable<T>;
};

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 复合需求演示
    std::cout << "-- 复合需求 (Compound Requirement) --\n";
    std::cout << "vector<int> 可索引: " << std::boolalpha << Indexable<std::vector<int>> << "\n";
    std::cout << "string 可索引: " << Indexable<std::string> << "\n";

    std::cout << "\n-- Arithmetic concept --\n";
    std::cout << "int 满足 Arithmetic: " << Arithmetic<int> << "\n";
    std::cout << "double 满足 Arithmetic: " << Arithmetic<double> << "\n";
    std::cout << "string 满足 Arithmetic: " << Arithmetic<std::string> << "\n";

    // 嵌套需求演示
    std::cout << "\n-- 嵌套需求 (Nested Requirement) --\n";
    std::cout << "int 是 SmallType: " << SmallType<int> << "\n";
    std::cout << "double 是 SmallType: " << SmallType<double> << "\n";
    std::cout << "char 是 SmallType: " << SmallType<char> << "\n";

    // 使用综合 concept 进行约束
    auto use_stack = []<StackLike S>(S& stack) {
        std::cout << "栈是否为空: " << stack.empty() << "\n";
    };
    // 注意：std::stack 满足这个 concept（但这里不引入 <stack> 来演示）

    // 在 if constexpr 中使用 requires
    std::cout << "\n-- 在 if constexpr 中使用 requires --\n";
    auto smart_print = [](const auto& value) {
        if constexpr (requires { std::cout << value; }) {
            std::cout << "可直接输出: " << value << "\n";
        } else {
            std::cout << "不可直接输出，大小: " << sizeof(value) << " 字节\n";
        }
    };

    smart_print(42);
    smart_print(std::string("hello"));
    smart_print(std::vector<int>{1, 2, 3});  // vector 不能直接 cout
}

// ===================== pitfalls() =====================

// 陷阱演示用的类型
struct Weird {
    int operator+(const Weird&) { return 0; }  // + 返回 int，不是 Weird
};

// 区分"简单需求"和"复合需求"
template<typename T>
concept SimpleAddable = requires(T a, T b) {
    a + b;  // 简单需求：只检查表达式是否合法
};

template<typename T>
concept StrictAddable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;  // 复合需求：还检查返回类型
};

void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：简单需求不检查返回类型
    std::cout << "1. 简单需求 vs 复合需求的区别:\n";
    std::cout << "   Weird 满足 SimpleAddable: "
              << std::boolalpha << SimpleAddable<Weird> << "\n";
    std::cout << "   Weird 满足 StrictAddable: "
              << StrictAddable<Weird> << "\n";
    std::cout << "   (Weird 的 operator+ 返回 int，不是 Weird)\n\n";

    // 陷阱2：requires 表达式中的表达式不会被求值
    std::cout << "2. requires 中的表达式不会被执行（只检查编译合法性）\n";
    std::cout << "   即使写 requires(int a) { a / 0; } 也不会出错\n";
    constexpr bool div_zero_ok = requires(int a) { a / 0; };
    std::cout << "   requires(int a) { a / 0; } = " << div_zero_ok << "\n\n";

    // 陷阱3：requires 表达式 vs requires 子句
    std::cout << "3. 区分 requires 表达式和 requires 子句:\n";
    std::cout << "   requires 表达式: requires(T a) { a + a; }  → 产生 bool 值\n";
    std::cout << "   requires 子句:   requires std::integral<T>  → 约束模板\n";
    std::cout << "   两者可以嵌套: requires requires(T a) { a + a; }\n";
    std::cout << "   第一个 requires 是子句，第二个是表达式\n\n";

    // 陷阱4：简单需求中不能有分号以外的语句
    std::cout << "4. requires 表达式中只能有需求（表达式），不能有语句\n";
    std::cout << "   如 if、for、变量声明等都不允许\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== requires 表达式 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: requires 表达式](https://en.cppreference.com/w/cpp/language/requires)
