---
sidebar_position: 46
title: "数学常量 (std::numbers)"
description: "C++20 数学常量 (std::numbers) 教学示例"
tags: [cpp20, math_constants]
---

# 数学常量 (std::numbers)

C++20 标准引入的 数学常量 (std::numbers) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/45_math_constants.cpp"
/**
 * ============================================================
 * 特性名称: 数学常量 (std::numbers)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: std::numbers 命名空间提供了标准数学常量，
 *          如 pi、e、sqrt2、ln2 等，支持不同精度的浮点类型。
 *
 * [为什么引入]: 之前 C++ 没有标准的数学常量，
 *             程序员需要自定义或依赖非标准的 M_PI 等宏。
 *             std::numbers 提供了类型安全、精度正确的标准常量。
 *
 * [关键要点]:
 *   - 所有常量在 <numbers> 头文件中
 *   - 支持 float、double、long double 模板版本
 *   - 简写版本（如 std::numbers::pi）是 double 类型
 *   - 模板版本（如 std::numbers::pi_v<float>）指定类型
 * ============================================================
 */

#include <cmath>
#include <iostream>
#include <numbers>
#include <iomanip>

// ===================== basic_usage() =====================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 常用数学常量（double 版本）
    std::cout << std::setprecision(15);

    std::cout << "pi (圆周率):         " << std::numbers::pi << "\n";
    std::cout << "e (自然常数):         " << std::numbers::e << "\n";
    std::cout << "sqrt2 (根号2):       " << std::numbers::sqrt2 << "\n";
    std::cout << "sqrt3 (根号3):       " << std::numbers::sqrt3 << "\n";
    std::cout << "ln2 (ln(2)):         " << std::numbers::ln2 << "\n";
    std::cout << "ln10 (ln(10)):       " << std::numbers::ln10 << "\n";
    std::cout << "log2e (log2(e)):     " << std::numbers::log2e << "\n";
    std::cout << "log10e (log10(e)):   " << std::numbers::log10e << "\n";

    // 其他常量
    std::cout << "\n其他常量:\n";
    std::cout << "phi (黄金比例):       " << std::numbers::phi << "\n";
    std::cout << "inv_pi (1/pi):       " << std::numbers::inv_pi << "\n";
    std::cout << "inv_sqrtpi (1/√π):   " << std::numbers::inv_sqrtpi << "\n";
    std::cout << "egamma (欧拉-马斯刻罗尼): " << std::numbers::egamma << "\n";

    // 实际使用
    std::cout << "\n-- 实际计算示例 --\n";
    double radius = 5.0;
    double area = std::numbers::pi * radius * radius;
    double circumference = 2.0 * std::numbers::pi * radius;
    std::cout << "半径 " << radius << " 的圆:\n";
    std::cout << "  面积: " << area << "\n";
    std::cout << "  周长: " << circumference << "\n";
}

