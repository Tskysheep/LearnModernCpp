---
sidebar_position: 3
title: "Concepts 高级 (自定义 concept、组合约束)"
description: "C++20 Concepts 高级 (自定义 concept、组合约束) 教学示例"
tags: [cpp20, concepts_advanced]
---

# Concepts 高级 (自定义 concept、组合约束)

C++20 标准引入的 Concepts 高级 (自定义 concept、组合约束) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/02_concepts_advanced.cpp"
/**
 * ============================================================
 * 特性名称: Concepts 高级 (自定义 concept、组合约束)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: 除了使用标准库提供的 concept，程序员可以自定义 concept，
 *          并使用逻辑运算符对 concept 进行组合。
 *
 * [为什么引入]: 标准库的内置 concept 不能覆盖所有场景，
 *             自定义 concept 可以精确表达领域特有的类型约束。
 *
 * [关键要点]:
 *   - 使用 concept 关键字定义自己的约束
 *   - concept 可以通过 && 和 || 进行组合
 *   - concept 细化（refinement）允许更特化的 concept 继承约束
 *   - subsumption 规则决定了重载解析时的优先级
 * ============================================================
 */

#include <concepts>
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

// ===================== basic_usage() =====================

// 自定义 concept：要求类型可以相加
template<typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

// 自定义 concept：要求类型支持输出到流
template<typename T>
concept Printable = requires(T a, std::ostream& os) {
    { os << a } -> std::same_as<std::ostream&>;
};

// 自定义 concept：要求类型像容器一样可以遍历
template<typename T>
concept Container = requires(T c) {
    { c.begin() } -> std::input_or_output_iterator;
    { c.end() } -> std::input_or_output_iterator;
    { c.size() } -> std::convertible_to<std::size_t>;
};

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 使用 Addable concept
    auto sum = []<Addable T>(T a, T b) -> T {
        return a + b;
    };

    std::cout << "整数相加: " << sum(10, 20) << "\n";
    std::cout << "浮点相加: " << sum(1.5, 2.5) << "\n";
    std::cout << "字符串拼接: " << sum(std::string("Hello, "), std::string("World!")) << "\n";

    // 使用 Printable concept
    auto display = []<Printable T>(const T& value) {
        std::cout << "可打印值: " << value << "\n";
    };

    display(42);
    display(3.14);
    display(std::string("C++20"));

    // 使用 Container concept
    auto print_size = []<Container C>(const C& container) {
        std::cout << "容器大小: " << container.size() << "\n";
    };

    std::vector<int> v = {1, 2, 3, 4, 5};
    std::string s = "Hello";
    print_size(v);
    print_size(s);
}

// ===================== advanced_usage() =====================

// ---- concept 组合 (&&, ||) ----

// 使用 && 组合：同时满足两个约束
template<typename T>
concept PrintableAndAddable = Printable<T> && Addable<T>;

// 使用 || 组合：满足其中一个约束即可
template<typename T>
concept NumberLike = std::integral<T> || std::floating_point<T>;

// ---- concept 细化 (refinement) ----

// 基础 concept：支持相等比较
template<typename T>
concept EqualityComparable = requires(T a, T b) {
    { a == b } -> std::convertible_to<bool>;
    { a != b } -> std::convertible_to<bool>;
};

// 细化 concept：在相等比较的基础上，增加排序比较
template<typename T>
concept Ordered = EqualityComparable<T> && requires(T a, T b) {
    { a < b }  -> std::convertible_to<bool>;
    { a > b }  -> std::convertible_to<bool>;
    { a <= b } -> std::convertible_to<bool>;
    { a >= b } -> std::convertible_to<bool>;
};

// 更进一步细化：有界的有序类型
template<typename T>
concept BoundedOrdered = Ordered<T> && requires {
    { std::numeric_limits<T>::min() } -> std::same_as<T>;
    { std::numeric_limits<T>::max() } -> std::same_as<T>;
};

