---
sidebar_position: 8
title: "consteval 立即函数"
description: "C++20 consteval 立即函数 教学示例"
tags: [cpp20, consteval]
---

# consteval 立即函数

C++20 标准引入的 consteval 立即函数 特性完整教学示例。

## 完整源码

```cpp title="cpp20/07_consteval.cpp"
/**
 * ============================================================
 * 特性名称: consteval 立即函数
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: consteval 关键字声明的函数是"立即函数"（immediate function），
 *          必须在编译期求值，不能在运行时调用。
 *
 * [为什么引入]: constexpr 函数虽然可以在编译期求值，
 *             但不保证一定在编译期执行。consteval 提供了更强的保证：
 *             如果不能在编译期求值，则直接报编译错误。
 *
 * [关键要点]:
 *   - consteval 函数必须在编译期求值
 *   - 如果传入运行时变量，编译错误
 *   - 可用于编译期断言、编译期计算、编译期代码生成
 *   - 与 constexpr 的关键区别：constexpr 是"可以"，consteval 是"必须"
 * ============================================================
 */

#include <iostream>
#include <array>
#include <cstdint>
#include <type_traits>

// ===================== basic_usage() =====================

// consteval 函数：必须在编译期求值
consteval int square(int n) {
    return n * n;
}

// constexpr 函数（对比）：可以编译期，也可以运行时
constexpr int cube(int n) {
    return n * n * n;
}

// consteval 阶乘
consteval long long factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// consteval 编译期字符串长度
consteval std::size_t compile_time_strlen(const char* str) {
    std::size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }
    return len;
}

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // consteval 函数必须用编译期常量调用
    constexpr int result1 = square(5);  // 编译期计算
    std::cout << "square(5) = " << result1 << "\n";

    // 可以直接用字面量
    std::cout << "square(10) = " << square(10) << "\n";

    // 对比 constexpr
    constexpr int result2 = cube(3);  // 编译期计算
    std::cout << "cube(3) = " << result2 << "\n";

    int runtime_val = 4;
    int result3 = cube(runtime_val);  // 运行时计算 —— constexpr 允许!
    std::cout << "cube(runtime_val=4) = " << result3 << "\n";

    // int bad = square(runtime_val);  // 编译错误! consteval 不允许运行时参数

    // 编译期阶乘
    std::cout << "\n编译期阶乘:\n";
    std::cout << "10! = " << factorial(10) << "\n";
    std::cout << "15! = " << factorial(15) << "\n";
    std::cout << "20! = " << factorial(20) << "\n";

    // 编译期字符串长度
    constexpr auto len = compile_time_strlen("Hello, C++20!");
    std::cout << "\n\"Hello, C++20!\" 的长度: " << len << "\n";
}

// ===================== advanced_usage() =====================

// 编译期查找表生成
consteval auto generate_squares() {
    std::array<int, 16> table{};
    for (int i = 0; i < 16; ++i) {
        table[static_cast<std::size_t>(i)] = i * i;
    }
    return table;
}

// 编译期斐波那契
consteval auto generate_fibonacci() {
    std::array<long long, 20> fib{};
    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i < 20; ++i) {
        fib[static_cast<std::size_t>(i)] =
            fib[static_cast<std::size_t>(i - 1)] +
            fib[static_cast<std::size_t>(i - 2)];
    }
    return fib;
}

// consteval 用于编译期验证
consteval int safe_divide(int a, int b) {
    if (b == 0) {
        // 如果 b 是 0，编译期就会报错
        // 因为 throw 不能在常量求值中执行
        throw "除数不能为零!";
    }
    return a / b;
}

// 编译期位操作
consteval uint32_t make_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
    return (static_cast<uint32_t>(a) << 24) |
           (static_cast<uint32_t>(r) << 16) |
           (static_cast<uint32_t>(g) << 8)  |
           static_cast<uint32_t>(b);
}

// 使用 consteval 做编译期类型检查辅助
consteval bool is_power_of_two(unsigned int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

// consteval 在模板中使用
template<int N>
consteval auto make_iota_array() {
    std::array<int, N> arr{};
    for (int i = 0; i < N; ++i) {
        arr[static_cast<std::size_t>(i)] = i;
    }
    return arr;
}

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 编译期查找表
    constexpr auto squares = generate_squares();
    std::cout << "平方表: ";
    for (int i = 0; i < 10; ++i) {
        std::cout << squares[static_cast<std::size_t>(i)] << " ";
    }
    std::cout << "...\n";

    // 编译期斐波那契
    constexpr auto fib = generate_fibonacci();
    std::cout << "斐波那契: ";
    for (int i = 0; i < 15; ++i) {
        std::cout << fib[static_cast<std::size_t>(i)] << " ";
    }
    std::cout << "...\n";

    // 编译期安全除法
    constexpr int div_result = safe_divide(100, 7);
    std::cout << "\n安全除法 100/7 = " << div_result << "\n";
    // constexpr int bad = safe_divide(100, 0);  // 编译错误! 除数为零

    // 编译期颜色值
    constexpr auto red   = make_color(255, 0, 0);
    constexpr auto green = make_color(0, 255, 0);
    constexpr auto blue  = make_color(0, 0, 255);
    std::cout << "\n颜色值 (ARGB):\n";
    std::cout << "  红: 0x" << std::hex << red << "\n";
    std::cout << "  绿: 0x" << green << "\n";
    std::cout << "  蓝: 0x" << blue << "\n";
    std::cout << std::dec;

    // 编译期二的幂检测
    static_assert(is_power_of_two(256), "256 应该是 2 的幂");
    static_assert(!is_power_of_two(100), "100 不是 2 的幂");
    std::cout << "\n256 是 2 的幂: " << std::boolalpha << is_power_of_two(256) << "\n";
    std::cout << "100 是 2 的幂: " << is_power_of_two(100) << "\n";

    // 编译期数组生成
    constexpr auto iota5 = make_iota_array<5>();
    std::cout << "\niota<5>: ";
    for (auto v : iota5) std::cout << v << " ";
    std::cout << "\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：不能传入运行时变量
    std::cout << "1. consteval 函数不能接受运行时变量:\n";
    std::cout << "   int x = 5;\n";
    std::cout << "   square(x);  // 编译错误!\n";
    std::cout << "   但 constexpr int x = 5; square(x); 是合法的\n\n";

    // 陷阱2：consteval 与 constexpr 的选择
    std::cout << "2. 选择 consteval 还是 constexpr?\n";
    std::cout << "   - 必须编译期求值 → consteval\n";
    std::cout << "   - 编译期和运行时都可能使用 → constexpr\n";
    std::cout << "   - 安全验证（如范围检查） → consteval（编译期就能发现错误）\n\n";

    // 陷阱3：consteval 函数不能获取地址
    std::cout << "3. consteval 函数不能获取地址（不能作为函数指针）:\n";
    std::cout << "   consteval int f(int n) { return n; }\n";
    std::cout << "   auto ptr = &f;  // 编译错误!\n\n";

    // 陷阱4：consteval 函数中的限制
    std::cout << "4. consteval 函数的限制与 constexpr 类似:\n";
    std::cout << "   - 不能有 asm 语句\n";
    std::cout << "   - 不能有 goto\n";
    std::cout << "   - 不能有 static/thread_local 变量\n";
    std::cout << "   - 但 C++20 放宽了很多，允许 try-catch、虚函数等\n\n";

    // 演示：consteval 与 static_assert 配合
    std::cout << "5. consteval + static_assert 是编译期验证的利器:\n";
    constexpr auto result = factorial(12);
    static_assert(result == 479001600, "12! 应该等于 479001600");
    std::cout << "   static_assert(factorial(12) == 479001600) 通过!\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== consteval 立即函数 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: consteval 立即函数](https://en.cppreference.com/w/cpp/language/consteval)
