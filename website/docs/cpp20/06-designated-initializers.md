---
sidebar_position: 7
slug: 06-designated-initializers
title: "指定初始化器 (Designated Initializers)"
description: "C++20 指定初始化器 (Designated Initializers) 教学示例"
tags: [cpp20, designated_initializers]
---

# 指定初始化器 (Designated Initializers)

C++20 标准引入的 指定初始化器 (Designated Initializers) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/06_designated_initializers.cpp"
/**
 * ============================================================
 * 特性名称: 指定初始化器 (Designated Initializers)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: 指定初始化器允许在初始化聚合体时，
 *          用 .member = value 的方式明确指定每个成员的初始值。
 *
 * [为什么引入]: C99 早就支持此特性，C++ 终于引入。
 *             可以提高代码可读性，明确哪个值对应哪个成员，
 *             避免因成员顺序记错而导致的 bug。
 *
 * [关键要点]:
 *   - 初始化器必须按成员声明顺序出现（C++ 规则，C 没有此限制）
 *   - 未指定的成员会被值初始化（零初始化或默认构造）
 *   - 只能用于聚合体（无用户声明构造函数、无虚函数等）
 *   - 不支持嵌套指定（.a.b = val）和数组指定（[0] = val），这是与 C99 的区别
 * ============================================================
 */

#include <iostream>
#include <string>
#include <array>

// ===================== basic_usage() =====================

// 简单的聚合体
struct Color {
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;
};

struct Config {
    int width = 800;
    int height = 600;
    bool fullscreen = false;
    bool vsync = true;
    int fps_limit = 60;
};

void print_color(const Color& c) {
    std::cout << "  Color(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")\n";
}

void print_config(const Config& cfg) {
    std::cout << "  窗口: " << cfg.width << "x" << cfg.height
              << (cfg.fullscreen ? " 全屏" : " 窗口")
              << (cfg.vsync ? " 垂直同步" : "")
              << " 帧率上限: " << cfg.fps_limit << "\n";
}

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 传统初始化：必须按顺序提供所有值
    Color red_old = {255, 0, 0, 255};

    // C++20 指定初始化器：可以命名每个成员
    Color red = {.r = 255, .g = 0, .b = 0, .a = 255};
    std::cout << "红色: ";
    print_color(red);

    // 未指定的成员使用默认值
    Color blue = {.b = 255};  // r=0, g=0, a=255 (默认值)
    std::cout << "蓝色 (只指定 b): ";
    print_color(blue);

    // 实际应用：配置结构
    Config my_config = {
        .width = 1920,
        .height = 1080,
        .fullscreen = true,
        // vsync 使用默认值 true
        .fps_limit = 144
    };
    std::cout << "\n游戏配置: ";
    print_config(my_config);

    // 只修改少数参数，其余使用默认值
    Config simple_config = {.fullscreen = true};
    std::cout << "简单配置 (只改全屏): ";
    print_config(simple_config);
}

// ===================== advanced_usage() =====================

// 带嵌套结构的聚合体
struct Vector2D {
    float x = 0.0f;
    float y = 0.0f;
};

struct Rect {
    Vector2D position;
    Vector2D size;
    Color color;
};

// 带数组成员的聚合体
struct Matrix2x2 {
    float data[4] = {1, 0, 0, 1};  // 默认为单位矩阵
};

// 联合体也支持指定初始化器
union Variant {
    int i;
    float f;
    char c;
};

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 嵌套结构体的指定初始化
    Rect button = {
        .position = {.x = 100.0f, .y = 200.0f},
        .size = {.x = 80.0f, .y = 30.0f},
        .color = {.r = 0, .g = 128, .b = 255}
    };
    std::cout << "按钮位置: (" << button.position.x << ", " << button.position.y << ")\n";
    std::cout << "按钮大小: " << button.size.x << " x " << button.size.y << "\n";
    std::cout << "按钮颜色: ";
    print_color(button.color);

    // 联合体的指定初始化
    Variant v1 = {.i = 42};
    Variant v2 = {.f = 3.14f};
    std::cout << "\n联合体 (int): " << v1.i << "\n";
    std::cout << "联合体 (float): " << v2.f << "\n";

    // 在函数参数中使用（直接构造临时对象）
    std::cout << "\n直接在函数调用中使用:\n";
    print_color({.r = 128, .g = 128, .b = 128});  // 灰色

    // 与 const 结合
    const Config default_config = {
        .width = 1024,
        .height = 768,
    };
    std::cout << "默认配置: ";
    print_config(default_config);
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：必须按声明顺序
    std::cout << "1. 初始化器必须按成员声明顺序出现（C++ 强制要求）:\n";
    std::cout << "   struct S { int a; int b; int c; };\n";
    std::cout << "   S s = {.a=1, .c=3};       // 合法：跳过 b\n";
    std::cout << "   S s = {.c=3, .a=1};       // 编译错误：顺序不对!\n\n";

    // 演示合法的跳过
    struct S { int a = 0; int b = 0; int c = 0; };
    S s = {.a = 1, .c = 3};  // b 使用默认值 0
    std::cout << "   跳过成员: a=" << s.a << " b=" << s.b << " c=" << s.c << "\n\n";

    // 陷阱2：与 C99 的区别
    std::cout << "2. 与 C99 指定初始化器的区别:\n";
    std::cout << "   C99 允许乱序: {.c=3, .a=1}           C++ 不允许\n";
    std::cout << "   C99 允许数组指定: {[0]=1, [3]=4}     C++ 不允许\n";
    std::cout << "   C99 允许嵌套指定: {.a.x=1}           C++ 不允许\n\n";

    // 陷阱3：不能与位置初始化混用
    std::cout << "3. 不能混用指定初始化和位置初始化:\n";
    std::cout << "   S s = {1, .b=2, 3};  // 编译错误!\n";
    std::cout << "   要么全部用指定初始化，要么全部用位置初始化\n\n";

    // 陷阱4：只能用于聚合体
    std::cout << "4. 只能用于聚合体（aggregate）:\n";
    std::cout << "   - 无用户声明的构造函数\n";
    std::cout << "   - 无 private/protected 非静态数据成员\n";
    std::cout << "   - 无虚函数\n";
    std::cout << "   - 无虚/private/protected 基类\n\n";

    // 陷阱5：窄化转换检查
    std::cout << "5. 指定初始化器使用花括号初始化，会检查窄化转换:\n";
    std::cout << "   Color c = {.r = 256};   // 如果 r 是 char，这是窄化错误\n";
    std::cout << "   Color c = {.r = 3.14};  // double→int 是窄化错误\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== 指定初始化器 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 指定初始化器 (Designated Initializers)](https://en.cppreference.com/w/cpp/language/aggregate_initialization)
