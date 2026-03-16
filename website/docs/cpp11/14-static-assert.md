---
sidebar_position: 15
slug: 14-static-assert
title: "static_assert 编译期断言"
description: "C++11 static_assert 编译期断言 教学示例"
tags: [cpp11, static_assert]
---

# static_assert 编译期断言

C++11 标准引入的 static_assert 编译期断言 特性完整教学示例。

## 完整源码

```cpp title="cpp11/14_static_assert.cpp"
/**
 * ============================================================
 * 特性名称: static_assert 编译期断言
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: static_assert在编译期对常量表达式进行检查，如果
 *          条件为false则产生编译错误并显示指定的错误信息。
 *
 * [为什么引入]: C++03中只有运行时assert，无法在编译期捕获错误。
 *   很多错误（如类型大小假设、模板参数约束等）本应在编译期就发现，
 *   而不是等到运行时崩溃。static_assert让"尽早失败"成为可能。
 *
 * [关键要点]:
 *   - 语法: static_assert(常量表达式, "错误信息");
 *   - 条件必须是编译期可求值的常量表达式
 *   - 可以出现在命名空间、类、函数作用域中
 *   - 与type_traits结合使用威力巨大
 *   - C++17允许省略错误信息字符串
 * ============================================================
 */

#include <iostream>
#include <type_traits>
#include <cstdint>
#include <string>
#include <limits>

// ============================================================
// 基础用法
// ============================================================

// 1. 命名空间作用域的static_assert —— 平台假设检查
static_assert(sizeof(int) >= 4, "int必须至少4字节");
static_assert(sizeof(void*) == 4 || sizeof(void*) == 8, "只支持32位或64位平台");
static_assert(sizeof(char) == 1, "char必须是1字节");

// 2. 类中的static_assert
template <typename T>
class NumericContainer {
    // 只允许数值类型
    static_assert(std::is_arithmetic<T>::value,
                  "NumericContainer只能存储数值类型!");
public:
    void add(T value) {
        data_ = value;
        std::cout << "  存储了: " << data_ << std::endl;
    }
private:
    T data_;
};

// 3. 模板参数约束
template <typename T>
T safe_cast(double value) {
    static_assert(std::is_integral<T>::value,
                  "safe_cast的目标类型必须是整数类型");
    return static_cast<T>(value);
}

void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 平台信息
    std::cout << "sizeof(int) = " << sizeof(int) << std::endl;
    std::cout << "sizeof(void*) = " << sizeof(void*) << std::endl;

    // 使用受约束的模板类
    NumericContainer<int> ic;
    ic.add(42);

    NumericContainer<double> dc;
    dc.add(3.14);

    // 以下会触发static_assert编译错误:
    // NumericContainer<std::string> sc; // "只能存储数值类型!"

    // 安全类型转换
    int result = safe_cast<int>(3.14);
    std::cout << "safe_cast<int>(3.14) = " << result << std::endl;

    // 以下会触发编译错误:
    // safe_cast<std::string>(3.14); // "目标类型必须是整数类型"
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 与type_traits的高级组合
template <typename T>
class SmartBuffer {
    static_assert(!std::is_reference<T>::value,
                  "SmartBuffer不支持引用类型");
    static_assert(!std::is_void<T>::value,
                  "SmartBuffer不支持void类型");
    static_assert(std::is_default_constructible<T>::value,
                  "SmartBuffer需要T是可默认构造的");
public:
    SmartBuffer(size_t size) : size_(size) {
        std::cout << "  SmartBuffer<" << typeid(T).name()
                  << "> 大小=" << size_ << std::endl;
    }
private:
    size_t size_;
};

// 2. 检查结构体布局（用于网络协议、文件格式等）
#pragma pack(push, 1)
struct PacketHeader {
    uint8_t  version;
    uint8_t  type;
    uint16_t length;
    uint32_t sequence;
};
#pragma pack(pop)

static_assert(sizeof(PacketHeader) == 8, "PacketHeader大小必须精确为8字节");

// 3. 编译期常量的验证
constexpr int MAX_BUFFER_SIZE = 4096;
constexpr int MIN_BUFFER_SIZE = 64;
static_assert(MAX_BUFFER_SIZE > MIN_BUFFER_SIZE, "最大缓冲区必须大于最小缓冲区");
static_assert((MAX_BUFFER_SIZE & (MAX_BUFFER_SIZE - 1)) == 0, "MAX_BUFFER_SIZE必须是2的幂");

// 4. 模板中的条件约束
template <typename From, typename To>
To safe_numeric_cast(From value) {
    static_assert(std::is_arithmetic<From>::value && std::is_arithmetic<To>::value,
                  "safe_numeric_cast只能用于算术类型之间的转换");
    static_assert(sizeof(To) >= sizeof(From) || std::is_floating_point<To>::value,
                  "目标类型不能比源类型小（除非是浮点类型）");
    return static_cast<To>(value);
}

// 5. 确保枚举的底层类型大小
enum class MessageType : uint8_t {
    Request  = 0x01,
    Response = 0x02,
    Error    = 0xFF
};
static_assert(sizeof(MessageType) == 1, "MessageType必须是1字节");

void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // SmartBuffer
    SmartBuffer<int> buf1(100);
    SmartBuffer<double> buf2(50);
    // SmartBuffer<int&> buf3(10);     // 编译错误: 不支持引用类型
    // SmartBuffer<void> buf4(10);     // 编译错误: 不支持void类型

    // PacketHeader验证
    std::cout << "\nPacketHeader大小: " << sizeof(PacketHeader) << " 字节" << std::endl;

    // 安全数值转换
    double d = safe_numeric_cast<int, double>(42);
    long l = safe_numeric_cast<int, long>(100);
    std::cout << "safe_numeric_cast<int, double>(42) = " << d << std::endl;
    std::cout << "safe_numeric_cast<int, long>(100) = " << l << std::endl;
    // safe_numeric_cast<double, int>(3.14); // 可能触发编译错误（取决于sizeof）

    // 编译期常量
    std::cout << "\n编译期常量验证:" << std::endl;
    std::cout << "  MAX_BUFFER_SIZE = " << MAX_BUFFER_SIZE << std::endl;
    std::cout << "  MIN_BUFFER_SIZE = " << MIN_BUFFER_SIZE << std::endl;
    std::cout << "  MAX是2的幂: 是" << std::endl;

    // 类型特性检查
    std::cout << "\n类型特性:" << std::endl;
    std::cout << "  is_integral<int>: " << std::is_integral<int>::value << std::endl;
    std::cout << "  is_floating_point<double>: " << std::is_floating_point<double>::value << std::endl;
    std::cout << "  is_arithmetic<string>: " << std::is_arithmetic<std::string>::value << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: 条件必须是编译期常量表达式
    // int x = 42;
    // static_assert(x > 0, "x必须为正"); // 编译错误! x不是编译期常量
    constexpr int x = 42;
    static_assert(x > 0, "x必须为正"); // OK!
    std::cout << "陷阱1: static_assert的条件必须是编译期常量" << std::endl;

    // 陷阱2: 模板中的static_assert在实例化时才检查
    // template <typename T>
    // void foo() {
    //     static_assert(false, "总是失败"); // 某些编译器即使不实例化也报错
    //     static_assert(sizeof(T) == 0, "总是失败"); // 这样更安全（依赖T）
    // }
    std::cout << "\n陷阱2: 模板中的static_assert应依赖模板参数" << std::endl;
    std::cout << "  static_assert(false, ...) 可能在模板定义时就失败" << std::endl;
    std::cout << "  用 static_assert(sizeof(T)==0, ...) 确保只在实例化时检查" << std::endl;

    // 陷阱3: C++11要求必须有错误信息字符串
    // static_assert(sizeof(int) >= 4); // C++11中编译错误! C++17中OK
    std::cout << "\n陷阱3: C++11中必须提供错误信息字符串" << std::endl;
    std::cout << "  static_assert(条件, \"消息\"); // C++11" << std::endl;
    std::cout << "  static_assert(条件);          // C++17才可以" << std::endl;

    // 陷阱4: 过度使用static_assert
    std::cout << "\n陷阱4: 不要过度使用" << std::endl;
    std::cout << "  好的: 验证平台假设、模板参数约束、数据布局" << std::endl;
    std::cout << "  坏的: 检查运行时才知道的条件" << std::endl;

    // 最佳实践
    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 用static_assert验证平台假设（指针大小、类型大小等）" << std::endl;
    std::cout << "  2. 与type_traits结合约束模板参数" << std::endl;
    std::cout << "  3. 验证结构体内存布局（网络协议、序列化等）" << std::endl;
    std::cout << "  4. 错误信息要清晰、可操作" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: static_assert" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: static_assert 编译期断言](https://en.cppreference.com/w/cpp/language/static_assert)
