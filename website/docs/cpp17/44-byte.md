---
sidebar_position: 45
slug: 44-byte
title: "std::byte (类型安全字节)"
description: "C++17 std::byte (类型安全字节) 教学示例"
tags: [cpp17, byte]
---

# std::byte (类型安全字节)

C++17 标准引入的 std::byte (类型安全字节) 特性完整教学示例。

## 完整源码

```cpp title="cpp17/44_byte.cpp"
/**
 * ============================================================
 * 特性名称: std::byte (类型安全字节)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: std::byte 是一个枚举类型，专门表示「原始字节」数据。
 *          它只支持位运算操作，不支持算术运算，从而在类型层面
 *          区分了「字节数据」和「数值数据」。
 *
 * [为什么引入]: C/C++ 中 char/unsigned char 既用于字符也用于字节，
 *              语义混乱。std::byte 明确表示「这是原始字节」，
 *              禁止了不合适的算术运算（如 byte + byte），
 *              让代码意图更清晰。
 *
 * [关键要点]:
 *   - std::byte 是 enum class byte : unsigned char {};
 *   - 支持位运算: |  &  ^  ~  <<  >>
 *   - 不支持算术运算: +  -  *  / （编译错误）
 *   - 与整数互转: std::to_integer<int>(b)  和  std::byte{42}
 *   - 主要用于底层数据处理、网络协议、文件 I/O 等场景
 * ============================================================
 */

#include <iostream>
#include <cstddef>   // std::byte
#include <array>
#include <vector>
#include <iomanip>
#include <cstring>

// ============================================================
// 辅助函数：打印字节
// ============================================================
void print_byte(std::byte b) {
    std::cout << "0x" << std::hex << std::setfill('0') << std::setw(2)
              << std::to_integer<int>(b) << std::dec;
}

void print_bytes(const std::byte* data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        print_byte(data[i]);
        std::cout << " ";
    }
    std::cout << "\n";
}

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 创建 byte
    std::byte b1{42};                // 从整数字面量
    std::byte b2{0xFF};              // 十六进制
    std::byte b3{0b10101010};        // 二进制

    // 2. 转回整数
    int val = std::to_integer<int>(b1);
    unsigned char uc = std::to_integer<unsigned char>(b2);
    std::cout << "b1 = " << val << "\n";
    std::cout << "b2 = " << static_cast<int>(uc) << "\n";

    // 3. 位运算
    std::byte a{0b11001100};
    std::byte b{0b10101010};

    std::byte r_or  = a | b;    // OR
    std::byte r_and = a & b;    // AND
    std::byte r_xor = a ^ b;    // XOR
    std::byte r_not = ~a;       // NOT
    std::byte r_shl = a << 2;   // 左移
    std::byte r_shr = a >> 2;   // 右移

    std::cout << "a       = "; print_byte(a); std::cout << "\n";
    std::cout << "b       = "; print_byte(b); std::cout << "\n";
    std::cout << "a | b   = "; print_byte(r_or); std::cout << "\n";
    std::cout << "a & b   = "; print_byte(r_and); std::cout << "\n";
    std::cout << "a ^ b   = "; print_byte(r_xor); std::cout << "\n";
    std::cout << "~a      = "; print_byte(r_not); std::cout << "\n";
    std::cout << "a << 2  = "; print_byte(r_shl); std::cout << "\n";
    std::cout << "a >> 2  = "; print_byte(r_shr); std::cout << "\n";

    // 4. 复合赋值运算
    std::byte c{0x0F};
    c |= std::byte{0xF0};
    std::cout << "0x0F | 0xF0 = "; print_byte(c); std::cout << "\n";

    // 5. 不能做算术运算 —— 以下都是编译错误：
    // std::byte sum = b1 + b2;     // 错误！
    // std::byte diff = b1 - b2;    // 错误！
    // std::byte prod = b1 * b2;    // 错误！
    // if (b1 < b2) { }             // 错误！没有比较运算符
    std::cout << "算术运算 (+, -, *, /) 和比较 (<, >) 都不可用\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 模拟网络数据包头
struct PacketHeader {
    std::byte version;
    std::byte type;
    std::array<std::byte, 2> length;  // 大端序 16 位长度

    uint16_t get_length() const {
        return (std::to_integer<uint16_t>(length[0]) << 8) |
                std::to_integer<uint16_t>(length[1]);
    }

    void set_length(uint16_t len) {
        length[0] = std::byte{static_cast<unsigned char>((len >> 8) & 0xFF)};
        length[1] = std::byte{static_cast<unsigned char>(len & 0xFF)};
    }
};

// 2. 位掩码操作
namespace Flags {
    constexpr std::byte READABLE   {0b00000001};
    constexpr std::byte WRITABLE   {0b00000010};
    constexpr std::byte EXECUTABLE {0b00000100};
    constexpr std::byte HIDDEN     {0b00001000};
}

bool has_flag(std::byte flags, std::byte flag) {
    return std::to_integer<int>(flags & flag) != 0;
}

std::byte set_flag(std::byte flags, std::byte flag) {
    return flags | flag;
}

std::byte clear_flag(std::byte flags, std::byte flag) {
    return flags & ~flag;
}

// 3. 字节缓冲区
class ByteBuffer {
public:
    void push(std::byte b) {
        data_.push_back(b);
    }

    void push_int(int32_t value) {
        // 大端序写入
        push(std::byte{static_cast<unsigned char>((value >> 24) & 0xFF)});
        push(std::byte{static_cast<unsigned char>((value >> 16) & 0xFF)});
        push(std::byte{static_cast<unsigned char>((value >> 8) & 0xFF)});
        push(std::byte{static_cast<unsigned char>(value & 0xFF)});
    }

    void print() const {
        print_bytes(data_.data(), data_.size());
    }

    size_t size() const { return data_.size(); }

private:
    std::vector<std::byte> data_;
};

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 数据包头
    PacketHeader header;
    header.version = std::byte{1};
    header.type = std::byte{0x42};
    header.set_length(1024);

    std::cout << "数据包版本: " << std::to_integer<int>(header.version) << "\n";
    std::cout << "数据包类型: "; print_byte(header.type); std::cout << "\n";
    std::cout << "数据包长度: " << header.get_length() << "\n";

    // 位掩码
    std::byte perms{0};
    perms = set_flag(perms, Flags::READABLE);
    perms = set_flag(perms, Flags::WRITABLE);
    perms = set_flag(perms, Flags::HIDDEN);

    std::cout << "\n文件权限: "; print_byte(perms); std::cout << "\n";
    std::cout << "可读: " << has_flag(perms, Flags::READABLE) << "\n";
    std::cout << "可写: " << has_flag(perms, Flags::WRITABLE) << "\n";
    std::cout << "可执行: " << has_flag(perms, Flags::EXECUTABLE) << "\n";
    std::cout << "隐藏: " << has_flag(perms, Flags::HIDDEN) << "\n";

    perms = clear_flag(perms, Flags::HIDDEN);
    std::cout << "清除隐藏后: "; print_byte(perms); std::cout << "\n";

    // 字节缓冲区
    ByteBuffer buf;
    buf.push_int(0x12345678);
    buf.push(std::byte{0xAB});
    std::cout << "\n缓冲区 (" << buf.size() << " 字节): ";
    buf.print();

    // 从内存拷贝
    int source = 42;
    std::array<std::byte, sizeof(int)> bytes;
    std::memcpy(bytes.data(), &source, sizeof(int));
    std::cout << "int 42 的内存表示: ";
    print_bytes(bytes.data(), bytes.size());
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: 不能从 int 隐式构造
    // std::byte b = 42;      // 编译错误！
    // std::byte b = (std::byte)42;  // C 风格转换，不推荐
    std::byte b{42};          // 正确：列表初始化
    std::cout << "陷阱1 - 必须用 std::byte{42}，不能 std::byte = 42\n";

    // 陷阱 2: 不能直接输出
    // std::cout << b;  // 编译错误！没有 << 运算符
    std::cout << "陷阱2 - 不能直接 cout，需要 to_integer: "
              << std::to_integer<int>(b) << "\n";

    // 陷阱 3: 不能与整数直接运算
    // auto result = b + 1;  // 编译错误！
    // 需要先转为整数
    auto result = std::to_integer<int>(b) + 1;
    std::cout << "陷阱3 - 需要 to_integer 后才能做算术: " << result << "\n";

    // 陷阱 4: 与 char/unsigned char 的对比
    unsigned char uc = 42;
    // uc = uc + 1;          // OK —— unsigned char 支持算术
    // uc = uc * 2;          // OK
    // uc = uc << 1;         // OK
    // 所有操作都合法，但语义混乱（字节？字符？数字？）
    std::cout << "陷阱4 - unsigned char 啥都能做（类型不安全）\n";
    std::cout << "        std::byte 只能位运算（类型安全）\n";
    (void)uc;

    // 陷阱 5: std::byte 数组和 unsigned char 数组的互操作
    unsigned char raw[] = {0x01, 0x02, 0x03};
    std::byte bytes[3];
    std::memcpy(bytes, raw, 3);  // OK: memcpy 可以
    // bytes[0] = raw[0];     // 编译错误！类型不匹配
    std::cout << "陷阱5 - byte 和 unsigned char 不能直接赋值，用 memcpy\n";

    // 陷阱 6: MinGW 的 <cstddef> 可能需要额外注意
    std::cout << "陷阱6 - sizeof(std::byte) = " << sizeof(std::byte)
              << " (和 char 一样大)\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::byte (类型安全字节) - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::byte (类型安全字节)](https://en.cppreference.com/w/cpp/types/byte)
