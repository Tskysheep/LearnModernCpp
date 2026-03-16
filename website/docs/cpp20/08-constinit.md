---
sidebar_position: 9
slug: 08-constinit
title: "constinit"
description: "C++20 constinit 教学示例"
tags: [cpp20, constinit]
---

# constinit

C++20 标准引入的 constinit 特性完整教学示例。

## 完整源码

```cpp title="cpp20/08_constinit.cpp"
/**
 * ============================================================
 * 特性名称: constinit
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: constinit 确保变量在编译期初始化（常量初始化），
 *          但变量本身在运行时是可变的（不同于 constexpr）。
 *
 * [为什么引入]: 防止静态初始化顺序问题（Static Initialization Order Fiasco, SIOF）。
 *             当多个翻译单元中的静态变量相互依赖时，
 *             初始化顺序不确定，可能导致使用未初始化的对象。
 *
 * [关键要点]:
 *   - constinit 确保变量在编译期就被初始化（常量初始化阶段）
 *   - 变量在运行时是可变的（与 constexpr 变量不同）
 *   - 只能用于 static 或 thread_local 变量
 *   - 如果初始化器不是常量表达式，编译错误
 * ============================================================
 */

#include <iostream>
#include <string>
#include <array>
#include <mutex>

// ===================== basic_usage() =====================

// constinit 用于全局/静态变量
constinit int global_counter = 0;       // 编译期初始化为 0
constinit double pi_approx = 3.14159;   // 编译期初始化

// constexpr 对比：也是编译期初始化，但值不可变
constexpr int MAX_SIZE = 100;           // 编译期常量，不可修改

// const 对比：不保证编译期初始化
const int some_const = 42;              // 可能是编译期，也可能是运行时

// constinit 配合 constexpr 函数
constexpr int compute_initial_value() {
    return 42 * 10;
}
constinit int computed_value = compute_initial_value();  // 编译期计算并初始化

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // constinit 变量的值可以在运行时修改
    std::cout << "global_counter 初始值: " << global_counter << "\n";
    global_counter = 100;  // 可以修改！
    std::cout << "global_counter 修改后: " << global_counter << "\n";

    std::cout << "pi_approx 初始值: " << pi_approx << "\n";
    pi_approx = 3.14159265358979;  // 也可以修改
    std::cout << "pi_approx 修改后: " << pi_approx << "\n";

    std::cout << "computed_value: " << computed_value << "\n";

    // 对比三者
    std::cout << "\n-- const vs constexpr vs constinit 对比 --\n";
    std::cout << "const:      编译期或运行时初始化，值不可变\n";
    std::cout << "constexpr:  必须编译期初始化，值不可变\n";
    std::cout << "constinit:  必须编译期初始化，值可变\n";

    // 局部 static 变量也可以用 constinit
    static constinit int local_static = 0;
    local_static++;
    std::cout << "\n局部 static constinit 变量: " << local_static << "\n";
}

// ===================== advanced_usage() =====================

// SIOF 问题演示（概念性）
// 文件 a.cpp:  int a = compute_a();       // 运行时初始化
// 文件 b.cpp:  int b = a + 1;             // 依赖 a，但 a 可能还没初始化!

// 使用 constinit 解决：
// 文件 a.cpp:  constinit int a = 42;      // 编译期初始化，保证在任何动态初始化之前
// 文件 b.cpp:  constinit int b = 42 + 1;  // 也是编译期初始化

// constinit 与 thread_local
constinit thread_local int tls_counter = 0;

// constinit 数组
constexpr auto make_lookup_table() {
    std::array<int, 10> table{};
    for (int i = 0; i < 10; ++i) {
        table[static_cast<std::size_t>(i)] = i * i;
    }
    return table;
}

constinit auto lookup_table = make_lookup_table();

// constinit 与指针
constinit const char* greeting = "Hello, C++20!";  // 字符串字面量的地址是编译期常量
constinit int* null_ptr = nullptr;                   // nullptr 是编译期常量

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // SIOF 问题说明
    std::cout << "-- 静态初始化顺序问题 (SIOF) --\n";
    std::cout << "问题：多个翻译单元的全局变量初始化顺序不确定\n";
    std::cout << "解决：constinit 保证在编译期初始化，不依赖顺序\n\n";

    // thread_local + constinit
    std::cout << "-- thread_local + constinit --\n";
    std::cout << "tls_counter 初始值: " << tls_counter << "\n";
    tls_counter = 42;
    std::cout << "tls_counter 修改后: " << tls_counter << "\n";

    // 查找表
    std::cout << "\n-- 编译期初始化的查找表 --\n";
    std::cout << "平方表: ";
    for (const auto& v : lookup_table) {
        std::cout << v << " ";
    }
    std::cout << "\n";

    // 修改查找表（constinit 允许！）
    lookup_table[0] = 999;
    std::cout << "修改后: " << lookup_table[0] << " (constinit 允许运行时修改)\n";

    // constinit 指针
    std::cout << "\n-- constinit 指针 --\n";
    std::cout << "greeting: " << greeting << "\n";
    greeting = "Modified greeting";  // 可以修改指针指向
    std::cout << "修改后: " << greeting << "\n";

    // 初始化阶段说明
    std::cout << "\n-- C++ 变量初始化阶段 --\n";
    std::cout << "1. 零初始化 (zero initialization)\n";
    std::cout << "2. 常量初始化 (constant initialization) ← constinit 保证在此阶段\n";
    std::cout << "3. 动态初始化 (dynamic initialization) ← 普通全局变量可能在此阶段\n";
    std::cout << "constinit 保证变量在阶段2完成初始化，避免阶段3的顺序问题\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：constinit 不意味着 const
    std::cout << "1. constinit 不等于 const:\n";
    std::cout << "   constinit int x = 0;       // 编译期初始化，运行时可变\n";
    std::cout << "   constinit const int x = 0;  // 编译期初始化，运行时不可变\n";
    std::cout << "   如果需要不可变，使用 constexpr 或 constinit const\n\n";

    // 陷阱2：只能用于 static/thread_local 变量
    std::cout << "2. constinit 只能用于 static 存储期或 thread_local 变量:\n";
    std::cout << "   constinit int x = 0;              // 全局变量 → 合法\n";
    std::cout << "   static constinit int x = 0;       // 局部 static → 合法\n";
    std::cout << "   constinit thread_local int x = 0;  // thread_local → 合法\n";
    std::cout << "   void f() { constinit int x = 0; }  // 局部自动变量 → 编译错误!\n\n";

    // 陷阱3：初始化器必须是常量表达式
    std::cout << "3. 初始化器必须是常量表达式:\n";
    std::cout << "   constinit int x = 42;               // 合法\n";
    std::cout << "   constinit int y = constexpr_func();  // 合法（如果函数是 constexpr）\n";
    std::cout << "   constinit int z = runtime_func();    // 编译错误!\n\n";

    // 陷阱4：constinit 不能用于类成员
    std::cout << "4. constinit 不能用于非静态类成员:\n";
    std::cout << "   struct S { constinit int x = 0; };  // 编译错误!\n";
    std::cout << "   struct S { static constinit int x; }; // 合法\n\n";

    // 实际对比
    std::cout << "-- 三者对比总结 --\n";
    std::cout << "┌──────────┬────────────┬──────────┬──────────────┐\n";
    std::cout << "│          │ 编译期初始化 │ 运行时可变 │ 存储期限制    │\n";
    std::cout << "├──────────┼────────────┼──────────┼──────────────┤\n";
    std::cout << "│ const    │ 不保证      │ 否       │ 无           │\n";
    std::cout << "│ constexpr│ 是          │ 否       │ 无           │\n";
    std::cout << "│ constinit│ 是          │ 是       │ static/tls   │\n";
    std::cout << "└──────────┴────────────┴──────────┴──────────────┘\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== constinit - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: constinit](https://en.cppreference.com/w/cpp/language/constinit)