// ===================== advanced_usage() =====================
void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 不同精度的常量
    std::cout << "-- 不同浮点类型的精度 --\n";

    float pi_f = std::numbers::pi_v<float>;
    double pi_d = std::numbers::pi_v<double>;
    long double pi_ld = std::numbers::pi_v<long double>;

    std::cout << std::setprecision(20);
    std::cout << "float pi:       " << pi_f << "\n";
    std::cout << "double pi:      " << pi_d << "\n";
    std::cout << "long double pi: " << pi_ld << "\n";

    // 简写等价关系
    std::cout << "\n-- 简写与模板版本 --\n";
    std::cout << "std::numbers::pi == std::numbers::pi_v<double>: "
              << std::boolalpha << (std::numbers::pi == std::numbers::pi_v<double>) << "\n";

    // 与传统自定义常量对比
    std::cout << "\n-- 与传统方式对比 --\n";

    // 传统方式1：使用宏（非标准）
    #ifdef M_PI
    std::cout << "M_PI (非标准宏):    " << M_PI << "\n";
    #else
    std::cout << "M_PI: 未定义（该平台不提供）\n";
    #endif

    // 传统方式2：自定义常量
    constexpr double MY_PI = 3.14159265358979323846;
    std::cout << "自定义 MY_PI:       " << MY_PI << "\n";
    std::cout << "std::numbers::pi:   " << std::numbers::pi << "\n";

    // 传统方式3：运行时计算
    double computed_pi = std::acos(-1.0);
    std::cout << "acos(-1.0):         " << computed_pi << "\n";

    // 在模板中使用
    std::cout << "\n-- 在模板中使用 --\n";

    auto compute_circle_area = []<typename T>(T radius) -> T {
        return std::numbers::pi_v<T> * radius * radius;
    };

    float area_f = compute_circle_area(5.0f);
    double area_d = compute_circle_area(5.0);

    std::cout << std::setprecision(10);
    std::cout << "float 精度面积:  " << area_f << "\n";
    std::cout << "double 精度面积: " << area_d << "\n";

    // constexpr 使用
    std::cout << "\n-- constexpr 计算 --\n";
    constexpr double half_pi = std::numbers::pi / 2.0;
    constexpr double two_pi = std::numbers::pi * 2.0;
    constexpr double degree_to_rad = std::numbers::pi / 180.0;

    std::cout << "π/2 = " << half_pi << "\n";
    std::cout << "2π = " << two_pi << "\n";
    std::cout << "1° = " << degree_to_rad << " 弧度\n";
    std::cout << "45° = " << 45.0 * degree_to_rad << " 弧度\n";
    std::cout << "90° = " << 90.0 * degree_to_rad << " 弧度\n";

    // 所有可用常量一览
    std::cout << "\n-- 所有标准数学常量 --\n";
    std::cout << std::setprecision(15);
    std::cout << "e          (e):              " << std::numbers::e << "\n";
    std::cout << "log2e      (log₂(e)):        " << std::numbers::log2e << "\n";
    std::cout << "log10e     (log₁₀(e)):       " << std::numbers::log10e << "\n";
    std::cout << "pi         (π):              " << std::numbers::pi << "\n";
    std::cout << "inv_pi     (1/π):            " << std::numbers::inv_pi << "\n";
    std::cout << "inv_sqrtpi (1/√π):           " << std::numbers::inv_sqrtpi << "\n";
    std::cout << "ln2        (ln(2)):          " << std::numbers::ln2 << "\n";
    std::cout << "ln10       (ln(10)):         " << std::numbers::ln10 << "\n";
    std::cout << "sqrt2      (√2):             " << std::numbers::sqrt2 << "\n";
    std::cout << "sqrt3      (√3):             " << std::numbers::sqrt3 << "\n";
    std::cout << "inv_sqrt3  (1/√3):           " << std::numbers::inv_sqrt3 << "\n";
    std::cout << "egamma     (γ):              " << std::numbers::egamma << "\n";
    std::cout << "phi        (φ = (1+√5)/2):   " << std::numbers::phi << "\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：简写版本是 double
    std::cout << "1. 简写版本（如 pi）总是 double:\n";
    std::cout << "   auto x = std::numbers::pi;  // x 是 double\n";
    std::cout << "   如果需要 float，使用 pi_v<float>\n";
    std::cout << "   float f = std::numbers::pi;  // 隐式窄化，可能丢失精度\n\n";

    // 陷阱2：头文件
    std::cout << "2. 需要包含 <numbers> 头文件:\n";
    std::cout << "   #include <numbers>  // 必须显式包含\n";
    std::cout << "   不包含在 <cmath> 中\n\n";

    // 陷阱3：命名空间
    std::cout << "3. 常量在 std::numbers 命名空间下:\n";
    std::cout << "   std::numbers::pi      // 正确\n";
    std::cout << "   std::pi               // 错误! 不在 std 直属\n";
    std::cout << "   using namespace std::numbers;  // 可以简化\n\n";

    // 陷阱4：整数类型
    std::cout << "4. 数学常量不支持整数类型:\n";
    std::cout << "   std::numbers::pi_v<int>  // 编译错误!\n";
    std::cout << "   只支持 float, double, long double\n\n";

    // 陷阱5：精度
    std::cout << "5. 常量的精度受限于浮点类型:\n";
    std::cout << std::setprecision(20);
    std::cout << "   float pi:       " << std::numbers::pi_v<float> << "\n";
    std::cout << "   double pi:      " << std::numbers::pi_v<double> << "\n";
    std::cout << "   float 只有约7位有效数字，double 约15位\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== 数学常量 std::numbers - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 数学常量 (std::numbers)](https://en.cppreference.com/w/cpp/numeric/constants)
