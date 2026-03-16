---
sidebar_position: 2
slug: 01-generic-lambdas
title: "泛型Lambda（auto参数）"
description: "C++14 泛型Lambda（auto参数） 教学示例"
tags: [cpp14, generic_lambdas]
---

# 泛型Lambda（auto参数）

C++14 标准引入的 泛型Lambda（auto参数） 特性完整教学示例。

## 完整源码

```cpp title="cpp14/01_generic_lambdas.cpp"
/**
 * ============================================================
 * 特性名称: 泛型Lambda（auto参数）
 * 所属标准: C++14
 * ============================================================
 *
 * [是什么]: Lambda表达式的参数可以使用auto关键字，使Lambda具有模板函数的能力，
 *          编译器会为每种调用的参数类型自动生成对应的operator()。
 *
 * [为什么引入]: C++11中Lambda的参数类型必须明确指定，当需要对多种类型执行相同
 *              操作时，必须编写多个Lambda或转而使用模板函数。泛型Lambda消除了
 *              这种冗余，使Lambda真正成为轻量级的多态可调用对象。
 *
 * [关键要点]:
 *   - auto参数使Lambda的operator()变为模板成员函数
 *   - 可以有多个auto参数，每个独立推导类型
 *   - 结合decltype和完美转发可实现通用包装器
 *   - 泛型Lambda本质上是带模板operator()的匿名函数对象
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <type_traits>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 最简单的泛型Lambda：参数使用auto
    auto print = [](auto value) {
        std::cout << "  值: " << value << "\n";
    };

    print(42);              // auto推导为int
    print(3.14);            // auto推导为double
    print("hello C++14");   // auto推导为const char*

    // 2. 多个auto参数，各自独立推导
    auto add = [](auto a, auto b) {
        return a + b;
    };

    std::cout << "  int + int     = " << add(1, 2) << "\n";
    std::cout << "  double + int  = " << add(1.5, 2) << "\n";
    std::cout << "  string + char*= " << add(std::string("Hello, "), "World!") << "\n";

    // 3. 在STL算法中使用泛型Lambda
    std::vector<int> nums = {5, 3, 1, 4, 2};
    // 同一个Lambda可用于不同容器类型的排序
    auto less_than = [](auto a, auto b) { return a < b; };

    std::sort(nums.begin(), nums.end(), less_than);
    std::cout << "  排序后: ";
    for (auto n : nums) std::cout << n << " ";
    std::cout << "\n";

    std::vector<std::string> words = {"banana", "apple", "cherry"};
    std::sort(words.begin(), words.end(), less_than);
    std::cout << "  排序后: ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << "\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 辅助：普通模板函数，用于对比
template<typename T>
void template_print(const T& value) {
    std::cout << "  [模板函数] " << value << "\n";
}

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. 泛型Lambda vs 模板函数
    // 泛型Lambda可以赋给变量、存入容器，模板函数不行
    auto lambda_print = [](const auto& value) {
        std::cout << "  [泛型Lambda] " << value << "\n";
    };

    lambda_print(100);
    lambda_print("text");
    template_print(100);
    template_print("text");

    // Lambda可作为变量传递给其他函数，模板函数需要显式实例化才行
    auto apply = [](auto func, auto arg) {
        func(arg);
    };
    apply(lambda_print, 999);

    // 2. 完美转发Lambda（C++14惯用法）
    // 使用auto&&参数 + std::forward实现完美转发
    auto forwarder = [](auto&& ...args) {
        // 展示参数包的大小
        std::cout << "  转发了 " << sizeof...(args) << " 个参数\n";
    };
    forwarder(1, "two", 3.0);

    // 3. 实际的完美转发包装器
    auto make_pair_wrapper = [](auto&& a, auto&& b) {
        return std::make_pair(std::forward<decltype(a)>(a),
                              std::forward<decltype(b)>(b));
    };
    auto p = make_pair_wrapper(42, std::string("hello"));
    std::cout << "  pair = (" << p.first << ", " << p.second << ")\n";

    // 4. 多态Lambda：利用auto实现类型分支
    auto type_describer = [](const auto& val) {
        // 编译期类型判断
        using T = std::decay_t<decltype(val)>;
        if (std::is_integral<T>::value)
            std::cout << "  整数类型: " << val << "\n";
        else if (std::is_floating_point<T>::value)
            std::cout << "  浮点类型: " << val << "\n";
        else
            std::cout << "  其他类型: " << val << "\n";
    };

    type_describer(42);
    type_describer(3.14);
    type_describer("C++14");

    // 5. 泛型Lambda递归（通过传递自身）
    // 阶乘Lambda
    auto factorial = [](auto self, int n) -> int {
        return n <= 1 ? 1 : n * self(self, n - 1);
    };
    std::cout << "  5! = " << factorial(factorial, 5) << "\n";
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱1：auto参数的每种类型组合都会生成一个新的operator()实例
    // 这可能导致代码膨胀
    auto bloat = [](auto x) { return x * 2; };
    bloat(1);           // 生成 operator()<int>
    bloat(1.0);         // 生成 operator()<double>
    bloat(1.0f);        // 生成 operator()<float>
    std::cout << "  陷阱1: 每种类型组合都实例化一份operator()，注意代码膨胀\n";

    // 陷阱2：泛型Lambda不能直接取地址或用于需要具体函数指针的场景
    // auto generic = [](auto x) { return x; };
    // int (*fp)(int) = generic;  // 编译错误！泛型Lambda不能隐式转换为函数指针
    // 非泛型Lambda可以：
    auto specific = [](int x) { return x; };
    int (*fp)(int) = specific;  // OK，非泛型无捕获Lambda可转换
    std::cout << "  陷阱2: 泛型Lambda不能隐式转为函数指针，fp(10) = " << fp(10) << "\n";

    // 陷阱3：auto参数推导可能出乎意料
    auto tricky = [](auto x) {
        // 如果传入字符串字面量，x是const char*，不是std::string
        return x;
    };
    auto result = tricky("hello");
    // result的类型是const char*，不是std::string
    std::cout << "  陷阱3: tricky(\"hello\") 返回类型是 const char*，不是 std::string\n";
    std::cout << "    is_same<const char*, decltype(result)> = "
              << std::is_same<const char*, decltype(result)>::value << "\n";

    // 陷阱4：auto&&不一定是右值引用，它是转发引用
    auto check_ref = [](auto&& x) {
        using T = decltype(x);
        if (std::is_lvalue_reference<T>::value)
            std::cout << "  陷阱4: 左值引用\n";
        else
            std::cout << "  陷阱4: 右值引用\n";
    };
    int val = 10;
    check_ref(val);     // 左值 -> auto&& 推导为 int&
    check_ref(42);      // 右值 -> auto&& 推导为 int&&
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 泛型Lambda（auto参数） - C++14 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 泛型Lambda（auto参数）](https://en.cppreference.com/w/cpp/language/lambda)
