---
sidebar_position: 22
title: "constexpr 函数和变量"
description: "C++11 constexpr 函数和变量 教学示例"
tags: [cpp11, constexpr]
---

# constexpr 函数和变量

C++11 标准引入的 constexpr 函数和变量 特性完整教学示例。

## 完整源码

```cpp title="cpp11/21_constexpr.cpp"
/**
 * ============================================================
 * 特性名称: constexpr 函数和变量
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: constexpr声明函数或变量可在编译期求值。
 *          constexpr函数在参数为编译期常量时在编译期执行，
 *          在参数为运行时值时退化为普通函数。
 *
 * [为什么引入]: C++03中编译期计算只能通过模板元编程（TMP）实现，
 *   代码晦涩难读。constexpr让编译期计算像普通函数一样直观。
 *   同时它比const更强——const只保证不可修改，constexpr保证
 *   编译期可求值。
 *
 * [关键要点]:
 *   - C++11的constexpr函数限制严格：只能有一个return语句
 *   - constexpr变量必须用编译期常量初始化
 *   - constexpr函数可以在编译期和运行时两种上下文中使用
 *   - constexpr隐含const（对于变量）
 *   - C++14/17大幅放宽了constexpr的限制
 * ============================================================
 */

#include <iostream>
#include <array>
#include <cstddef>
#include <type_traits>

// ============================================================
// 基础用法
// ============================================================

// 1. constexpr变量 —— 编译期常量
constexpr int MAX_SIZE = 100;
constexpr double PI = 3.14159265358979;
constexpr int SQUARE_OF_TEN = 10 * 10;

// 2. constexpr函数（C++11限制：基本上只能有一个return）
constexpr int square(int x) {
    return x * x;
}

constexpr int factorial(int n) {
    // C++11中只能用三元运算符实现递归（不能用if-else）
    return n <= 1 ? 1 : n * factorial(n - 1);
}

constexpr double circle_area(double r) {
    return PI * r * r;
}

// 3. constexpr用作数组大小
constexpr int ARRAY_SIZE = square(5); // 25，编译期计算

void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. constexpr变量
    std::cout << "constexpr变量:" << std::endl;
    std::cout << "  MAX_SIZE = " << MAX_SIZE << std::endl;
    std::cout << "  PI = " << PI << std::endl;
    std::cout << "  SQUARE_OF_TEN = " << SQUARE_OF_TEN << std::endl;

    // 2. constexpr函数在编译期求值
    constexpr int s = square(7);  // 编译期计算
    constexpr int f = factorial(10); // 编译期计算
    constexpr double a = circle_area(5.0); // 编译期计算

    std::cout << "\nconstexpr函数(编译期求值):" << std::endl;
    std::cout << "  square(7) = " << s << std::endl;
    std::cout << "  factorial(10) = " << f << std::endl;
    std::cout << "  circle_area(5.0) = " << a << std::endl;

    // 3. constexpr函数也可以在运行时调用
    int x;
    std::cout << "\n请想象输入一个整数(这里用7): ";
    x = 7; // 模拟运行时输入
    std::cout << x << std::endl;
    int result = square(x); // 运行时调用（x不是编译期常量）
    std::cout << "  square(" << x << ") = " << result << " (运行时计算)" << std::endl;

    // 4. 用constexpr作为数组大小
    int arr[ARRAY_SIZE]; // OK! ARRAY_SIZE是编译期常量
    std::cout << "\n数组大小 ARRAY_SIZE = " << ARRAY_SIZE << std::endl;

    // 5. 与std::array配合
    std::array<int, factorial(5)> big_array; // 120个元素!
    std::cout << "std::array大小 = " << big_array.size() << std::endl;

    (void)arr;
}

// ============================================================
// 进阶用法
// ============================================================

// 1. constexpr递归 —— 编译期斐波那契数列
constexpr long long fibonacci(int n) {
    return n <= 1 ? n : fibonacci(n - 1) + fibonacci(n - 2);
}

// 2. constexpr字符串操作（C++11中很受限）
constexpr int str_length(const char* str) {
    return *str ? 1 + str_length(str + 1) : 0;
}

constexpr bool str_equal(const char* a, const char* b) {
    return (*a == *b) ? (*a == '\0' ? true : str_equal(a + 1, b + 1)) : false;
}

// 3. constexpr类（字面值类型）
class ConstPoint {
public:
    constexpr ConstPoint(double x = 0, double y = 0) : x_(x), y_(y) {}
    constexpr double x() const { return x_; }
    constexpr double y() const { return y_; }
    constexpr double distance_sq() const { return x_ * x_ + y_ * y_; }

private:
    double x_;
    double y_;
};

// 4. constexpr与模板元编程对比
// 模板元编程（TMP）方式计算阶乘
template <int N>
struct FactorialTMP {
    static constexpr int value = N * FactorialTMP<N - 1>::value;
};

template <>
struct FactorialTMP<0> {
    static constexpr int value = 1;
};

// 5. constexpr生成查找表
constexpr int power_of_2(int n) {
    return n == 0 ? 1 : 2 * power_of_2(n - 1);
}

void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 编译期斐波那契
    constexpr long long fib20 = fibonacci(20);
    std::cout << "fibonacci(20) = " << fib20 << " (编译期计算)" << std::endl;

    // 2. 编译期字符串操作
    constexpr int len = str_length("Hello, World!");
    constexpr bool eq = str_equal("abc", "abc");
    constexpr bool neq = str_equal("abc", "def");

    std::cout << "\n编译期字符串:" << std::endl;
    std::cout << "  str_length(\"Hello, World!\") = " << len << std::endl;
    std::cout << "  str_equal(\"abc\", \"abc\") = " << eq << std::endl;
    std::cout << "  str_equal(\"abc\", \"def\") = " << neq << std::endl;

    // 3. constexpr类
    constexpr ConstPoint p1(3.0, 4.0);
    constexpr double dist_sq = p1.distance_sq();

    std::cout << "\nconstexpr类:" << std::endl;
    std::cout << "  点(" << p1.x() << ", " << p1.y() << ")" << std::endl;
    std::cout << "  距离平方 = " << dist_sq << " (编译期计算)" << std::endl;

    // 用constexpr结果作为模板参数
    std::array<int, static_cast<int>(dist_sq)> arr; // 25个元素
    std::cout << "  array大小 = " << arr.size() << std::endl;

    // 4. TMP vs constexpr对比
    std::cout << "\nTMP vs constexpr:" << std::endl;
    std::cout << "  TMP: FactorialTMP<10>::value = " << FactorialTMP<10>::value << std::endl;
    std::cout << "  constexpr: factorial(10) = " << factorial(10) << std::endl;
    std::cout << "  constexpr版本更直观易读!" << std::endl;

    // 5. 编译期查找表
    std::cout << "\n编译期生成的2的幂表:" << std::endl;
    constexpr int pow2_0 = power_of_2(0);
    constexpr int pow2_4 = power_of_2(4);
    constexpr int pow2_8 = power_of_2(8);
    constexpr int pow2_10 = power_of_2(10);
    std::cout << "  2^0=" << pow2_0 << ", 2^4=" << pow2_4
              << ", 2^8=" << pow2_8 << ", 2^10=" << pow2_10 << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: C++11中constexpr函数的严格限制
    std::cout << "陷阱1: C++11中constexpr函数只能有一个return语句" << std::endl;
    std::cout << "  不能有: 局部变量、if-else、循环、多条语句" << std::endl;
    std::cout << "  只能用: 三元运算符、递归" << std::endl;
    std::cout << "  C++14放宽了这些限制" << std::endl;

    // 陷阱2: constexpr不保证编译期求值
    int runtime_val = 5;
    int r = square(runtime_val); // 运行时调用! 虽然square是constexpr
    std::cout << "\n陷阱2: constexpr函数不保证编译期求值" << std::endl;
    std::cout << "  只有当结果用于需要编译期常量的上下文时才保证" << std::endl;
    std::cout << "  如: constexpr变量初始化、数组大小、模板参数等" << std::endl;
    (void)r;

    // 陷阱3: const vs constexpr
    const int a = 42;           // 编译期常量(用字面量初始化)
    int x = 10;
    const int b = x;            // 运行时常量(用变量初始化)
    constexpr int c = 42;       // 一定是编译期常量
    // constexpr int d = x;     // 编译错误! x不是编译期常量

    std::cout << "\n陷阱3: const vs constexpr" << std::endl;
    std::cout << "  const int a = 42;   // 可能是编译期，也可能不是" << std::endl;
    std::cout << "  constexpr int c = 42; // 保证是编译期常量" << std::endl;
    (void)a; (void)b; (void)c;

    // 陷阱4: constexpr对象必须是字面值类型(literal type)
    // 字面值类型: 算术类型、指针、引用、字面值类(trivial析构函数+constexpr构造)
    // std::string不是字面值类型（C++11中）
    // constexpr std::string s = "hello"; // C++11编译错误!
    std::cout << "\n陷阱4: constexpr变量必须是字面值类型" << std::endl;
    std::cout << "  int, double, 指针, 自定义字面值类: OK" << std::endl;
    std::cout << "  std::string, std::vector: 不行(C++11/14)" << std::endl;
    std::cout << "  C++20放宽到支持std::string和std::vector" << std::endl;

    // 陷阱5: 递归深度
    std::cout << "\n陷阱5: constexpr递归有编译器限制的最大深度" << std::endl;
    std::cout << "  通常512步(可用编译器选项调整)" << std::endl;
    // constexpr auto big = fibonacci(50); // 可能超时或超过递归限制

    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 能用constexpr的常量尽量用constexpr而非const" << std::endl;
    std::cout << "  2. 简单的数学计算函数标记为constexpr" << std::endl;
    std::cout << "  3. 需要保证编译期求值时，将结果存入constexpr变量" << std::endl;
    std::cout << "  4. C++14/17/20逐步放宽限制，尽可能使用更新的标准" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: constexpr" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: constexpr 函数和变量](https://en.cppreference.com/w/cpp/language/constexpr)
