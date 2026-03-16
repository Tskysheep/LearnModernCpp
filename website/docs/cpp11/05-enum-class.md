---
sidebar_position: 6
slug: 05-enum-class
title: "强类型枚举 (enum class / scoped enum)"
description: "C++11 强类型枚举 (enum class / scoped enum) 教学示例"
tags: [cpp11, enum_class]
---

# 强类型枚举 (enum class / scoped enum)

C++11 标准引入的 强类型枚举 (enum class / scoped enum) 特性完整教学示例。

## 完整源码

```cpp title="cpp11/05_enum_class.cpp"
/**
 * ============================================================
 * 特性名称: 强类型枚举 (enum class / scoped enum)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: enum class（也称为scoped enum）是C++11引入的强类型
 *          枚举，枚举值具有作用域限制且不会隐式转换为整数。
 *
 * [为什么引入]: C++03的传统enum存在多个问题：
 *   1. 枚举值暴露在外层作用域中，容易命名冲突
 *   2. 隐式转换为整数，破坏类型安全
 *   3. 底层类型由编译器决定，不可移植
 *
 * [关键要点]:
 *   - 枚举值不会泄漏到外层作用域，必须用 EnumName::Value 访问
 *   - 不会隐式转换为整数，需要 static_cast 显式转换
 *   - 可以指定底层类型（如 enum class Color : uint8_t）
 *   - 支持前向声明
 * ============================================================
 */

#include <iostream>
#include <string>
#include <cstdint>

// ============================================================
// 传统enum的问题演示
// ============================================================
// 传统枚举: 枚举值在外层作用域
enum OldColor { RED, GREEN, BLUE };
// enum TrafficLight { RED, YELLOW, GREEN }; // 编译错误! RED和GREEN重定义!

// ============================================================
// C++11 强类型枚举
// ============================================================
enum class Color { Red, Green, Blue };
enum class TrafficLight { Red, Yellow, Green }; // 没问题! 不同作用域

// 指定底层类型
enum class Weekday : uint8_t {
    Monday = 1,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

// 指定底层类型为更大的整数
enum class ErrorCode : uint32_t {
    Success    = 0,
    NotFound   = 404,
    Forbidden  = 403,
    Internal   = 500
};

// 前向声明（需要指定底层类型，或使用enum class默认为int）
enum class Direction : int;  // 前向声明

// 定义
enum class Direction : int {
    Up,
    Down,
    Left,
    Right
};

// ============================================================
// 辅助函数
// ============================================================
std::string color_to_string(Color c) {
    switch (c) {
        case Color::Red:   return "红色";
        case Color::Green: return "绿色";
        case Color::Blue:  return "蓝色";
        default:           return "未知";
    }
}

std::string weekday_to_string(Weekday day) {
    switch (day) {
        case Weekday::Monday:    return "星期一";
        case Weekday::Tuesday:   return "星期二";
        case Weekday::Wednesday: return "星期三";
        case Weekday::Thursday:  return "星期四";
        case Weekday::Friday:    return "星期五";
        case Weekday::Saturday:  return "星期六";
        case Weekday::Sunday:    return "星期日";
        default:                 return "未知";
    }
}

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. 使用enum class
    Color c1 = Color::Red;
    Color c2 = Color::Blue;
    TrafficLight t1 = TrafficLight::Red; // 与Color::Red不冲突

    std::cout << "颜色: " << color_to_string(c1) << std::endl;
    std::cout << "另一颜色: " << color_to_string(c2) << std::endl;

    // 2. 比较操作
    if (c1 == Color::Red) {
        std::cout << "c1确实是红色" << std::endl;
    }
    if (c1 != c2) {
        std::cout << "c1和c2不同" << std::endl;
    }
    // 注意: Color::Red != TrafficLight::Red，它们是不同类型
    // if (c1 == t1) {} // 编译错误! 不能比较不同的enum class

    // 3. 指定底层类型的枚举
    Weekday today = Weekday::Wednesday;
    std::cout << "今天是: " << weekday_to_string(today) << std::endl;
    std::cout << "底层数值: " << static_cast<int>(today) << std::endl;

    // 4. 传统enum对比 —— 隐式转换的问题
    OldColor old_c = RED; // 不需要前缀
    int n = old_c;        // 隐式转换为int（传统enum允许!）
    std::cout << "\n传统enum可以隐式转为int: " << n << std::endl;

    // enum class不允许隐式转换
    // int m = c1;       // 编译错误!
    int m = static_cast<int>(c1); // 必须显式转换
    std::cout << "enum class需要显式转换: " << m << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 使用ErrorCode
    ErrorCode err = ErrorCode::NotFound;
    std::cout << "错误码: " << static_cast<uint32_t>(err) << std::endl;

    switch (err) {
        case ErrorCode::Success:
            std::cout << "成功" << std::endl; break;
        case ErrorCode::NotFound:
            std::cout << "未找到 (404)" << std::endl; break;
        case ErrorCode::Forbidden:
            std::cout << "禁止访问 (403)" << std::endl; break;
        case ErrorCode::Internal:
            std::cout << "内部错误 (500)" << std::endl; break;
    }

    // 2. 使用前向声明的类型
    Direction dir = Direction::Up;
    std::cout << "方向值: " << static_cast<int>(dir) << std::endl;

    // 3. enum class 的大小取决于底层类型
    std::cout << "\n各枚举类型的大小:" << std::endl;
    std::cout << "  sizeof(Color)       = " << sizeof(Color)
              << " (默认int)" << std::endl;
    std::cout << "  sizeof(Weekday)     = " << sizeof(Weekday)
              << " (uint8_t)" << std::endl;
    std::cout << "  sizeof(ErrorCode)   = " << sizeof(ErrorCode)
              << " (uint32_t)" << std::endl;

    // 4. 将整数转回enum class（需要显式转换）
    int raw_value = 2;
    Color from_int = static_cast<Color>(raw_value);
    std::cout << "从整数2转换: " << color_to_string(from_int) << std::endl;

    // 5. 利用enum class实现类型安全的标志位
    enum class Permission : unsigned {
        None    = 0,
        Read    = 1 << 0,  // 1
        Write   = 1 << 1,  // 2
        Execute = 1 << 2   // 4
    };

    // 需要重载运算符才能使用位操作（这正是类型安全的体现）
    unsigned perms = static_cast<unsigned>(Permission::Read)
                   | static_cast<unsigned>(Permission::Write);
    std::cout << "权限组合(Read|Write): " << perms << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: 无效的枚举值
    // static_cast可以把任意整数转为enum class，即使值不在枚举定义中
    Color invalid = static_cast<Color>(999);
    std::cout << "无效枚举值: " << color_to_string(invalid) << std::endl;
    std::cout << "警告: static_cast不检查值是否有效!" << std::endl;

    // 陷阱2: enum class不能直接用于位运算
    // Permission p = Permission::Read | Permission::Write; // 编译错误!
    // 需要重载运算符或使用传统enum配合命名空间
    std::cout << "\nenum class不支持直接位运算，需要重载运算符" << std::endl;

    // 陷阱3: switch中忘记处理所有枚举值
    // 好的编译器会在-Wall下对不完整的switch给出警告
    Color c = Color::Green;
    switch (c) {
        case Color::Red:   std::cout << "红" << std::endl; break;
        case Color::Green: std::cout << "绿" << std::endl; break;
        case Color::Blue:  std::cout << "蓝" << std::endl; break;
        // 如果漏掉一个case，编译器可能会警告
    }

    // 陷阱4: 传统enum也可以在C++11中指定底层类型
    enum OldStyle : uint16_t { A = 1, B = 2, C = 3 };
    // 但仍然会隐式转换为整数，仍然污染命名空间
    int val = A; // 仍然可以隐式转换!
    (void)val;
    std::cout << "\n提示: 优先使用enum class而非传统enum" << std::endl;

    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 新代码始终使用enum class" << std::endl;
    std::cout << "  2. 根据值域选择合适的底层类型" << std::endl;
    std::cout << "  3. 需要位运算时，重载相关运算符" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: 强类型枚举 (enum class)" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 强类型枚举 (enum class / scoped enum)](https://en.cppreference.com/w/cpp/language/enum)
