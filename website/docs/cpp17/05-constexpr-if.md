---
sidebar_position: 6
title: "if constexpr (编译期 if)"
description: "C++17 if constexpr (编译期 if) 教学示例"
tags: [cpp17, constexpr_if]
---

# if constexpr (编译期 if)

C++17 标准引入的 if constexpr (编译期 if) 特性完整教学示例。

## 完整源码

```cpp title="cpp17/05_constexpr_if.cpp"
/**
 * ============================================================
 * 特性名称: if constexpr (编译期 if)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: 在编译期根据常量表达式的值选择性地编译代码分支。
 *          不满足条件的分支会被完全丢弃（不实例化），而非仅仅
 *          不执行。
 *
 * [为什么引入]: C++14 中实现类型派发需要 SFINAE、标签派发、
 *              特化等复杂技巧。if constexpr 让编译期条件分支
 *              像普通 if 一样直观。
 *
 * [关键要点]:
 *   - 条件必须是编译期常量表达式
 *   - 未选中的分支不会被实例化 → 可以包含对当前类型无效的代码
 *   - 常与 type_traits (is_integral, is_same 等) 配合使用
 *   - 可替代大量 SFINAE 和模板特化代码
 *   - 可用于递归模板终止条件
 * ============================================================
 */

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <array>

// ============================================================
// 基础用法
// ============================================================

// 1. 根据类型选择不同处理方式
template<typename T>
std::string type_to_string(T value) {
    if constexpr (std::is_integral_v<T>) {
        return "整数: " + std::to_string(value);
    } else if constexpr (std::is_floating_point_v<T>) {
        return "浮点数: " + std::to_string(value);
    } else {
        return "其他类型";
    }
}

// 2. 替代 SFINAE
//    C++14 SFINAE 写法：
//    template<typename T>
//    std::enable_if_t<std::is_integral_v<T>> process(T v) { ... }
//    template<typename T>
//    std::enable_if_t<!std::is_integral_v<T>> process(T v) { ... }

//    C++17 if constexpr 写法：
template<typename T>
void process(T value) {
    if constexpr (std::is_integral_v<T>) {
        std::cout << "处理整数: " << value * 2 << "\n";
    } else if constexpr (std::is_floating_point_v<T>) {
        std::cout << "处理浮点: " << value + 0.5 << "\n";
    } else {
        std::cout << "处理其他: " << value << "\n";
    }
}

void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 类型到字符串
    std::cout << type_to_string(42) << "\n";
    std::cout << type_to_string(3.14) << "\n";

    // 类型派发处理
    process(10);
    process(2.5);
    process(std::string("hello"));
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 递归模板终止 —— 编译期计算参数个数并打印
template<typename T, typename... Rest>
void print_values(T first, Rest... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0) {
        std::cout << ", ";
        print_values(rest...);  // 递归展开
    } else {
        std::cout << "\n";      // 终止条件
    }
}

// 2. 编译期阶乘
template<int N>
constexpr int factorial() {
    if constexpr (N <= 1) {
        return 1;
    } else {
        return N * factorial<N - 1>();
    }
}

// 3. 根据容器类型选择遍历方式
template<typename Container>
void smart_print(const Container& c) {
    if constexpr (std::is_array_v<std::remove_reference_t<decltype(c)>>) {
        std::cout << "原生数组: ";
    } else {
        std::cout << "容器(size=" << c.size() << "): ";
    }
    for (const auto& elem : c) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

// 4. SFINAE 辅助 —— 检测类型是否有 .size() 成员
template<typename T, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>>
    : std::true_type {};

template<typename T>
auto get_size(const T& t) {
    if constexpr (has_size<T>::value) {
        return t.size();
    } else {
        return 0;
    }
}

// 5. 类型安全的转换函数
template<typename To, typename From>
To safe_cast(From value) {
    if constexpr (std::is_same_v<To, From>) {
        // 相同类型，直接返回
        return value;
    } else if constexpr (std::is_convertible_v<From, To>) {
        // 可隐式转换
        return static_cast<To>(value);
    } else {
        static_assert(std::is_convertible_v<From, To>,
                      "无法转换的类型!");
        return To{};
    }
}

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 递归打印
    std::cout << "递归打印: ";
    print_values(1, "hello", 3.14, 'A');

    // 编译期阶乘
    constexpr int f5 = factorial<5>();
    std::cout << "5! = " << f5 << "\n";
    constexpr int f10 = factorial<10>();
    std::cout << "10! = " << f10 << "\n";

    // 智能打印容器
    std::vector<int> v{1, 2, 3};
    smart_print(v);

    // 安全转换
    auto i = safe_cast<int>(3.14);
    std::cout << "safe_cast<int>(3.14) = " << i << "\n";
    auto s = safe_cast<std::string>(std::string("hello"));
    std::cout << "safe_cast<string>(\"hello\") = " << s << "\n";
}

// ============================================================
// 易错点 / 陷阱
// ============================================================

// 示例：展示 if constexpr 的威力 —— 被丢弃的分支不需要合法
template<typename T>
void demo_discard(T value) {
    if constexpr (std::is_integral_v<T>) {
        // 仅对整数类型调用 value % 2
        std::cout << value << " 是" << (value % 2 == 0 ? "偶数" : "奇数") << "\n";
    } else {
        // 对于 string 类型，value % 2 不合法
        // 但因为 if constexpr，string 实例化时这个分支被丢弃
        std::cout << "不是整数类型\n";
    }
}

void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: 条件必须是编译期常量
    // int x = 5;
    // if constexpr (x > 0) { }  // 错误！x 不是 constexpr
    std::cout << "陷阱1 - 条件必须是编译期常量表达式\n";

    // 陷阱 2: if constexpr 不是 #if —— 它仍然需要语法正确
    //         被丢弃的分支必须语法正确，只是不做语义检查
    std::cout << "陷阱2 - 被丢弃的分支仍需语法正确\n";

    // 陷阱 3: 在非模板上下文中，两个分支都会被检查
    //         if constexpr 的「丢弃」特性只在模板中生效
    // void non_template_func() {
    //     if constexpr (false) {
    //         // 这里的代码在非模板函数中仍然会被编译器检查!
    //     }
    // }
    std::cout << "陷阱3 - 非模板函数中 if constexpr 两个分支都会被检查\n";

    // 陷阱 4: 展示被丢弃分支的威力
    demo_discard(42);
    demo_discard(std::string("hello"));

    // 陷阱 5: 不要混淆 constexpr if 和 runtime if
    constexpr int N = 10;
    if constexpr (N > 5) {
        std::cout << "陷阱5 - 编译期确定: N > 5\n";
    }
    // 等价于直接写 std::cout << "编译期确定: N > 5\n";
    // 编译器会优化掉条件判断
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== if constexpr (编译期 if) - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: if constexpr (编译期 if)](https://en.cppreference.com/w/cpp/language/constexpr)
