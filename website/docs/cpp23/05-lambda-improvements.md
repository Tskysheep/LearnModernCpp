---
sidebar_position: 6
title: "Lambda 改进（C++23）"
description: "C++23 Lambda 改进（C++23） 教学示例"
tags: [cpp23, lambda_improvements]
---

# Lambda 改进（C++23）

C++23 标准引入的 Lambda 改进（C++23） 特性完整教学示例。

## 完整源码

```cpp title="cpp23/05_lambda_improvements.cpp"
/**
 * ============================================================
 * 特性名称: Lambda 改进（C++23）
 * 所属标准: C++23
 * ============================================================
 *
 * [是什么]: C++23 对 Lambda 表达式进行了多项改进：
 *          1. 当 Lambda 有属性（attributes）时，可以省略参数列表的 ()
 *          2. 引入 static Lambda，允许 Lambda 不捕获任何内容时
 *             声明为 static，提示编译器更好地优化
 *
 * [为什么引入]:
 *   - 省略 () 的改进使语法更一致（C++20 已允许无属性时省略）
 *   - static Lambda 明确表达"无状态"意图，可能减少间接调用开销
 *
 * [关键要点]:
 *   - static Lambda: []() static { ... }，不能捕获任何变量
 *   - 有属性时省略 (): [] [[nodiscard]] { return 42; }
 *   - static Lambda 本质上等同于普通函数，可以被内联优化
 *   - static Lambda 不能有捕获列表（除了空捕获）
 * ============================================================
 */

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <string>

// ============================================================
// 特性检测
// ============================================================

// static Lambda 检测
#ifdef __cpp_static_call_operator
#define HAS_STATIC_LAMBDA 1
#else
#define HAS_STATIC_LAMBDA 0
#endif

// Lambda 属性改进：通常与 C++23 一起支持
#if __cplusplus >= 202302L
#define HAS_LAMBDA_ATTRS 1
#else
#define HAS_LAMBDA_ATTRS 0
#endif

// ============================================================
// 基础用法
// ============================================================
namespace basic_usage {

void demonstrate() {
    std::cout << "[基础用法]" << std::endl;

    // --- 部分 1: Lambda 省略 () 的改进 ---
    std::cout << "  -- Lambda 参数列表省略 --" << std::endl;

#if HAS_LAMBDA_ATTRS
    // C++23: 带属性的 Lambda 也可以省略 ()
    auto get_value = [] [[nodiscard]] { return 42; };
    std::cout << "  [] [[nodiscard]] { return 42; } => " << get_value() << std::endl;

    // 对比各版本的写法
    // C++11: []() -> int { return 42; }  // 必须写 ()
    // C++20: [] { return 42; }           // 无属性时可省略 ()
    // C++23: [] [[nodiscard]] { return 42; }  // 有属性也可省略 ()
#else
    std::cout << "  [不支持] 编译器未支持 Lambda 属性改进" << std::endl;
    std::cout << "  C++20 已支持无属性时省略 ():" << std::endl;
    auto get_value = [] { return 42; };
    std::cout << "  [] { return 42; } => " << get_value() << std::endl;
#endif

    // --- 部分 2: static Lambda ---
    std::cout << "  -- static Lambda --" << std::endl;

#if HAS_STATIC_LAMBDA
    // static Lambda：明确表示不需要捕获
    auto add = [](int a, int b) static { return a + b; };
    std::cout << "  static lambda add(3, 4) = " << add(3, 4) << std::endl;

    // 用作回调
    auto square = [](int x) static -> int { return x * x; };
    std::cout << "  static lambda square(5) = " << square(5) << std::endl;

    // 在算法中使用
    std::vector<int> nums = {1, 2, 3, 4, 5};
    std::vector<int> results;
    std::transform(nums.begin(), nums.end(), std::back_inserter(results),
                   [](int x) static { return x * 2; });
    std::cout << "  transform with static lambda: [";
    for (std::size_t i = 0; i < results.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << results[i];
    }
    std::cout << "]" << std::endl;
#else
    std::cout << "  [不支持] 编译器未支持 static Lambda" << std::endl;
    std::cout << "  普通无捕获 Lambda 作为替代:" << std::endl;
    auto add = [](int a, int b) { return a + b; };
    std::cout << "  lambda add(3, 4) = " << add(3, 4) << std::endl;
#endif

    std::cout << std::endl;
}

} // namespace basic_usage

// ============================================================
// 进阶用法
// ============================================================
namespace advanced_usage {

// 辅助：接受函数指针的 C 风格 API
using CCallback = int(*)(int, int);

int apply_operation(int a, int b, CCallback op) {
    return op(a, b);
}

void demonstrate() {
    std::cout << "[进阶用法]" << std::endl;

#if HAS_STATIC_LAMBDA
    // 场景1：static Lambda 可以转换为函数指针
    std::cout << "  -- static Lambda 转函数指针 --" << std::endl;
    // static Lambda 和无捕获 Lambda 一样可以转换为函数指针
    auto multiply = [](int a, int b) static { return a * b; };

    // 传给需要函数指针的 C 风格 API
    int result = apply_operation(6, 7, multiply);
    std::cout << "  apply_operation(6, 7, multiply) = " << result << std::endl;

    // 场景2：static Lambda 的调用运算符是 static 成员函数
    std::cout << "  -- static 调用运算符的含义 --" << std::endl;
    std::cout << "  普通 Lambda: operator() 是非 static 成员函数" << std::endl;
    std::cout << "  static Lambda: operator() 是 static 成员函数" << std::endl;
    std::cout << "  区别: static 版本不需要 this 指针，可能更高效" << std::endl;

    // 场景3：static Lambda 与模板参数
    std::cout << "  -- static Lambda 在模板中 --" << std::endl;
    auto generic_static = []<typename T>(T x, T y) static { return x + y; };
    std::cout << "  generic_static(3, 4) = " << generic_static(3, 4) << std::endl;
    std::cout << "  generic_static(1.5, 2.5) = " << generic_static(1.5, 2.5) << std::endl;

    // 场景4：static Lambda 在编译期
    std::cout << "  -- static Lambda 在 constexpr 中 --" << std::endl;
    constexpr auto ct_add = [](int a, int b) static constexpr { return a + b; };
    constexpr int ct_result = ct_add(10, 20);
    std::cout << "  constexpr static lambda: ct_add(10, 20) = " << ct_result << std::endl;

    // 场景5：在排序中使用 static Lambda
    std::cout << "  -- 排序中使用 static Lambda --" << std::endl;
    std::vector<int> data = {5, 2, 8, 1, 9, 3};
    std::sort(data.begin(), data.end(),
              [](int a, int b) static { return a > b; });  // 降序
    std::cout << "  降序排序: [";
    for (std::size_t i = 0; i < data.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << data[i];
    }
    std::cout << "]" << std::endl;

#else
    std::cout << "  [不支持] 编译器未支持 static Lambda" << std::endl;
    std::cout << "  static Lambda 的优势（概念性说明）:" << std::endl;
    std::cout << "    1. 明确表达'无状态'意图" << std::endl;
    std::cout << "    2. operator() 不需要 this 指针" << std::endl;
    std::cout << "    3. 编译器可能生成更高效的代码" << std::endl;
    std::cout << "    4. 可以与函数指针互转" << std::endl;

    // 普通无捕获 Lambda 也可以转函数指针
    auto multiply = [](int a, int b) { return a * b; };
    int result = apply_operation(6, 7, multiply);
    std::cout << "  普通无捕获 Lambda 也能转函数指针: " << result << std::endl;
#endif

    std::cout << std::endl;
}

} // namespace advanced_usage

// ============================================================
// 易错点与陷阱
// ============================================================
namespace pitfalls {

void demonstrate() {
    std::cout << "[易错点与陷阱]" << std::endl;

#if HAS_STATIC_LAMBDA
    // 陷阱1：static Lambda 不能捕获变量
    std::cout << "  陷阱1: static Lambda 不能有任何捕获" << std::endl;
    std::cout << "    - [x](int y) static { ... }  // 编译错误！" << std::endl;
    std::cout << "    - [&](int y) static { ... }   // 编译错误！" << std::endl;
    std::cout << "    - [](int y) static { ... }    // 正确：空捕获" << std::endl;

    // 以下代码会导致编译错误（已注释）：
    // int x = 10;
    // auto bad = [x](int y) static { return x + y; };  // 错误！

    // 陷阱2：static 的位置
    std::cout << "  陷阱2: static 关键字的位置" << std::endl;
    std::cout << "    - [](int x) static { ... }        // 正确" << std::endl;
    std::cout << "    - [](int x) static -> int { ... }  // 正确" << std::endl;
    std::cout << "    - [] static (int x) { ... }        // 错误！位置不对" << std::endl;
#else
    std::cout << "  陷阱1: static Lambda 不能有任何捕获" << std::endl;
    std::cout << "    - 只能使用空捕获列表 []" << std::endl;
    std::cout << "  陷阱2: static 关键字必须在参数列表之后" << std::endl;
#endif

    // 陷阱3：与 static 局部变量的 Lambda 区分
    std::cout << "  陷阱3: 不要混淆 static Lambda 和含 static 变量的 Lambda" << std::endl;
    std::cout << "    - [](int x) static { ... }           // static Lambda (C++23)" << std::endl;
    std::cout << "    - [](int x) { static int s = 0; ... } // 含 static 变量的普通 Lambda" << std::endl;

    // 陷阱4：mutable 和 static 互斥
    std::cout << "  陷阱4: mutable 和 static 不能同时使用" << std::endl;
    std::cout << "    - mutable 意味着可以修改捕获的副本" << std::endl;
    std::cout << "    - static 意味着没有捕获" << std::endl;
    std::cout << "    - 二者互斥" << std::endl;

    std::cout << std::endl;
}

} // namespace pitfalls

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== Lambda 改进 - C++23 =====\n" << std::endl;

    basic_usage::demonstrate();
    advanced_usage::demonstrate();
    pitfalls::demonstrate();

    return 0;
}
```

## 参考链接

- [cppreference: Lambda 改进（C++23）](https://en.cppreference.com/w/cpp/language/lambda)