// ---- subsumption 规则演示 ----
// 当多个重载都满足时，编译器选择约束更严格的版本

// 版本1：只要求可比较
template<EqualityComparable T>
std::string describe_type(T) {
    return "可比较类型";
}

// 版本2：要求有序（更严格，因为 Ordered 包含 EqualityComparable）
template<Ordered T>
std::string describe_type(T) {
    return "有序类型";
}

// 版本3：要求有界有序（最严格）
template<BoundedOrdered T>
std::string describe_type(T) {
    return "有界有序类型";
}

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // concept 组合演示
    auto add_and_print = []<PrintableAndAddable T>(T a, T b) {
        T result = a + b;
        std::cout << "结果: " << result << "\n";
    };

    add_and_print(10, 20);
    add_and_print(std::string("C++"), std::string("20"));

    auto process_number = []<NumberLike T>(T value) {
        std::cout << "处理数字: " << value << " (大小: " << sizeof(T) << " 字节)\n";
    };

    process_number(42);
    process_number(3.14);
    process_number(static_cast<short>(7));

    // subsumption 演示
    std::cout << "\n-- Subsumption 规则演示 --\n";
    std::cout << "int 类型: " << describe_type(42) << "\n";       // 选择最严格的 BoundedOrdered
    std::cout << "double 类型: " << describe_type(3.14) << "\n";  // 选择 BoundedOrdered

    // concept 细化链展示
    std::cout << "\n-- Concept 细化链 --\n";
    std::cout << "EqualityComparable<int>: "
              << std::boolalpha << EqualityComparable<int> << "\n";
    std::cout << "Ordered<int>: "
              << Ordered<int> << "\n";
    std::cout << "BoundedOrdered<int>: "
              << BoundedOrdered<int> << "\n";
    std::cout << "BoundedOrdered<std::string>: "
              << BoundedOrdered<std::string> << "\n";  // false，string 没有 numeric_limits
}

// ===================== pitfalls() =====================

// 陷阱1演示：concept 中的短路求值
template<typename T>
concept HasValueType = requires { typename T::value_type; };

template<typename T>
concept IterableContainer = HasValueType<T> && requires(T c) {
    { c.begin() };
    { c.end() };
};
// 如果 T 没有 value_type，&& 会短路，不会检查 begin()/end()

// 陷阱2演示：concept 不检查语义，只检查语法
template<typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：concept 组合的短路求值
    std::cout << "1. concept 组合使用短路求值:\n";
    std::cout << "   HasValueType<int>: " << std::boolalpha << HasValueType<int> << "\n";
    std::cout << "   HasValueType<vector<int>>: " << HasValueType<std::vector<int>> << "\n";
    std::cout << "   IterableContainer<int>: " << IterableContainer<int> << "\n";
    std::cout << "   (int 没有 value_type，&& 短路，不检查 begin/end)\n\n";

    // 陷阱2：concept 只检查语法有效性，不保证语义正确
    std::cout << "2. concept 只检查表达式是否合法（语法），不检查行为是否正确（语义）\n";
    std::cout << "   例如：某类型的 operator< 可能不满足严格弱序，\n";
    std::cout << "   但 Ordered concept 仍然会通过\n\n";

    // 陷阱3：concept 不能递归
    std::cout << "3. concept 定义不能直接递归引用自身\n";
    std::cout << "   需要借助间接方式（如辅助模板）来实现递归约束\n\n";

    // 陷阱4：subsumption 只在原子约束相同时才生效
    std::cout << "4. Subsumption 要求约束中使用相同的原子 concept 表达式\n";
    std::cout << "   如果两个 concept 虽然逻辑等价但写法不同，\n";
    std::cout << "   编译器不会认为它们有包含关系\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== 自定义 Concepts 与组合约束 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: Concepts 高级 (自定义 concept、组合约束)](https://en.cppreference.com/w/cpp/language/constraints)
