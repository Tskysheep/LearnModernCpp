---
sidebar_position: 42
title: "std::variant 与 std::visit (类型安全联合体)"
description: "C++17 std::variant 与 std::visit (类型安全联合体) 教学示例"
tags: [cpp17, variant]
---

# std::variant 与 std::visit (类型安全联合体)

C++17 标准引入的 std::variant 与 std::visit (类型安全联合体) 特性完整教学示例。

## 完整源码

```cpp title="cpp17/41_variant.cpp"
/**
 * ============================================================
 * 特性名称: std::variant 与 std::visit (类型安全联合体)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: std::variant<Types...> 是类型安全的联合体(tagged union)，
 *          同一时刻持有其中一个类型的值。std::visit 对 variant
 *          应用访问者模式，根据当前持有的类型自动派发。
 *
 * [为什么引入]: C 语言的 union 不是类型安全的，不知道当前存储的
 *              是哪个类型。variant 提供类型安全的替代方案，
 *              编译期保证所有类型都被处理。
 *
 * [关键要点]:
 *   - variant 默认构造为第一个类型的默认值
 *   - std::get<T> / std::get<index> 取值，类型不匹配抛 bad_variant_access
 *   - std::get_if<T> 返回指针，类型不匹配返回 nullptr（不抛异常）
 *   - std::visit + overloaded 模式 实现优雅的类型派发
 *   - variant 不能持有引用类型、void、数组
 * ============================================================
 */

#include <iostream>
#include <variant>
#include <string>
#include <vector>
#include <cmath>

// ============================================================
// overloaded 辅助结构 —— 用于 std::visit
// ============================================================
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// C++17 推导指引
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 创建 variant
    std::variant<int, double, std::string> v1;           // 默认: int{0}
    std::variant<int, double, std::string> v2 = 42;      // 持有 int
    std::variant<int, double, std::string> v3 = 3.14;    // 持有 double
    std::variant<int, double, std::string> v4 = "hello";  // 持有 string

    // 2. 检查当前持有哪个类型
    std::cout << "v1 当前类型索引: " << v1.index() << "\n";  // 0 = int
    std::cout << "v3 当前类型索引: " << v3.index() << "\n";  // 1 = double
    std::cout << "v4 当前类型索引: " << v4.index() << "\n";  // 2 = string

    // 3. 用 std::get<T> 取值
    std::cout << "v2 (int): " << std::get<int>(v2) << "\n";
    std::cout << "v3 (double): " << std::get<double>(v3) << "\n";
    std::cout << "v4 (string): " << std::get<std::string>(v4) << "\n";

    // 4. 用 std::get<index> 取值
    std::cout << "v2 (index 0): " << std::get<0>(v2) << "\n";

    // 5. 用 std::get_if<T> 安全取值
    if (auto* p = std::get_if<int>(&v2)) {
        std::cout << "v2 是 int: " << *p << "\n";
    }
    if (auto* p = std::get_if<double>(&v2)) {
        std::cout << "v2 是 double\n";
    } else {
        std::cout << "v2 不是 double\n";
    }

    // 6. 赋值 —— 改变持有的类型
    v2 = std::string("now a string");
    std::cout << "v2 变成了: " << std::get<std::string>(v2) << "\n";

    // 7. std::holds_alternative 检查类型
    std::cout << "v2 持有 string: " << std::holds_alternative<std::string>(v2) << "\n";
    std::cout << "v2 持有 int: " << std::holds_alternative<int>(v2) << "\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 用 variant 实现简单的表达式树
struct Add;
struct Mul;
struct Num;

using Expr = std::variant<int, std::string>;

// 1. 形状系统 —— 多态的替代方案
struct Circle  { double radius; };
struct Square  { double side; };
struct Triangle { double base, height; };

using Shape = std::variant<Circle, Square, Triangle>;

double area(const Shape& shape) {
    return std::visit(overloaded{
        [](const Circle& c)   { return 3.14159 * c.radius * c.radius; },
        [](const Square& s)   { return s.side * s.side; },
        [](const Triangle& t) { return 0.5 * t.base * t.height; },
    }, shape);
}

std::string shape_name(const Shape& shape) {
    return std::visit(overloaded{
        [](const Circle&)   -> std::string { return "圆形"; },
        [](const Square&)   -> std::string { return "正方形"; },
        [](const Triangle&) -> std::string { return "三角形"; },
    }, shape);
}

// 2. 状态机
using State = std::variant<
    struct Idle,
    struct Loading,
    struct Ready,
    struct Error
>;

struct Idle    {};
struct Loading { int progress; };
struct Ready   { std::string data; };
struct Error   { std::string message; };

void print_state(const State& state) {
    std::visit(overloaded{
        [](const Idle&)       { std::cout << "状态: 空闲\n"; },
        [](const Loading& l)  { std::cout << "状态: 加载中 " << l.progress << "%\n"; },
        [](const Ready& r)    { std::cout << "状态: 就绪 - " << r.data << "\n"; },
        [](const Error& e)    { std::cout << "状态: 错误 - " << e.message << "\n"; },
    }, state);
}

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 形状系统
    std::vector<Shape> shapes{
        Circle{5.0},
        Square{4.0},
        Triangle{3.0, 6.0}
    };
    for (const auto& s : shapes) {
        std::cout << shape_name(s) << " 的面积: " << area(s) << "\n";
    }

    // 状态机模拟
    std::cout << "\n状态机模拟:\n";
    State state = Idle{};
    print_state(state);

    state = Loading{30};
    print_state(state);

    state = Loading{80};
    print_state(state);

    state = Ready{"数据加载完成"};
    print_state(state);

    state = Error{"网络超时"};
    print_state(state);

    // overloaded 模式的展示
    std::cout << "\noverloaded 模式:\n";
    std::variant<int, double, std::string> data = 42;
    auto visitor = overloaded{
        [](int i)                { std::cout << "整数: " << i << "\n"; },
        [](double d)             { std::cout << "浮点: " << d << "\n"; },
        [](const std::string& s) { std::cout << "字符串: " << s << "\n"; },
    };

    std::visit(visitor, data);
    data = 3.14;
    std::visit(visitor, data);
    data = std::string("hello");
    std::visit(visitor, data);

    // 多 variant 的 visit
    std::cout << "\n多 variant visit:\n";
    std::variant<int, std::string> a = 10;
    std::variant<int, std::string> b = std::string("world");
    std::visit(overloaded{
        [](int x, int y)                              { std::cout << "两个整数: " << x+y << "\n"; },
        [](int x, const std::string& s)               { std::cout << "整数+字符串: " << x << " " << s << "\n"; },
        [](const std::string& s, int y)               { std::cout << "字符串+整数: " << s << " " << y << "\n"; },
        [](const std::string& s1, const std::string& s2) { std::cout << "两个字符串: " << s1 << s2 << "\n"; },
    }, a, b);
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: get<T> 类型不匹配时抛异常
    std::variant<int, std::string> v = 42;
    try {
        auto s = std::get<std::string>(v);  // 当前是 int！
        (void)s;
    } catch (const std::bad_variant_access& e) {
        std::cout << "陷阱1 - get<> 类型不匹配: " << e.what() << "\n";
    }

    // 陷阱 2: variant 默认构造为第一个类型
    //         如果第一个类型不可默认构造，variant 也不可默认构造
    std::variant<int, std::string> v2;  // 默认是 int{0}
    std::cout << "陷阱2 - 默认构造为第一个类型: " << std::get<int>(v2) << "\n";
    // 解决方案：用 std::monostate 作为第一个类型
    std::variant<std::monostate, int, std::string> v3;  // 默认是 monostate
    std::cout << "  用 monostate: index=" << v3.index() << "\n";

    // 陷阱 3: 类型歧义
    // std::variant<int, long> v = 42;   // 歧义！int 和 long 都能匹配
    // 解决：显式指定 std::variant<int, long> v{std::in_place_type<int>, 42};
    std::variant<int, long> v4{std::in_place_type<int>, 42};
    std::cout << "陷阱3 - 用 in_place_type 消除歧义: index=" << v4.index() << "\n";

    // 陷阱 4: variant 可能进入 valueless_by_exception 状态
    //         如果赋值时新值的构造函数抛异常，variant 可能变成无值状态
    std::cout << "陷阱4 - 赋值时异常可能导致 valueless_by_exception\n";
    std::cout << "  当前 v 是否 valueless: " << v.valueless_by_exception() << "\n";

    // 陷阱 5: visit 必须处理所有类型，否则编译错误
    //         这是一个优点 —— 编译期保证完整性
    std::cout << "陷阱5 - visit 必须处理所有类型（编译期保证）\n";

    // 陷阱 6: variant 的大小是最大类型的大小 + tag
    std::cout << "陷阱6 - sizeof: variant<int,double,string>="
              << sizeof(std::variant<int, double, std::string>)
              << ", int=" << sizeof(int)
              << ", string=" << sizeof(std::string) << "\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::variant 与 std::visit - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::variant 与 std::visit (类型安全联合体)](https://en.cppreference.com/w/cpp/utility/variant)
