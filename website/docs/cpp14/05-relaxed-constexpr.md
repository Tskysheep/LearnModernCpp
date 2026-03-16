---
sidebar_position: 6
title: "放松的constexpr"
description: "C++14 放松的constexpr 教学示例"
tags: [cpp14, relaxed_constexpr]
---

# 放松的constexpr

C++14 标准引入的 放松的constexpr 特性完整教学示例。

## 完整源码

```cpp title="cpp14/05_relaxed_constexpr.cpp"
/**
 * ============================================================
 * 特性名称: 放松的constexpr
 * 所属标准: C++14
 * ============================================================
 *
 * [是什么]: C++14大幅放宽了constexpr函数的限制。C++11中constexpr函数体
 *          只能包含一条return语句，而C++14允许使用局部变量、循环、条件分支、
 *          多条语句等常规控制流，使constexpr函数更接近普通函数的写法。
 *
 * [为什么引入]: C++11的constexpr限制过于严格，许多在编译期完全可以计算的
 *              算法（如循环计算阶乘）只能用递归写法表达，既不直观也不高效。
 *              放松限制使编译期计算的代码更自然、更可维护。
 *
 * [关键要点]:
 *   - 允许局部变量声明（但不能是static或thread_local）
 *   - 允许if/else和switch语句
 *   - 允许for/while/do-while循环
 *   - 允许修改局部变量的值（即可变状态）
 *   - constexpr成员函数不再隐式为const（C++14改变）
 * ============================================================
 */

#include <iostream>
#include <array>
#include <type_traits>
#include <cstddef>

// ============================================================
// 基础用法
// ============================================================

// C++11写法：只能用递归，因为函数体只能有一条return
constexpr int factorial_cpp11(int n) {
    return n <= 1 ? 1 : n * factorial_cpp11(n - 1);
}

// C++14写法：可以使用循环和局部变量
constexpr int factorial_cpp14(int n) {
    int result = 1;          // 局部变量 —— C++11中不允许
    for (int i = 2; i <= n; ++i) {  // 循环 —— C++11中不允许
        result *= i;         // 修改局部变量 —— C++11中不允许
    }
    return result;
}

// 可以使用if/else
constexpr int abs_val(int n) {
    if (n < 0)
        return -n;
    else
        return n;
}

// 可以使用多条语句
constexpr int sum_range(int from, int to) {
    int total = 0;
    for (int i = from; i <= to; ++i) {
        total += i;
    }
    return total;
}

void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 编译期计算
    constexpr int f5_11 = factorial_cpp11(5);
    constexpr int f5_14 = factorial_cpp14(5);
    std::cout << "  C++11递归 factorial(5) = " << f5_11 << "\n";
    std::cout << "  C++14循环 factorial(5) = " << f5_14 << "\n";

    // 编译期验证
    static_assert(factorial_cpp14(0) == 1, "0! should be 1");
    static_assert(factorial_cpp14(5) == 120, "5! should be 120");
    static_assert(factorial_cpp14(10) == 3628800, "10! should be 3628800");

    // if/else
    constexpr int a = abs_val(-42);
    constexpr int b = abs_val(42);
    std::cout << "  abs_val(-42) = " << a << "\n";
    std::cout << "  abs_val(42)  = " << b << "\n";

    // 循环求和
    constexpr int s = sum_range(1, 100);
    static_assert(s == 5050, "1+2+...+100 should be 5050");
    std::cout << "  sum_range(1, 100) = " << s << "\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 1. constexpr函数中使用switch
constexpr const char* day_name(int day) {
    switch (day) {
        case 1: return "Monday";
        case 2: return "Tuesday";
        case 3: return "Wednesday";
        case 4: return "Thursday";
        case 5: return "Friday";
        case 6: return "Saturday";
        case 7: return "Sunday";
        default: return "Unknown";
    }
}

// 2. 编译期字符串处理
constexpr int string_length(const char* str) {
    int len = 0;
    while (str[len] != '\0') {
        ++len;
    }
    return len;
}

constexpr bool strings_equal(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        ++i;
    }
    return a[i] == b[i];
}

// 3. constexpr类——成员函数可以修改成员变量
class Point {
public:
    constexpr Point(double x = 0, double y = 0) : x_(x), y_(y) {}

    // C++14中constexpr成员函数不再隐式const，可以修改成员
    constexpr void set_x(double x) { x_ = x; }
    constexpr void set_y(double y) { y_ = y; }

    constexpr double x() const { return x_; }
    constexpr double y() const { return y_; }

    // 可以有复杂逻辑
    constexpr double distance_squared(const Point& other) const {
        double dx = x_ - other.x_;
        double dy = y_ - other.y_;
        return dx * dx + dy * dy;
    }

    constexpr Point& translate(double dx, double dy) {
        x_ += dx;
        y_ += dy;
        return *this;
    }

private:
    double x_, y_;
};

// 4. 编译期排序（冒泡排序演示）
template<typename T, std::size_t N>
struct ConstArray {
    T data[N];

    constexpr T& operator[](std::size_t i) { return data[i]; }
    constexpr const T& operator[](std::size_t i) const { return data[i]; }
    constexpr std::size_t size() const { return N; }
};

template<typename T, std::size_t N>
constexpr ConstArray<T, N> bubble_sort(ConstArray<T, N> arr) {
    // 完全的冒泡排序——在编译期执行！
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j + 1 < N - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                // 交换
                T temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return arr;
}

// 5. 编译期GCD
constexpr int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. switch
    constexpr auto name = day_name(3);
    std::cout << "  day_name(3) = " << name << "\n";

    // 2. 编译期字符串处理
    constexpr int len = string_length("Hello C++14");
    static_assert(len == 11, "length should be 11");
    std::cout << "  string_length(\"Hello C++14\") = " << len << "\n";

    constexpr bool eq = strings_equal("abc", "abc");
    static_assert(eq, "should be equal");
    std::cout << "  strings_equal(\"abc\", \"abc\") = " << eq << "\n";

    // 3. constexpr类操作
    constexpr Point p1(3.0, 4.0);
    constexpr Point p2(6.0, 8.0);
    constexpr double dist2 = p1.distance_squared(p2);
    static_assert(dist2 == 25.0, "distance squared should be 25");
    std::cout << "  Point(3,4)到Point(6,8)的距离平方 = " << dist2 << "\n";

    // 使用translate（修改成员变量）—— 运行时演示
    Point p3(1.0, 2.0);
    p3.translate(10.0, 20.0);    // 修改对象成员！
    p3.set_x(p3.x() + 5.0);
    std::cout << "  平移后的点 = (" << p3.x() << ", " << p3.y() << ")\n";

    // 4. 编译期排序
    constexpr ConstArray<int, 5> unsorted = {{5, 3, 1, 4, 2}};
    constexpr auto sorted = bubble_sort(unsorted);
    static_assert(sorted[0] == 1 && sorted[4] == 5, "should be sorted");
    std::cout << "  编译期排序: ";
    for (std::size_t i = 0; i < sorted.size(); ++i) {
        std::cout << sorted[i] << " ";
    }
    std::cout << "\n";

    // 5. 编译期GCD
    constexpr int g = gcd(48, 18);
    static_assert(g == 6, "GCD(48,18) should be 6");
    std::cout << "  gcd(48, 18) = " << g << "\n";
}

// ============================================================
// 易错点与陷阱
// ============================================================

// 陷阱演示
constexpr int safe_divide(int a, int b) {
    // C++14 constexpr中可以检查错误条件
    if (b == 0) {
        // 在constexpr上下文中，这会导致编译错误（而不是运行时未定义行为）
        return 0;  // 返回一个安全默认值
    }
    return a / b;
}

void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱1：constexpr函数仍然可以在运行期调用
    std::cout << "  陷阱1: constexpr不保证编译期求值\n";
    int runtime_val = 5;
    // 下面不是编译期计算，因为runtime_val不是constexpr
    int result = factorial_cpp14(runtime_val);  // 运行期执行
    std::cout << "    运行期调用 factorial(5) = " << result << "\n";
    // 要强制编译期求值，用constexpr变量接收
    constexpr int compile_time = factorial_cpp14(5);  // 一定是编译期
    std::cout << "    编译期调用 factorial(5) = " << compile_time << "\n";

    // 陷阱2：仍然不允许的操作
    std::cout << "\n  陷阱2: C++14 constexpr仍然不允许的操作\n";
    std::cout << "    - static局部变量\n";
    std::cout << "    - thread_local变量\n";
    std::cout << "    - goto语句\n";
    std::cout << "    - try/catch异常处理\n";
    std::cout << "    - 未初始化的变量\n";
    std::cout << "    - 调用非constexpr函数\n";
    std::cout << "    (这些要等到C++20才进一步放宽)\n";

    // 陷阱3：constexpr成员函数不再隐式const（C++14改变）
    std::cout << "\n  陷阱3: C++14中constexpr成员函数不再隐式const\n";
    std::cout << "    C++11: constexpr int get() { ... } 隐含const\n";
    std::cout << "    C++14: 不再隐含const，需要显式添加\n";

    // 陷阱4：编译期计算有递归深度和步骤限制
    std::cout << "\n  陷阱4: 编译器对constexpr有求值步骤限制\n";
    std::cout << "    GCC默认512步递归深度\n";
    std::cout << "    可通过-fconstexpr-depth=N调整\n";

    // 陷阱5：不要滥用——不是所有函数都需要constexpr
    std::cout << "\n  陷阱5: 不要过度使用constexpr\n";
    std::cout << "    只有真正需要编译期求值的场景才使用\n";
    std::cout << "    constexpr函数的修改可能导致ABI变化\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 放松的constexpr - C++14 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 放松的constexpr](https://en.cppreference.com/w/cpp/language/constexpr)
