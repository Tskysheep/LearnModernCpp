/**
 * ============================================================
 * 特性名称: 变量模板
 * 所属标准: C++14
 * ============================================================
 *
 * [是什么]: C++14允许使用template关键字声明变量模板，使变量本身可以参数化。
 *          这是对函数模板和类模板的自然扩展——现在变量也可以有模板参数。
 *
 * [为什么引入]: 在C++11中，要实现参数化常量需要借助constexpr函数或类的静态
 *              成员，语法繁琐。变量模板直接支持"参数化变量"的语义，使pi<double>
 *              这样的写法成为可能。标准库中的_v后缀类型特征就是变量模板的应用。
 *
 * [关键要点]:
 *   - 变量模板可以有默认模板参数
 *   - 可以对变量模板进行特化（全特化和偏特化）
 *   - 常与constexpr结合用于编译期常量
 *   - 标准库中is_same_v<T,U>等_v后缀就是变量模板的典型应用
 * ============================================================
 */

#include <iostream>
#include <string>
#include <type_traits>
#include <cmath>

// ============================================================
// 基础用法
// ============================================================

// 1. 最经典的变量模板：数学常量pi
template<typename T>
constexpr T pi = T(3.14159265358979323846L);

// 2. 另一个数学常量
template<typename T>
constexpr T e = T(2.71828182845904523536L);

// 3. 简单的变量模板（非constexpr也行，但常量场景最常用）
template<typename T>
T default_value = T{};  // 各类型的默认值

void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 不同精度的pi
    std::cout << "  pi<float>       = " << pi<float> << "\n";
    std::cout << "  pi<double>      = " << pi<double> << "\n";
    std::cout << "  pi<long double> = " << pi<long double> << "\n";

    // 利用pi进行计算
    double radius = 5.0;
    double area = pi<double> * radius * radius;
    std::cout << "  圆面积(r=5): " << area << "\n";

    // e常量
    std::cout << "  e<double>       = " << e<double> << "\n";

    // 默认值
    std::cout << "  default_value<int>    = " << default_value<int> << "\n";
    std::cout << "  default_value<double> = " << default_value<double> << "\n";
    std::cout << "  default_value<string> = \"" << default_value<std::string> << "\"\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 与constexpr函数的对比
// C++11方式：用constexpr函数包装
template<typename T>
constexpr T pi_func() { return T(3.14159265358979323846L); }

// C++14方式：变量模板——更直观
// （pi<T>已在上面定义）

// 2. 变量模板的特化
template<typename T>
constexpr T max_value = T{};   // 默认

template<>
constexpr int max_value<int> = 2147483647;

template<>
constexpr double max_value<double> = 1.7976931348623157e+308;

template<>
constexpr char max_value<char> = 127;

// 3. 模拟标准库的_v后缀类型特征
// 标准库直到C++17才正式提供 is_same_v 等，但C++14变量模板可以自行实现
template<typename T, typename U>
constexpr bool is_same_v = std::is_same<T, U>::value;

template<typename T>
constexpr bool is_integral_v = std::is_integral<T>::value;

template<typename T>
constexpr bool is_floating_point_v = std::is_floating_point<T>::value;

// 4. 带默认模板参数的变量模板
template<typename T = double>
constexpr T golden_ratio = T(1.6180339887498948482L);

// 5. 用变量模板简化模板元编程
template<unsigned N>
constexpr unsigned factorial_v = N * factorial_v<N - 1>;

template<>
constexpr unsigned factorial_v<0> = 1;

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. 对比：函数调用 vs 变量模板
    std::cout << "  constexpr函数: pi_func<double>() = " << pi_func<double>() << "\n";
    std::cout << "  变量模板:      pi<double>         = " << pi<double> << "\n";
    std::cout << "  变量模板语法更简洁自然\n";

    // 2. 特化
    std::cout << "  max_value<int>    = " << max_value<int> << "\n";
    std::cout << "  max_value<double> = " << max_value<double> << "\n";
    std::cout << "  max_value<char>   = " << (int)max_value<char> << "\n";

    // 3. 类型特征_v简化
    std::cout << "  is_same_v<int, int>         = " << is_same_v<int, int> << "\n";
    std::cout << "  is_same_v<int, double>      = " << is_same_v<int, double> << "\n";
    std::cout << "  is_integral_v<int>          = " << is_integral_v<int> << "\n";
    std::cout << "  is_floating_point_v<double> = " << is_floating_point_v<double> << "\n";

    // 对比C++11的写法
    std::cout << "  C++11: std::is_same<int,int>::value = "
              << std::is_same<int, int>::value << "\n";
    std::cout << "  C++14: is_same_v<int,int>           = "
              << is_same_v<int, int> << " (更简洁!)\n";

    // 4. 默认模板参数
    std::cout << "  golden_ratio<>       = " << golden_ratio<> << "\n";
    std::cout << "  golden_ratio<float>  = " << golden_ratio<float> << "\n";

    // 5. 编译期阶乘
    std::cout << "  factorial_v<0>  = " << factorial_v<0> << "\n";
    std::cout << "  factorial_v<5>  = " << factorial_v<5> << "\n";
    std::cout << "  factorial_v<10> = " << factorial_v<10> << "\n";

    // 编译期验证
    static_assert(factorial_v<5> == 120, "5! should be 120");
    static_assert(factorial_v<0> == 1, "0! should be 1");
}

// ============================================================
// 易错点与陷阱
// ============================================================

// 用于演示陷阱的变量模板
template<typename T>
T mutable_var = T(42);  // 非const！每个翻译单元独立

void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱1：非const变量模板的每个实例化是独立的变量
    std::cout << "  陷阱1: 非const变量模板的实例是独立变量\n";
    mutable_var<int> = 100;
    mutable_var<double> = 200.0;
    std::cout << "    mutable_var<int>    = " << mutable_var<int> << "\n";
    std::cout << "    mutable_var<double> = " << mutable_var<double> << "\n";
    // 修改int版本不影响double版本，它们是完全独立的变量

    // 陷阱2：变量模板不是类型，不能用于类型参数
    std::cout << "  陷阱2: 变量模板是值，不是类型\n";
    std::cout << "    pi<double>的类型是: const double\n";
    // template<typename T> constexpr T pi 的每个实例化都是一个独立的变量

    // 陷阱3：ODR（单一定义规则）注意事项
    std::cout << "  陷阱3: 非内联非constexpr变量模板在多个翻译单元中可能有多个副本\n";
    std::cout << "    建议：变量模板尽量使用constexpr，确保编译期常量语义\n";

    // 陷阱4：变量模板的特化必须在使用之前声明
    std::cout << "  陷阱4: 特化必须在首次使用之前可见，否则使用通用版本\n";

    // 陷阱5：不要在头文件中声明非const的变量模板
    std::cout << "  陷阱5: 头文件中的非const变量模板可能导致链接问题\n";
    std::cout << "    建议：头文件中只使用constexpr变量模板\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 变量模板 - C++14 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
