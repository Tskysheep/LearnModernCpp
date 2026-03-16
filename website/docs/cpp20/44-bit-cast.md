---
sidebar_position: 45
title: "std::bit_cast"
description: "C++20 std::bit_cast 教学示例"
tags: [cpp20, bit_cast]
---

# std::bit_cast

C++20 标准引入的 std::bit_cast 特性完整教学示例。

## 完整源码

```cpp title="cpp20/44_bit_cast.cpp"
/**
 * ============================================================
 * 特性名称: std::bit_cast
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: std::bit_cast 提供了一种安全的方式将一个类型的位模式
 *          重新解释为另一个类型的值，类似于 memcpy 但是 constexpr 友好。
 *
 * [为什么引入]: reinterpret_cast 在很多情况下是未定义行为，
 *             memcpy 不能在编译期使用。
 *             bit_cast 提供了类型安全、constexpr 友好的位转换。
 *
 * [关键要点]:
 *   - 源类型和目标类型必须大小相同
 *   - 两种类型都必须是可平凡复制的（TriviallyCopyable）
 *   - 可以在 constexpr 上下文中使用
 *   - 替代了 reinterpret_cast + memcpy 的常见模式
 * ============================================================
 */

#include <bit>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <array>
#include <type_traits>

// ===================== basic_usage() =====================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. float 与 uint32_t 之间的位转换
    float f = 3.14f;
    auto bits = std::bit_cast<uint32_t>(f);

    std::cout << "float 3.14 的位模式:\n";
    std::cout << "  十六进制: 0x" << std::hex << bits << std::dec << "\n";
    std::cout << "  十进制: " << bits << "\n";

    // 反向转换
    float f_back = std::bit_cast<float>(bits);
    std::cout << "  转回 float: " << f_back << "\n";

    // 2. double 与 uint64_t
    double d = 3.141592653589793;
    auto d_bits = std::bit_cast<uint64_t>(d);
    std::cout << "\ndouble pi 的位模式:\n";
    std::cout << "  十六进制: 0x" << std::hex << d_bits << std::dec << "\n";

    double d_back = std::bit_cast<double>(d_bits);
    std::cout << "  转回 double: " << d_back << "\n";

    // 3. 特殊浮点值
    std::cout << "\n特殊浮点值的位模式:\n";
    float pos_zero = 0.0f;
    float neg_zero = -0.0f;
    float inf = std::numeric_limits<float>::infinity();
    float nan = std::numeric_limits<float>::quiet_NaN();

    std::cout << "  +0.0f: 0x" << std::hex << std::bit_cast<uint32_t>(pos_zero) << "\n";
    std::cout << "  -0.0f: 0x" << std::bit_cast<uint32_t>(neg_zero) << "\n";
    std::cout << "  +inf:  0x" << std::bit_cast<uint32_t>(inf) << "\n";
    std::cout << "  NaN:   0x" << std::bit_cast<uint32_t>(nan) << "\n";
    std::cout << std::dec;
}

// ===================== advanced_usage() =====================

// constexpr 使用 bit_cast
constexpr uint32_t float_to_bits(float f) {
    return std::bit_cast<uint32_t>(f);
}

constexpr float bits_to_float(uint32_t bits) {
    return std::bit_cast<float>(bits);
}

// 使用 bit_cast 检查浮点数属性（编译期！）
constexpr bool is_negative_zero(float f) {
    return std::bit_cast<uint32_t>(f) == 0x80000000u;
}

constexpr bool is_nan(float f) {
    uint32_t bits = std::bit_cast<uint32_t>(f);
    return (bits & 0x7F800000u) == 0x7F800000u && (bits & 0x007FFFFFu) != 0;
}

// 在结构体之间转换
struct FloatParts {
    uint32_t mantissa : 23;
    uint32_t exponent : 8;
    uint32_t sign : 1;
};

// bit_cast 与传统方式对比
void compare_methods(float value) {
    std::cout << "值: " << value << "\n";

    // 方式1：bit_cast（推荐，安全，constexpr）
    auto bits1 = std::bit_cast<uint32_t>(value);
    std::cout << "  bit_cast: 0x" << std::hex << bits1 << "\n";

    // 方式2：memcpy（安全，但不是 constexpr）
    uint32_t bits2;
    std::memcpy(&bits2, &value, sizeof(float));
    std::cout << "  memcpy:   0x" << bits2 << "\n";

    // 方式3：reinterpret_cast（未定义行为！违反严格别名规则）
    // uint32_t bits3 = *reinterpret_cast<uint32_t*>(&value);
    // 不要使用这种方式！
    std::cout << "  reinterpret_cast: 未定义行为，不推荐!\n";

    // 方式4：union type punning（C 中合法，C++ 中未定义行为）
    // union { float f; uint32_t u; } u;
    // u.f = value;
    // 不要在 C++ 中使用这种方式！
    std::cout << "  union punning:    C++ 中未定义行为，不推荐!\n";

    std::cout << std::dec;
}

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // constexpr bit_cast
    std::cout << "-- constexpr bit_cast --\n";
    constexpr auto pi_bits = float_to_bits(3.14f);
    std::cout << "编译期 float→bits: 0x" << std::hex << pi_bits << std::dec << "\n";

    constexpr auto pi_back = bits_to_float(pi_bits);
    std::cout << "编译期 bits→float: " << pi_back << "\n";

    // 编译期浮点检查
    static_assert(is_negative_zero(-0.0f), "-0.0f 应该是负零");
    static_assert(!is_negative_zero(0.0f), "0.0f 不是负零");
    std::cout << "\nstatic_assert 编译期检查通过!\n";
    std::cout << "is_negative_zero(-0.0f) = " << std::boolalpha << is_negative_zero(-0.0f) << "\n";
    std::cout << "is_nan(NaN) = " << is_nan(std::numeric_limits<float>::quiet_NaN()) << "\n";

    // 结构体位转换
    std::cout << "\n-- 浮点数位域分析 --\n";
    float pi = 3.14f;
    auto parts = std::bit_cast<FloatParts>(pi);
    std::cout << "3.14f 的 IEEE 754 表示:\n";
    std::cout << "  符号位:   " << parts.sign << " (" << (parts.sign ? "负" : "正") << ")\n";
    std::cout << "  指数:     " << parts.exponent << " (偏移后: " << (int(parts.exponent) - 127) << ")\n";
    std::cout << "  尾数:     0x" << std::hex << parts.mantissa << std::dec << "\n";

    // 与传统方式对比
    std::cout << "\n-- 与传统方式对比 --\n";
    compare_methods(1.0f);

    // 数组类型之间的 bit_cast
    std::cout << "\n-- 数组转换 --\n";
    std::array<uint8_t, 4> bytes = {0x00, 0x00, 0x80, 0x3F};  // 1.0f 的 IEEE 754 表示 (小端)
    float from_bytes = std::bit_cast<float>(bytes);
    std::cout << "字节 {0x00, 0x00, 0x80, 0x3F} → float: " << from_bytes << "\n";

    auto to_bytes = std::bit_cast<std::array<uint8_t, 4>>(1.0f);
    std::cout << "float 1.0 → 字节: ";
    for (auto b : to_bytes) {
        std::cout << "0x" << std::hex << static_cast<int>(b) << " ";
    }
    std::cout << std::dec << "\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：大小必须相同
    std::cout << "1. 源类型和目标类型大小必须完全相同:\n";
    std::cout << "   bit_cast<int>(3.14);     // 错误! double(8字节) → int(4字节)\n";
    std::cout << "   bit_cast<int>(3.14f);    // 正确! float(4字节) → int(4字节)\n";
    std::cout << "   sizeof(float)=" << sizeof(float) << ", sizeof(int)=" << sizeof(int) << "\n\n";

    // 陷阱2：必须是可平凡复制类型
    std::cout << "2. 两种类型都必须是 TriviallyCopyable:\n";
    std::cout << "   bit_cast<int>(string);  // 错误! string 不可平凡复制\n";
    std::cout << "   float, int, struct(POD) 通常都是 TriviallyCopyable\n\n";

    // 验证
    std::cout << "   float 可平凡复制: "
              << std::boolalpha << std::is_trivially_copyable_v<float> << "\n";
    std::cout << "   int 可平凡复制: "
              << std::is_trivially_copyable_v<int> << "\n";
    std::cout << "   string 可平凡复制: "
              << std::is_trivially_copyable_v<std::string> << "\n\n";

    // 陷阱3：字节序（大端/小端）
    std::cout << "3. bit_cast 不处理字节序:\n";
    std::cout << "   结果取决于平台的字节序\n";
    uint32_t val = 0x01020304;
    auto bytes = std::bit_cast<std::array<uint8_t, 4>>(val);
    std::cout << "   0x01020304 的字节序: ";
    for (auto b : bytes) std::cout << std::hex << static_cast<int>(b) << " ";
    std::cout << std::dec;
    if (bytes[0] == 0x04) std::cout << "(小端)\n";
    else std::cout << "(大端)\n";
    std::cout << "\n";

    // 陷阱4：不是类型转换
    std::cout << "4. bit_cast 不是值的转换，是位模式的重新解释:\n";
    std::cout << "   static_cast<int>(3.14f) = " << static_cast<int>(3.14f) << " (值转换)\n";
    std::cout << "   bit_cast<int>(3.14f) = " << std::bit_cast<int>(3.14f) << " (位重新解释)\n";
    std::cout << "   两者完全不同!\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== std::bit_cast - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::bit_cast](https://en.cppreference.com/w/cpp/numeric/bit_cast)
