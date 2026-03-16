---
sidebar_position: 5
slug: 04-binary-literals-separators
title: "二进制字面量与数字分隔符"
description: "C++14 二进制字面量与数字分隔符 教学示例"
tags: [cpp14, binary_literals_separators]
---

# 二进制字面量与数字分隔符

C++14 标准引入的 二进制字面量与数字分隔符 特性完整教学示例。

## 完整源码

```cpp title="cpp14/04_binary_literals_separators.cpp"
/**
 * ============================================================
 * 特性名称: 二进制字面量与数字分隔符
 * 所属标准: C++14
 * ============================================================
 *
 * [是什么]: C++14引入了两个提升数值字面量可读性的特性：
 *          (1) 二进制字面量：使用0b或0B前缀直接书写二进制数
 *          (2) 数字分隔符：使用单引号(')作为数字间的可视分隔符
 *
 * [为什么引入]: 在嵌入式编程、位操作、网络协议等场景中，二进制表示更直观；
 *              对于大数字（如1000000），分隔符大幅提升可读性。
 *              这两个特性都只影响可读性，不改变数值的类型或值。
 *
 * [关键要点]:
 *   - 0b/0B前缀表示二进制字面量，只含0和1
 *   - 单引号分隔符可用于任何进制的数值字面量（十、十六、八、二进制）
 *   - 分隔符可以放在任意两个数字之间，不影响数值
 *   - 浮点数的整数部分和小数部分都可以使用分隔符
 * ============================================================
 */

#include <iostream>
#include <iomanip>
#include <bitset>
#include <cstdint>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // ---- 二进制字面量 ----
    std::cout << "  [二进制字面量]\n";

    // 基本的二进制数
    int a = 0b1010;         // 十进制10
    int b = 0B1100;         // 十进制12（大写B也行）
    int c = 0b11111111;     // 十进制255

    std::cout << "    0b1010     = " << a << "\n";
    std::cout << "    0B1100     = " << b << "\n";
    std::cout << "    0b11111111 = " << c << "\n";

    // 位掩码——二进制字面量特别适合
    uint8_t mask = 0b00001111;  // 低4位掩码
    uint8_t data = 0b10110110;
    uint8_t result = data & mask;

    std::cout << "    data   = " << std::bitset<8>(data) << " (" << (int)data << ")\n";
    std::cout << "    mask   = " << std::bitset<8>(mask) << " (" << (int)mask << ")\n";
    std::cout << "    result = " << std::bitset<8>(result) << " (" << (int)result << ")\n";

    // ---- 数字分隔符 ----
    std::cout << "\n  [数字分隔符]\n";

    // 十进制
    long long population = 1'400'000'000;    // 14亿
    double pi = 3.141'592'653'589'793;

    std::cout << "    1'400'000'000     = " << population << "\n";
    std::cout << std::fixed << std::setprecision(15);
    std::cout << "    3.141'592'653'... = " << pi << "\n";
    std::cout << std::defaultfloat;

    // 十六进制
    unsigned int color = 0xFF'A5'00;  // 橙色RGB
    std::cout << "    0xFF'A5'00        = 0x" << std::hex << std::uppercase
              << color << std::dec << "\n";

    // 八进制
    int permission = 0'755;  // Unix文件权限
    std::cout << "    0'755 (八进制)    = " << permission << " (十进制)\n";

    // 二进制 + 分隔符
    int byte_val = 0b1010'0011;  // 每4位分隔
    std::cout << "    0b1010'0011       = " << byte_val << "\n";
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. 位操作场景——二进制字面量让位的含义一目了然
    std::cout << "  [硬件寄存器位域]\n";

    // 假设一个8位控制寄存器
    constexpr uint8_t BIT_ENABLE  = 0b0000'0001;  // bit 0: 使能
    constexpr uint8_t BIT_MODE    = 0b0000'0110;  // bit 1-2: 模式
    constexpr uint8_t BIT_IRQ     = 0b0000'1000;  // bit 3: 中断
    constexpr uint8_t BIT_STATUS  = 0b1111'0000;  // bit 4-7: 状态

    uint8_t reg = 0b0000'0000;
    reg |= BIT_ENABLE;          // 设置使能位
    reg |= 0b0000'0100;         // 设置模式为2
    reg |= BIT_IRQ;             // 使能中断

    std::cout << "    寄存器值 = " << std::bitset<8>(reg) << "\n";
    std::cout << "    使能?    = " << ((reg & BIT_ENABLE) ? "是" : "否") << "\n";
    std::cout << "    模式     = " << ((reg & BIT_MODE) >> 1) << "\n";
    std::cout << "    中断?    = " << ((reg & BIT_IRQ) ? "是" : "否") << "\n";

    // 2. 各种进制对比同一个数
    std::cout << "\n  [同一个数的不同表示]\n";
    int dec = 255;
    int hex = 0xFF;
    int oct = 0377;
    int bin = 0b1111'1111;

    std::cout << "    十进制 255          = " << dec << "\n";
    std::cout << "    十六进制 0xFF       = " << hex << "\n";
    std::cout << "    八进制 0377         = " << oct << "\n";
    std::cout << "    二进制 0b1111'1111  = " << bin << "\n";
    std::cout << "    全部相等? " << (dec == hex && hex == oct && oct == bin ? "是" : "否") << "\n";

    // 3. 大数值的可读性对比
    std::cout << "\n  [大数值可读性对比]\n";
    // 不用分隔符
    long long without = 9999999999;
    // 使用分隔符
    long long with_sep = 9'999'999'999;
    std::cout << "    无分隔符: 9999999999   -> " << without << "\n";
    std::cout << "    有分隔符: 9'999'999'999 -> " << with_sep << "\n";

    // 4. 浮点数也可以用分隔符
    double avogadro = 6.022'140'76e23;
    double planck   = 6.626'070'15e-34;
    std::cout << "\n  [科学常数]\n";
    std::cout << "    阿伏伽德罗常数 = " << avogadro << "\n";
    std::cout << "    普朗克常数     = " << planck << "\n";

    // 5. 类型后缀与分隔符结合
    auto big_unsigned = 4'294'967'295u;     // unsigned
    auto big_long = 1'000'000'000'000LL;    // long long
    auto hex_val = 0xDEAD'BEEFu;            // unsigned hex
    std::cout << "\n  [类型后缀]\n";
    std::cout << "    4'294'967'295u    = " << big_unsigned << "\n";
    std::cout << "    1'000'000'000'000 = " << big_long << "\n";
    std::cout << "    0xDEAD'BEEFu      = " << hex_val << "\n";
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱1：分隔符不能放在开头、结尾或相邻
    std::cout << "  陷阱1: 分隔符位置规则\n";
    std::cout << "    int a = 1'000;    // OK\n";
    std::cout << "    int b = '1000;    // 错误! 不能放在开头\n";
    std::cout << "    int c = 1000';    // 错误! 不能放在结尾\n";
    std::cout << "    int d = 1''000;   // 错误! 不能连续使用\n";
    std::cout << "    int e = 0'b1010;  // 错误! 不能放在进制前缀中间\n";

    // 陷阱2：分隔符位置随意，不一定要按3位或4位分组
    int odd_sep = 1'0'0'0;  // 合法，但不推荐
    std::cout << "\n  陷阱2: 分隔符位置任意，但建议遵循惯例\n";
    std::cout << "    1'0'0'0 = " << odd_sep << " (合法但可读性差)\n";
    std::cout << "    推荐: 十进制每3位, 二进制每4或8位, 十六进制每2或4位\n";

    // 陷阱3：二进制字面量只能包含0和1
    std::cout << "\n  陷阱3: 二进制字面量只能含0和1\n";
    std::cout << "    0b1010 = " << 0b1010 << " (OK)\n";
    std::cout << "    0b1234 // 编译错误! 2,3,4不是合法的二进制数字\n";

    // 陷阱4：二进制字面量的类型推导
    std::cout << "\n  陷阱4: 二进制字面量的类型\n";
    auto small = 0b1;                          // int
    auto big   = 0b1111'1111'1111'1111'1111'1111'1111'1111;  // int或unsigned
    std::cout << "    0b1 的大小: " << sizeof(small) << " 字节\n";
    std::cout << "    32个1 的大小: " << sizeof(big) << " 字节\n";
    std::cout << "    类型规则与十进制/十六进制相同，由值的大小决定\n";

    // 陷阱5：分隔符不影响数值——纯粹是可视化辅助
    std::cout << "\n  陷阱5: 分隔符纯粹是视觉辅助，不改变任何语义\n";
    int x = 1000;
    int y = 1'000;
    std::cout << "    1000 == 1'000 ? " << (x == y ? "是" : "否") << "\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 二进制字面量与数字分隔符 - C++14 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 二进制字面量与数字分隔符](https://en.cppreference.com/w/cpp/language/integer_literal)
