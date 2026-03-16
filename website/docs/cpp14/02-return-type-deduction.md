---
sidebar_position: 3
slug: 02-return-type-deduction
title: "函数返回类型推导"
description: "C++14 函数返回类型推导 教学示例"
tags: [cpp14, return_type_deduction]
---

# 函数返回类型推导

C++14 标准引入的 函数返回类型推导 特性完整教学示例。

## 完整源码

```cpp title="cpp14/02_return_type_deduction.cpp"
/**
 * ============================================================
 * 特性名称: 函数返回类型推导
 * 所属标准: C++14
 * ============================================================
 *
 * [是什么]: C++14允许普通函数使用auto或decltype(auto)作为返回类型，
 *          编译器根据return语句自动推导返回类型，无需手动指定。
 *
 * [为什么引入]: C++11中auto返回类型仅能在Lambda中使用，普通函数需要
 *              尾置返回类型(-> decltype(...))，语法冗长且容易出错。
 *              C++14的返回类型推导使代码更简洁，尤其在模板编程中。
 *
 * [关键要点]:
 *   - auto推导遵循模板参数推导规则（去掉引用和cv限定）
 *   - decltype(auto)保留表达式的精确类型（包括引用和cv限定）
 *   - 多个return语句必须推导出相同类型
 *   - 递归函数中，递归调用前需要至少一个非递归return语句
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

// ============================================================
// 基础用法
// ============================================================

// 1. 最简单的auto返回类型推导
auto multiply(int a, int b) {
    return a * b;   // 推导为int
}

// 2. 模板函数中的auto推导
template<typename T, typename U>
auto flexible_add(T a, U b) {
    return a + b;   // 根据a+b的表达式类型推导
}

// 3. decltype(auto) —— 保留精确类型
int global_value = 42;

decltype(auto) get_ref() {
    return (global_value);  // 注意括号！推导为int&
}

auto get_copy() {
    return global_value;    // 推导为int（值拷贝）
}

void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // auto推导
    auto result1 = multiply(3, 4);
    std::cout << "  multiply(3, 4) = " << result1 << "\n";

    // 模板中的auto推导
    auto result2 = flexible_add(1, 2.5);  // int + double -> double
    std::cout << "  flexible_add(1, 2.5) = " << result2 << "\n";
    std::cout << "  返回类型是double? "
              << std::is_same<double, decltype(result2)>::value << "\n";

    auto result3 = flexible_add(std::string("Hello "), "World");
    std::cout << "  flexible_add(string, char*) = " << result3 << "\n";

    // decltype(auto)与auto的区别
    decltype(auto) ref = get_ref();   // int& —— 引用
    auto copy = get_copy();            // int —— 拷贝

    std::cout << "  global_value = " << global_value << "\n";
    ref = 100;  // 通过引用修改global_value
    std::cout << "  修改ref后, global_value = " << global_value << "\n";

    std::cout << "  get_ref()返回引用? "
              << std::is_reference<decltype(get_ref())>::value << "\n";
    std::cout << "  get_copy()返回引用? "
              << std::is_reference<decltype(get_copy())>::value << "\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 与C++11尾置返回类型对比
// C++11写法：需要尾置返回类型
template<typename T, typename U>
auto cpp11_add(T a, U b) -> decltype(a + b) {
    return a + b;
}

// C++14写法：简洁明了
template<typename T, typename U>
auto cpp14_add(T a, U b) {
    return a + b;
}

// 2. decltype(auto)用于完美转发返回值
template<typename Container>
decltype(auto) get_first(Container& c) {
    return c[0];   // 如果Container::operator[]返回引用，这里也返回引用
}

// 3. 递归函数中的auto推导
auto fibonacci(int n) -> int {
    // 注意：递归函数推荐使用尾置返回类型或确保编译器能在递归前看到return类型
    if (n <= 1) return n;       // 从此处推导返回类型为int
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// 4. Lambda中的auto返回类型（C++14增强）
// C++14中Lambda也可以使用auto返回类型推导

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. C++11 vs C++14对比
    std::cout << "  C++11写法: " << cpp11_add(1, 2.0) << "\n";
    std::cout << "  C++14写法: " << cpp14_add(1, 2.0) << "\n";

    // 2. decltype(auto)完美转发返回值
    std::vector<int> vec = {10, 20, 30};
    decltype(auto) first = get_first(vec);  // int& —— vector::operator[]返回引用
    std::cout << "  first = " << first << "\n";
    first = 99;  // 修改了vec[0]
    std::cout << "  修改first后, vec[0] = " << vec[0] << "\n";

    // 3. 递归
    std::cout << "  fibonacci(10) = " << fibonacci(10) << "\n";

    // 4. Lambda中的返回类型推导
    auto make_adder = [](auto x) {
        return [x](auto y) {       // 返回另一个Lambda
            return x + y;          // 多层auto推导
        };
    };
    auto add5 = make_adder(5);
    std::cout << "  add5(3) = " << add5(3) << "\n";
    std::cout << "  add5(3.14) = " << add5(3.14) << "\n";

    // 5. auto用于条件表达式——两个分支必须是同一类型
    auto safe_div = [](double a, double b) -> auto {
        if (b == 0.0) return 0.0;  // double
        return a / b;               // double —— 类型一致
    };
    std::cout << "  safe_div(10, 3) = " << safe_div(10, 3) << "\n";
    std::cout << "  safe_div(10, 0) = " << safe_div(10, 0) << "\n";
}

// ============================================================
// 易错点与陷阱
// ============================================================

// 陷阱1演示：多return语句类型必须一致
// auto bad_func(bool flag) {
//     if (flag) return 1;      // int
//     else return 1.0;         // double —— 编译错误！类型不一致
// }

// 陷阱2演示：auto去引用，decltype(auto)保留引用
int& get_global_ref() {
    return global_value;
}

// 陷阱3演示：递归函数的限制
// auto bad_recursive(int n) {
//     if (n > 0) return bad_recursive(n - 1);  // 错误：递归调用前无法推导类型
//     return 0;
// }
// 正确做法：将非递归return放在前面
auto good_recursive(int n) {
    if (n <= 0) return 0;                        // 先确定返回类型为int
    return good_recursive(n - 1) + 1;            // 现在可以递归了
}

void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱1：多return语句类型不一致会导致编译错误
    std::cout << "  陷阱1: 多个return语句必须推导出相同类型\n";
    std::cout << "    auto f(bool b) { if(b) return 1; else return 1.0; } // 编译错误!\n";

    // 陷阱2：auto vs decltype(auto)的引用丢失
    auto val_copy = get_global_ref();        // auto -> int (引用被去掉)
    decltype(auto) val_ref = get_global_ref(); // decltype(auto) -> int& (保留引用)

    global_value = 42;
    val_copy = 200;
    std::cout << "  陷阱2: auto丢失引用\n";
    std::cout << "    auto赋值200后, global = " << global_value << " (未改变)\n";
    val_ref = 200;
    std::cout << "    decltype(auto)赋值200后, global = " << global_value << " (改变了)\n";

    // 陷阱3：递归函数中的推导顺序
    std::cout << "  陷阱3: 递归函数需要在递归调用之前有非递归的return语句\n";
    std::cout << "    good_recursive(5) = " << good_recursive(5) << "\n";

    // 陷阱4：decltype(auto)中括号的影响
    int x = 10;
    decltype(auto) a = x;      // decltype(x) -> int, 所以a是int
    decltype(auto) b = (x);    // decltype((x)) -> int&, 所以b是int&

    b = 99;
    std::cout << "  陷阱4: decltype(auto) b = (x) 是引用!\n";
    std::cout << "    修改b后, x = " << x << "\n";
    std::cout << "    a是引用? " << std::is_reference<decltype(a)>::value << "\n";
    std::cout << "    b是引用? " << std::is_reference<decltype(b)>::value << "\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 函数返回类型推导 - C++14 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 函数返回类型推导](https://en.cppreference.com/w/cpp/language/function#Return_type_deduction)
