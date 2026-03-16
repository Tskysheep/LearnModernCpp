---
sidebar_position: 2
title: "if consteval（编译期求值分支）"
description: "C++23 if consteval（编译期求值分支） 教学示例"
tags: [cpp23, if_consteval]
---

# if consteval（编译期求值分支）

C++23 标准引入的 if consteval（编译期求值分支） 特性完整教学示例。

## 完整源码

```cpp title="cpp23/01_if_consteval.cpp"
/**
 * ============================================================
 * 特性名称: if consteval（编译期求值分支）
 * 所属标准: C++23
 * ============================================================
 *
 * [是什么]: if consteval 是一种新的编译期分支语句，用于检测当前
 *          函数调用是否在常量求值上下文中执行。如果是，则走
 *          consteval 分支；否则走 else 分支。
 *
 * [为什么引入]: 在 C++20 中，consteval 函数只能在编译期调用，
 *              constexpr 函数既可编译期也可运行期。但有时我们希望
 *              在同一个 constexpr 函数中根据"当前是否为编译期"
 *              选择不同实现路径。if consteval 正好解决了这一需求。
 *
 * [关键要点]:
 *   - if consteval 不需要条件表达式，它检测的是调用上下文
 *   - 与 if constexpr 不同：if constexpr 基于编译期布尔表达式，
 *     if consteval 基于"当前是否在常量求值中"
 *   - consteval 分支中可以调用 consteval 函数
 *   - 也可以写 if !consteval 来取反
 * ============================================================
 */

#include <iostream>
#include <cmath>
#include <type_traits>

// ============================================================
// 特性检测
// ============================================================
#if defined(__cpp_if_consteval) && __cpp_if_consteval >= 202106L
#define HAS_IF_CONSTEVAL 1
#elif __cplusplus >= 202302L
// 某些编译器可能未定义宏但支持该语法
#define HAS_IF_CONSTEVAL 1
#else
#define HAS_IF_CONSTEVAL 0
#endif

#if HAS_IF_CONSTEVAL

// ============================================================
// 基础用法
// ============================================================
namespace basic_usage {

// 一个 consteval 函数：只能在编译期调用
consteval int compile_time_square(int x) {
    return x * x;
}

// 一个 constexpr 函数，根据是否处于编译期选择不同路径
constexpr int smart_square(int x) {
    if consteval {
        // 编译期路径：可以调用 consteval 函数
        return compile_time_square(x);
    } else {
        // 运行期路径：可以使用运行期特有的操作
        std::cout << "  [运行期计算] smart_square(" << x << ")" << std::endl;
        return x * x;
    }
}

// 使用 if !consteval 取反
constexpr double smart_sqrt(double x) {
    if !consteval {
        // 运行期路径：使用标准库
        return std::sqrt(x);
    } else {
        // 编译期路径：使用简单的牛顿迭代法
        double guess = x / 2.0;
        for (int i = 0; i < 20; ++i) {
            guess = (guess + x / guess) / 2.0;
        }
        return guess;
    }
}

void demonstrate() {
    std::cout << "[基础用法]" << std::endl;

    // 编译期调用
    constexpr int ct_result = smart_square(7);
    std::cout << "  编译期 smart_square(7) = " << ct_result << std::endl;

    // 运行期调用
    int runtime_val = 7;
    int rt_result = smart_square(runtime_val);
    std::cout << "  运行期 smart_square(7) = " << rt_result << std::endl;

    // 编译期 sqrt
    constexpr double ct_sqrt = smart_sqrt(25.0);
    std::cout << "  编译期 smart_sqrt(25.0) = " << ct_sqrt << std::endl;

    // 运行期 sqrt
    double rt_sqrt = smart_sqrt(25.0);
    std::cout << "  运行期 smart_sqrt(25.0) = " << rt_sqrt << std::endl;

    std::cout << std::endl;
}

} // namespace basic_usage

// ============================================================
// 进阶用法
// ============================================================
namespace advanced_usage {

// 场景：编译期使用安全的边界检查版本，运行期使用高效版本
constexpr int safe_divide(int a, int b) {
    if consteval {
        // 编译期：如果除以零，直接触发编译错误
        if (b == 0) {
            // 编译期调用时，抛异常会导致编译错误，起到"静态断言"的效果
            throw "编译期除零错误！";
        }
        return a / b;
    } else {
        // 运行期：返回特殊值或使用其他策略
        if (b == 0) {
            std::cerr << "  [警告] 运行期除零，返回0" << std::endl;
            return 0;
        }
        return a / b;
    }
}

// 场景：编译期使用简单算法，运行期可调用系统/库函数
consteval int ct_factorial(int n) {
    // 纯编译期的阶乘
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

constexpr int smart_factorial(int n) {
    if consteval {
        return ct_factorial(n);
    } else {
        // 运行期：可以加日志、性能计数等
        std::cout << "  [运行期] 计算 " << n << " 的阶乘..." << std::endl;
        int result = 1;
        for (int i = 2; i <= n; ++i) result *= i;
        return result;
    }
}

// 结合模板使用
template <typename T>
constexpr T clamp_positive(T value) {
    if consteval {
        // 编译期：可以做更严格的检查
        if (value < T{0}) {
            throw "编译期不允许负值！";
        }
        return value;
    } else {
        // 运行期：友好处理
        return value < T{0} ? T{0} : value;
    }
}

void demonstrate() {
    std::cout << "[进阶用法]" << std::endl;

    // 编译期除法
    constexpr int ct_div = safe_divide(100, 7);
    std::cout << "  编译期 safe_divide(100, 7) = " << ct_div << std::endl;

    // 运行期除法（包括除零）
    int rt_div = safe_divide(100, 0);
    std::cout << "  运行期 safe_divide(100, 0) = " << rt_div << std::endl;

    // 编译期阶乘
    constexpr int ct_fact = smart_factorial(10);
    std::cout << "  编译期 smart_factorial(10) = " << ct_fact << std::endl;

    // 运行期阶乘
    int n = 10;
    int rt_fact = smart_factorial(n);
    std::cout << "  运行期 smart_factorial(10) = " << rt_fact << std::endl;

    // clamp_positive
    constexpr int ct_clamp = clamp_positive(42);
    std::cout << "  编译期 clamp_positive(42) = " << ct_clamp << std::endl;

    int neg = -5;
    int rt_clamp = clamp_positive(neg);
    std::cout << "  运行期 clamp_positive(-5) = " << rt_clamp << std::endl;

    // 注意：以下代码会导致编译错误（编译期负值）
    // constexpr int bad = clamp_positive(-5); // 编译错误！

    std::cout << std::endl;
}

} // namespace advanced_usage

// ============================================================
// 易错点与陷阱
// ============================================================
namespace pitfalls {

void demonstrate() {
    std::cout << "[易错点与陷阱]" << std::endl;

    // 陷阱1：if consteval 与 if constexpr 的区别
    std::cout << "  陷阱1: if consteval vs if constexpr" << std::endl;
    std::cout << "    - if constexpr(expr): 基于编译期布尔表达式选择分支" << std::endl;
    std::cout << "    - if consteval: 基于'调用是否在常量求值上下文中'" << std::endl;
    std::cout << "    - if consteval 没有条件表达式，不用写括号" << std::endl;

    // 陷阱2：if consteval 的大括号是必须的
    std::cout << "  陷阱2: if consteval 后面必须用大括号 { }" << std::endl;
    std::cout << "    - 正确: if consteval { ... } else { ... }" << std::endl;
    std::cout << "    - 错误: if consteval return 42;  // 语法错误！" << std::endl;

    // 陷阱3：不能在 if consteval 中使用条件
    std::cout << "  陷阱3: 不能写 if consteval(some_expr)" << std::endl;
    std::cout << "    - if consteval 不接受任何条件表达式" << std::endl;

    // 陷阱4：if consteval 只能在 constexpr 或 consteval 函数中使用
    std::cout << "  陷阱4: if consteval 需在 constexpr/consteval 函数体中" << std::endl;
    std::cout << "    - 在普通函数中使用，永远走 else 分支" << std::endl;

    std::cout << std::endl;
}

} // namespace pitfalls

#endif // HAS_IF_CONSTEVAL

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== if consteval（编译期求值分支） - C++23 =====\n" << std::endl;

#if HAS_IF_CONSTEVAL
    basic_usage::demonstrate();
    advanced_usage::demonstrate();
    pitfalls::demonstrate();
#else
    std::cout << "[提示] 当前编译器不支持 if consteval 特性。" << std::endl;
    std::cout << "请使用支持 C++23 的编译器（如 GCC 14+ 或 Clang 18+）。" << std::endl;
    std::cout << std::endl;
    std::cout << "if consteval 语法示例（供参考）：" << std::endl;
    std::cout << R"(
    constexpr int smart_square(int x) {
        if consteval {
            // 编译期路径
            return compile_time_square(x);
        } else {
            // 运行期路径
            return x * x;
        }
    }

    // 也可以取反：
    constexpr double smart_sqrt(double x) {
        if !consteval {
            return std::sqrt(x);  // 运行期
        } else {
            // 编译期牛顿迭代
            ...
        }
    }
)" << std::endl;
#endif

    return 0;
}
```

## 参考链接

- [cppreference: if consteval（编译期求值分支）](https://en.cppreference.com/w/cpp/language/consteval)
