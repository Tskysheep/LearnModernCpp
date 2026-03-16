---
sidebar_position: 71
title: "std::byteswap（字节序转换）"
description: "C++23 std::byteswap（字节序转换） 教学示例"
tags: [cpp23, byteswap]
---

# std::byteswap（字节序转换）

C++23 标准引入的 std::byteswap（字节序转换） 特性完整教学示例。

## 完整源码

```cpp title="cpp23/70_byteswap.cpp"
/**
 * ============================================================
 * 特性名称: std::byteswap（字节序转换）
 * 所属标准: C++23
 * ============================================================
 *
 * [是什么]: std::byteswap 是一个函数，用于反转整数类型的字节序。
 *          例如将大端序的 0x01020304 转换为小端序的 0x04030201，
 *          或反之。
 *
 * [为什么引入]: 字节序转换在网络编程和文件格式处理中非常常见。
 *              之前需要手动位运算或使用平台特定函数（如 htonl、
 *              bswap_32）。std::byteswap 提供了类型安全、constexpr
 *              的标准方式。
 *
 * [关键要点]:
 *   - 反转任意整数类型（包括有符号整数）的字节序
 *   - 是 constexpr 和 noexcept 的
 *   - 只接受整数类型（不接受浮点、枚举等）
 *   - 定义在 <bit> 头文件中
 *   - 编译器通常会优化为单条 BSWAP 指令
 * ============================================================
 */

#include <iostream>
#include <cstdint>
#include <iomanip>
#include <array>
#include <cstring>
#include <string>
#include <type_traits>

// ============================================================
// 特性检测
// ============================================================
#ifdef __cpp_lib_byteswap
#include <bit>
#define HAS_BYTESWAP 1
#else
#define HAS_BYTESWAP 0

// 提供替代实现
namespace fallback {
    template <typename T>
    requires std::is_integral_v<T>
    constexpr T byteswap(T value) noexcept {
        if constexpr (sizeof(T) == 1) {
            return value;
        } else if constexpr (sizeof(T) == 2) {
            auto u = static_cast<uint16_t>(value);
            return static_cast<T>((u >> 8) | (u << 8));
        } else if constexpr (sizeof(T) == 4) {
            auto u = static_cast<uint32_t>(value);
            return static_cast<T>(
                ((u >> 24) & 0xFF) |
                ((u >>  8) & 0xFF00) |
                ((u <<  8) & 0xFF0000) |
                ((u << 24) & 0xFF000000)
            );
        } else if constexpr (sizeof(T) == 8) {
            auto u = static_cast<uint64_t>(value);
            return static_cast<T>(
                ((u >> 56) & 0xFF) |
                ((u >> 40) & 0xFF00) |
                ((u >> 24) & 0xFF0000) |
                ((u >>  8) & 0xFF000000) |
                ((u <<  8) & 0xFF00000000ULL) |
                ((u << 24) & 0xFF0000000000ULL) |
                ((u << 40) & 0xFF000000000000ULL) |
                ((u << 56) & 0xFF00000000000000ULL)
            );
        }
    }
}
#endif

// 统一的调用方式
template <typename T>
constexpr T do_byteswap(T value) noexcept {
#if HAS_BYTESWAP
    return std::byteswap(value);
#else
    return fallback::byteswap(value);
#endif
}

// 辅助：打印十六进制
template <typename T>
void print_hex(const char* label, T value) {
    std::cout << "  " << label << " = 0x"
              << std::hex << std::setfill('0') << std::setw(sizeof(T) * 2)
              << static_cast<uint64_t>(static_cast<std::make_unsigned_t<T>>(value))
              << std::dec << std::endl;
}

// ============================================================
// 基础用法
// ============================================================
namespace basic_usage {

void demonstrate() {
    std::cout << "[基础用法]" << std::endl;

    // 16位字节序交换
    std::cout << "  -- 16位 (uint16_t) --" << std::endl;
    uint16_t val16 = 0x0102;
    uint16_t swapped16 = do_byteswap(val16);
    print_hex("原始值  ", val16);
    print_hex("交换后  ", swapped16);

    // 32位字节序交换
    std::cout << "  -- 32位 (uint32_t) --" << std::endl;
    uint32_t val32 = 0x01020304;
    uint32_t swapped32 = do_byteswap(val32);
    print_hex("原始值  ", val32);
    print_hex("交换后  ", swapped32);

    // 64位字节序交换
    std::cout << "  -- 64位 (uint64_t) --" << std::endl;
    uint64_t val64 = 0x0102030405060708ULL;
    uint64_t swapped64 = do_byteswap(val64);
    print_hex("原始值  ", val64);
    print_hex("交换后  ", swapped64);

    // 验证：两次交换恢复原值
    std::cout << "  -- 两次交换恢复原值 --" << std::endl;
    uint32_t original = 0xDEADBEEF;
    uint32_t double_swap = do_byteswap(do_byteswap(original));
    print_hex("原始值  ", original);
    print_hex("两次交换", double_swap);
    std::cout << "  相等: " << (original == double_swap ? "是" : "否") << std::endl;

    // constexpr 使用
    std::cout << "  -- constexpr 编译期交换 --" << std::endl;
    constexpr uint32_t ct_val = 0xAABBCCDD;
    constexpr uint32_t ct_swapped = do_byteswap(ct_val);
    print_hex("编译期原始值", ct_val);
    print_hex("编译期交换后", ct_swapped);

    std::cout << std::endl;
}

} // namespace basic_usage

// ============================================================
// 进阶用法
// ============================================================
namespace advanced_usage {

// 检测当前系统的字节序
bool is_little_endian() {
    uint16_t test = 0x0001;
    uint8_t first_byte;
    std::memcpy(&first_byte, &test, 1);
    return first_byte == 0x01;
}

// 场景1：网络字节序转换
// 网络字节序 = 大端序（Big-endian）
template <typename T>
T host_to_network(T value) {
    if (is_little_endian()) {
        return do_byteswap(value);  // 小端 -> 大端
    }
    return value;  // 已经是大端
}

template <typename T>
T network_to_host(T value) {
    if (is_little_endian()) {
        return do_byteswap(value);  // 大端 -> 小端
    }
    return value;  // 已经是小端
}

// 场景2：文件格式处理（BMP 文件头示例）
#pragma pack(push, 1)
struct BMPHeader {
    uint16_t signature;   // 'BM' = 0x4D42
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t data_offset;
};
#pragma pack(pop)

// BMP 使用小端序
BMPHeader create_bmp_header(uint32_t width, uint32_t height) {
    uint32_t row_size = ((width * 3 + 3) / 4) * 4;  // 每行对齐到 4 字节
    uint32_t data_size = row_size * height;

    BMPHeader header{};
    header.signature = 0x4D42;  // 'BM'（小端序存储）
    header.file_size = sizeof(BMPHeader) + 40 + data_size;  // 简化
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.data_offset = sizeof(BMPHeader) + 40;

    // 如果当前是大端系统，需要转换为小端
    if (!is_little_endian()) {
        header.signature = do_byteswap(header.signature);
        header.file_size = do_byteswap(header.file_size);
        header.data_offset = do_byteswap(header.data_offset);
    }

    return header;
}

// 场景3：IP 地址处理
std::string ip_to_string(uint32_t ip_network_order) {
    uint32_t ip = network_to_host(ip_network_order);
    return std::to_string((ip >> 24) & 0xFF) + "." +
           std::to_string((ip >> 16) & 0xFF) + "." +
           std::to_string((ip >>  8) & 0xFF) + "." +
           std::to_string((ip >>  0) & 0xFF);
}

void demonstrate() {
    std::cout << "[进阶用法]" << std::endl;

    // 系统字节序信息
    std::cout << "  当前系统字节序: " << (is_little_endian() ? "小端 (Little-endian)" : "大端 (Big-endian)") << std::endl;

    // 场景1：网络字节序
    std::cout << "  -- 网络字节序转换 --" << std::endl;
    uint16_t port = 8080;
    uint16_t net_port = host_to_network(port);
    uint16_t host_port = network_to_host(net_port);
    std::cout << "  端口 " << port << " -> 网络序: ";
    print_hex("", net_port);
    std::cout << "  网络序 -> 主机序: " << host_port << std::endl;

    uint32_t ip = 0xC0A80001;  // 192.168.0.1
    uint32_t net_ip = host_to_network(ip);
    print_hex("  IP（主机序）", ip);
    print_hex("  IP（网络序）", net_ip);

    // 场景2：BMP 文件头
    std::cout << "  -- BMP 文件头创建 --" << std::endl;
    auto header = create_bmp_header(640, 480);
    std::cout << "  BMP 签名: 0x" << std::hex << header.signature << std::dec << std::endl;
    std::cout << "  数据偏移: " << header.data_offset << " 字节" << std::endl;

    // 场景3：IP 地址
    std::cout << "  -- IP 地址处理 --" << std::endl;
    uint32_t net_order_ip = host_to_network(uint32_t(0xC0A80164));
    std::cout << "  IP 地址: " << ip_to_string(net_order_ip) << std::endl;

    // 场景4：有符号整数也可以
    std::cout << "  -- 有符号整数 --" << std::endl;
    int32_t signed_val = -1;  // 0xFFFFFFFF
    int32_t signed_swapped = do_byteswap(signed_val);
    print_hex("有符号原始值", signed_val);
    print_hex("有符号交换后", signed_swapped);
    std::cout << "  -1 交换后仍为 -1（因为所有字节都是 0xFF）" << std::endl;

    std::cout << std::endl;
}

} // namespace advanced_usage

// ============================================================
// 易错点与陷阱
// ============================================================
namespace pitfalls {

void demonstrate() {
    std::cout << "[易错点与陷阱]" << std::endl;

    // 陷阱1：只能用于整数类型
    std::cout << "  陷阱1: byteswap 只接受整数类型" << std::endl;
    std::cout << "    - uint32_t:  可以 ✓" << std::endl;
    std::cout << "    - int16_t:   可以 ✓" << std::endl;
    std::cout << "    - float:     不可以 ✗" << std::endl;
    std::cout << "    - double:    不可以 ✗" << std::endl;
    std::cout << "    - enum:      不可以 ✗" << std::endl;
    std::cout << "    - bool:      不可以 ✗（虽然是整数类型，但大小为1）" << std::endl;

    // 陷阱2：浮点数需要通过 memcpy 间接转换
    std::cout << "  陷阱2: 浮点数字节序转换需要间接方式" << std::endl;
    float f = 3.14f;
    uint32_t f_bits;
    std::memcpy(&f_bits, &f, sizeof(f));
    uint32_t f_swapped = do_byteswap(f_bits);
    float f_result;
    std::memcpy(&f_result, &f_swapped, sizeof(f_result));
    std::cout << "    float 3.14 交换字节序后: " << f_result
              << " (无意义，仅演示)" << std::endl;

    // 陷阱3：单字节类型没有效果
    std::cout << "  陷阱3: 单字节类型（uint8_t/char）交换无效果" << std::endl;
    uint8_t byte_val = 0xAB;
    uint8_t byte_swapped = do_byteswap(byte_val);
    std::cout << "    0xAB 交换后: 0x"
              << std::hex << static_cast<int>(byte_swapped) << std::dec
              << " (不变)" << std::endl;

    // 陷阱4：不要混淆字节序转换和位反转
    std::cout << "  陷阱4: byteswap 是字节序反转，不是位反转" << std::endl;
    uint16_t val = 0x0102;
    uint16_t swapped = do_byteswap(val);
    std::cout << "    0x0102 字节序反转: 0x"
              << std::hex << std::setfill('0') << std::setw(4) << swapped
              << std::dec << std::endl;
    std::cout << "    字节序: [01, 02] -> [02, 01]" << std::endl;
    std::cout << "    不是位反转: 0000_0001 0000_0010 -> 0100_0000 1000_0000" << std::endl;

    // 陷阱5：与平台函数的对比
    std::cout << "  陷阱5: 与平台特定函数的对比" << std::endl;
    std::cout << "    htonl/ntohl:      仅32位，主机序<->网络序" << std::endl;
    std::cout << "    htons/ntohs:      仅16位，主机序<->网络序" << std::endl;
    std::cout << "    std::byteswap:    任意整数，纯字节交换" << std::endl;
    std::cout << "    注意: byteswap 不关心你的系统是大端还是小端" << std::endl;
    std::cout << "    它只是单纯地反转字节顺序" << std::endl;

    std::cout << std::endl;
}

} // namespace pitfalls

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::byteswap（字节序转换） - C++23 =====\n" << std::endl;

#if HAS_BYTESWAP
    std::cout << "[信息] 使用 std::byteswap (C++23 标准)" << std::endl;
#else
    std::cout << "[信息] std::byteswap 不可用，使用手动实现的替代版本" << std::endl;
    std::cout << "请使用支持 C++23 的编译器（如 GCC 14+ 或 Clang 18+）获得标准支持。" << std::endl;
#endif
    std::cout << std::endl;

    basic_usage::demonstrate();
    advanced_usage::demonstrate();
    pitfalls::demonstrate();

    return 0;
}
```

## 参考链接

- [cppreference: std::byteswap（字节序转换）](https://en.cppreference.com/w/cpp/types/byte)
