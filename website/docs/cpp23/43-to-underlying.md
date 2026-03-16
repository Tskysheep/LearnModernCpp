---
sidebar_position: 44
title: "std::to_underlying（枚举转底层类型）"
description: "C++23 std::to_underlying（枚举转底层类型） 教学示例"
tags: [cpp23, to_underlying]
---

# std::to_underlying（枚举转底层类型）

C++23 标准引入的 std::to_underlying（枚举转底层类型） 特性完整教学示例。

## 完整源码

```cpp title="cpp23/43_to_underlying.cpp"
/**
 * ============================================================
 * 特性名称: std::to_underlying（枚举转底层类型）
 * 所属标准: C++23
 * ============================================================
 *
 * [是什么]: std::to_underlying 是一个工具函数，将枚举值转换为
 *          其底层整数类型的值。它是 static_cast 到枚举底层类型的
 *          类型安全替代品。
 *
 * [为什么引入]: 在 C++23 之前，将枚举转换为底层类型需要写
 *              static_cast<std::underlying_type_t<E>>(value)，
 *              冗长且容易出错。std::to_underlying 提供了简洁、
 *              可读且不易误用的替代方案。
 *
 * [关键要点]:
 *   - 等价于 static_cast<std::underlying_type_t<E>>(value)
 *   - 比 static_cast 更安全：只接受枚举类型
 *   - 是 constexpr 和 noexcept 的
 *   - 定义在 <utility> 头文件中
 *   - 适用于 enum 和 enum class
 * ============================================================
 */

#include <iostream>
#include <type_traits>
#include <cstdint>

// ============================================================
// 特性检测
// ============================================================
#ifdef __cpp_lib_to_underlying
#include <utility>
#define HAS_TO_UNDERLYING 1
#else
#define HAS_TO_UNDERLYING 0

// 提供一个简单的替代实现，用于不支持的编译器
namespace fallback {
    template <typename E>
    constexpr auto to_underlying(E e) noexcept {
        return static_cast<std::underlying_type_t<E>>(e);
    }
}
#endif

// ============================================================
// 基础用法
// ============================================================
namespace basic_usage {

// 不同底层类型的枚举
enum class Color : uint8_t {
    Red = 1,
    Green = 2,
    Blue = 3
};

enum class Permission : uint32_t {
    Read    = 0x01,
    Write   = 0x02,
    Execute = 0x04
};

// 传统的无作用域枚举
enum Status {
    OK = 0,
    Error = -1,
    Pending = 1
};

void demonstrate() {
    std::cout << "[基础用法]" << std::endl;

#if HAS_TO_UNDERLYING
    // 使用 std::to_underlying 转换
    auto red_val = std::to_underlying(Color::Red);
    auto green_val = std::to_underlying(Color::Green);
    std::cout << "  Color::Red   = " << static_cast<int>(red_val) << std::endl;
    std::cout << "  Color::Green = " << static_cast<int>(green_val) << std::endl;

    // 类型正确性
    static_assert(std::is_same_v<decltype(red_val), uint8_t>,
                  "Color 的底层类型应为 uint8_t");

    auto read_val = std::to_underlying(Permission::Read);
    auto write_val = std::to_underlying(Permission::Write);
    std::cout << "  Permission::Read  = 0x"
              << std::hex << read_val << std::dec << std::endl;
    std::cout << "  Permission::Write = 0x"
              << std::hex << write_val << std::dec << std::endl;

    static_assert(std::is_same_v<decltype(read_val), uint32_t>,
                  "Permission 的底层类型应为 uint32_t");

    // 对无作用域枚举也有效
    auto ok_val = std::to_underlying(Status::OK);
    auto err_val = std::to_underlying(Status::Error);
    std::cout << "  Status::OK    = " << ok_val << std::endl;
    std::cout << "  Status::Error = " << err_val << std::endl;
#else
    // 使用替代实现
    auto red_val = fallback::to_underlying(Color::Red);
    auto green_val = fallback::to_underlying(Color::Green);
    std::cout << "  [使用替代实现]" << std::endl;
    std::cout << "  Color::Red   = " << static_cast<int>(red_val) << std::endl;
    std::cout << "  Color::Green = " << static_cast<int>(green_val) << std::endl;

    auto read_val = fallback::to_underlying(Permission::Read);
    std::cout << "  Permission::Read = 0x"
              << std::hex << read_val << std::dec << std::endl;

    auto ok_val = fallback::to_underlying(Status::OK);
    std::cout << "  Status::OK = " << ok_val << std::endl;
#endif

    // 对比传统写法
    std::cout << "  -- 对比传统写法 --" << std::endl;
    std::cout << "  传统: static_cast<std::underlying_type_t<Color>>(Color::Red)" << std::endl;
    std::cout << "  C++23: std::to_underlying(Color::Red)" << std::endl;
    std::cout << "  更简洁、更安全、意图更明确" << std::endl;

    std::cout << std::endl;
}

} // namespace basic_usage

// ============================================================
// 进阶用法
// ============================================================
namespace advanced_usage {

// 位标志枚举
enum class FileMode : unsigned {
    None      = 0,
    Read      = 1 << 0,
    Write     = 1 << 1,
    Append    = 1 << 2,
    Binary    = 1 << 3,
    Truncate  = 1 << 4
};

// 使用 to_underlying 实现位运算操作符
#if HAS_TO_UNDERLYING
inline FileMode operator|(FileMode a, FileMode b) {
    return static_cast<FileMode>(std::to_underlying(a) | std::to_underlying(b));
}

inline FileMode operator&(FileMode a, FileMode b) {
    return static_cast<FileMode>(std::to_underlying(a) & std::to_underlying(b));
}

inline bool has_flag(FileMode mode, FileMode flag) {
    return std::to_underlying(mode & flag) != 0;
}
#else
inline FileMode operator|(FileMode a, FileMode b) {
    return static_cast<FileMode>(fallback::to_underlying(a) | fallback::to_underlying(b));
}

inline FileMode operator&(FileMode a, FileMode b) {
    return static_cast<FileMode>(fallback::to_underlying(a) & fallback::to_underlying(b));
}

inline bool has_flag(FileMode mode, FileMode flag) {
    return fallback::to_underlying(mode & flag) != 0;
}
#endif

// 序列化/反序列化场景
enum class MessageType : uint16_t {
    Heartbeat = 0x0001,
    Login     = 0x0002,
    Data      = 0x0003,
    Logout    = 0x0004
};

struct MessageHeader {
    uint16_t type;
    uint32_t length;
};

#if HAS_TO_UNDERLYING
MessageHeader create_header(MessageType type, uint32_t len) {
    return {std::to_underlying(type), len};
}
#else
MessageHeader create_header(MessageType type, uint32_t len) {
    return {fallback::to_underlying(type), len};
}
#endif

// constexpr 上下文
enum class Weekday : int {
    Monday = 1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday
};

#if HAS_TO_UNDERLYING
constexpr bool is_weekend(Weekday day) {
    return std::to_underlying(day) >= 6;
}
#else
constexpr bool is_weekend(Weekday day) {
    return fallback::to_underlying(day) >= 6;
}
#endif

void demonstrate() {
    std::cout << "[进阶用法]" << std::endl;

    // 场景1：位标志操作
    std::cout << "  -- 位标志操作 --" << std::endl;
    FileMode mode = FileMode::Read | FileMode::Write | FileMode::Binary;
    std::cout << "  文件模式: Read | Write | Binary" << std::endl;
    std::cout << "    包含 Read:   " << (has_flag(mode, FileMode::Read) ? "是" : "否") << std::endl;
    std::cout << "    包含 Write:  " << (has_flag(mode, FileMode::Write) ? "是" : "否") << std::endl;
    std::cout << "    包含 Append: " << (has_flag(mode, FileMode::Append) ? "是" : "否") << std::endl;
    std::cout << "    包含 Binary: " << (has_flag(mode, FileMode::Binary) ? "是" : "否") << std::endl;

    // 场景2：序列化
    std::cout << "  -- 序列化（网络消息头） --" << std::endl;
    auto header = create_header(MessageType::Login, 128);
    std::cout << "  消息类型: 0x" << std::hex << header.type
              << ", 长度: " << std::dec << header.length << std::endl;

    // 场景3：constexpr 使用
    std::cout << "  -- constexpr 上下文 --" << std::endl;
    constexpr bool sat_weekend = is_weekend(Weekday::Saturday);
    constexpr bool tue_weekend = is_weekend(Weekday::Tuesday);
    std::cout << "  星期六是周末: " << (sat_weekend ? "是" : "否") << std::endl;
    std::cout << "  星期二是周末: " << (tue_weekend ? "是" : "否") << std::endl;

    // 场景4：用于 switch 或查找表
    std::cout << "  -- 用作数组索引 --" << std::endl;
    const char* day_names[] = {
        "", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日"
    };
#if HAS_TO_UNDERLYING
    auto idx = std::to_underlying(Weekday::Wednesday);
#else
    auto idx = fallback::to_underlying(Weekday::Wednesday);
#endif
    std::cout << "  Weekday::Wednesday -> " << day_names[idx] << std::endl;

    std::cout << std::endl;
}

} // namespace advanced_usage

// ============================================================
// 易错点与陷阱
// ============================================================
namespace pitfalls {

void demonstrate() {
    std::cout << "[易错点与陷阱]" << std::endl;

    // 陷阱1：只能用于枚举类型
    std::cout << "  陷阱1: to_underlying 只接受枚举类型" << std::endl;
    std::cout << "    - std::to_underlying(Color::Red)  // 正确" << std::endl;
    std::cout << "    - std::to_underlying(42)           // 编译错误！int 不是枚举" << std::endl;
    std::cout << "    - 这正是它比 static_cast 更安全的原因" << std::endl;

    // 陷阱2：不检查枚举值是否合法
    std::cout << "  陷阱2: 不验证枚举值的合法性" << std::endl;
    enum class SmallEnum : uint8_t { A = 0, B = 1, C = 2 };
    SmallEnum invalid = static_cast<SmallEnum>(255);  // 不在定义范围内
#if HAS_TO_UNDERLYING
    auto val = std::to_underlying(invalid);
#else
    auto val = fallback::to_underlying(invalid);
#endif
    std::cout << "    to_underlying(非法枚举值 255) = " << static_cast<int>(val) << std::endl;
    std::cout << "    不会报错！需要自己验证枚举值的合法性" << std::endl;

    // 陷阱3：注意底层类型的大小
    std::cout << "  陷阱3: 注意底层类型差异" << std::endl;
    enum class Tiny : int8_t { X = 127 };
    enum class Big  : int64_t { Y = 1000000000LL };
#if HAS_TO_UNDERLYING
    auto tiny_val = std::to_underlying(Tiny::X);
    auto big_val = std::to_underlying(Big::Y);
#else
    auto tiny_val = fallback::to_underlying(Tiny::X);
    auto big_val = fallback::to_underlying(Big::Y);
#endif
    std::cout << "    int8_t 枚举:  sizeof = " << sizeof(tiny_val)
              << "，值 = " << static_cast<int>(tiny_val) << std::endl;
    std::cout << "    int64_t 枚举: sizeof = " << sizeof(big_val)
              << "，值 = " << big_val << std::endl;

    // 陷阱4：与强制类型转换的区别
    std::cout << "  陷阱4: to_underlying vs static_cast" << std::endl;
    std::cout << "    - to_underlying: 只能枚举 -> 底层类型" << std::endl;
    std::cout << "    - static_cast: 可以枚举 -> 任意整数类型（可能截断）" << std::endl;
    std::cout << "    - to_underlying 更安全，不会意外转换到错误类型" << std::endl;

    std::cout << std::endl;
}

} // namespace pitfalls

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::to_underlying（枚举转底层类型） - C++23 =====\n" << std::endl;

#if HAS_TO_UNDERLYING
    std::cout << "[信息] 使用 std::to_underlying (C++23 标准)" << std::endl;
#else
    std::cout << "[信息] std::to_underlying 不可用，使用替代实现" << std::endl;
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

- [cppreference: std::to_underlying（枚举转底层类型）](https://en.cppreference.com/w/cpp/utility/to_underlying)
