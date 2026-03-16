---
sidebar_position: 6
slug: 05-spaceship-operator
title: "三路比较运算符 (Spaceship Operator ⟺)"
description: "C++20 三路比较运算符 (Spaceship Operator ⟺) 教学示例"
tags: [cpp20, spaceship_operator]
---

# 三路比较运算符 (Spaceship Operator `<=>`)

C++20 标准引入的 三路比较运算符 (Spaceship Operator `<=>`) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/05_spaceship_operator.cpp"
/**
 * ============================================================
 * 特性名称: 三路比较运算符 (Spaceship Operator <=>)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: <=> 运算符（又称太空船运算符）可以一次性比较两个对象，
 *          返回三种结果之一：小于、等于、大于。编译器可以据此
 *          自动生成全部六个比较运算符（<, >, <=, >=, ==, !=）。
 *
 * [为什么引入]: C++17 及之前，为一个类型实现完整的比较运算符
 *             需要手写6个重载，代码冗余且容易出错。
 *
 * [关键要点]:
 *   - <=> 返回 strong_ordering、weak_ordering 或 partial_ordering
 *   - = default 可以自动生成按成员比较的 <=>
 *   - 编译器可以从 <=> 自动推导出 <, >, <=, >=
 *   - == 和 != 需要单独默认或自定义（C++20 规则）
 * ============================================================
 */

#include <compare>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

// ===================== basic_usage() =====================

// 最简单的用法：使用 =default 自动生成所有比较运算符
struct Point {
    int x;
    int y;

    // 默认 <=> 按成员顺序逐一比较（先比较 x，若相等再比较 y）
    auto operator<=>(const Point&) const = default;
    // 注意：= default 的 <=> 同时生成 == 和 !=
};

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    Point p1{1, 2}, p2{1, 3}, p3{1, 2};

    // 自动生成的六个比较运算符
    std::cout << "p1{1,2} vs p2{1,3}:\n";
    std::cout << "  p1 == p2: " << std::boolalpha << (p1 == p2) << "\n";
    std::cout << "  p1 != p2: " << (p1 != p2) << "\n";
    std::cout << "  p1 <  p2: " << (p1 < p2)  << "\n";
    std::cout << "  p1 >  p2: " << (p1 > p2)  << "\n";
    std::cout << "  p1 <= p2: " << (p1 <= p2) << "\n";
    std::cout << "  p1 >= p2: " << (p1 >= p2) << "\n";

    std::cout << "\np1{1,2} vs p3{1,2}:\n";
    std::cout << "  p1 == p3: " << (p1 == p3) << "\n";

    // 直接使用 <=> 运算符
    auto result = p1 <=> p2;
    if (result < 0) std::cout << "\np1 < p2 (通过 <=> 判断)\n";
    else if (result > 0) std::cout << "\np1 > p2 (通过 <=> 判断)\n";
    else std::cout << "\np1 == p2 (通过 <=> 判断)\n";

    // 可以用在排序中
    std::vector<Point> points = {{3, 1}, {1, 2}, {2, 0}, {1, 1}};
    std::sort(points.begin(), points.end());
    std::cout << "\n排序后的点: ";
    for (const auto& p : points) {
        std::cout << "(" << p.x << "," << p.y << ") ";
    }
    std::cout << "\n";
}

// ===================== advanced_usage() =====================

// strong_ordering：完全有序，区分每个不同的值
// 例如：整数的比较
struct StudentID {
    int id;

    std::strong_ordering operator<=>(const StudentID& other) const {
        return id <=> other.id;
    }
    bool operator==(const StudentID& other) const = default;
};

// weak_ordering：弱序，某些不同的值可能被视为等价
// 例如：不区分大小写的字符串比较
struct CaseInsensitiveString {
    std::string value;

    std::weak_ordering operator<=>(const CaseInsensitiveString& other) const {
        // 逐字符比较（忽略大小写）
        auto to_lower = [](char c) -> char {
            return (c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c;
        };
        size_t len = std::min(value.size(), other.value.size());
        for (size_t i = 0; i < len; ++i) {
            char a = to_lower(value[i]);
            char b = to_lower(other.value[i]);
            if (a < b) return std::weak_ordering::less;
            if (a > b) return std::weak_ordering::greater;
        }
        if (value.size() < other.value.size()) return std::weak_ordering::less;
        if (value.size() > other.value.size()) return std::weak_ordering::greater;
        return std::weak_ordering::equivalent;
    }

    bool operator==(const CaseInsensitiveString& other) const {
        return (*this <=> other) == 0;
    }
};

// partial_ordering：偏序，某些值可能不可比较
// 例如：浮点数（NaN 与任何值都不可比较）
struct Temperature {
    double celsius;

    std::partial_ordering operator<=>(const Temperature& other) const {
        return celsius <=> other.celsius;  // double 的 <=> 返回 partial_ordering
    }
    bool operator==(const Temperature& other) const {
        return celsius == other.celsius;
    }
};

// 自定义 <=>：混合不同类型成员
struct Employee {
    std::string department;  // 先按部门排序
    int level;               // 再按级别排序
    std::string name;        // 最后按名字排序

    auto operator<=>(const Employee&) const = default;
    // 默认按声明顺序逐成员比较
};

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // strong_ordering 演示
    std::cout << "-- strong_ordering (完全有序) --\n";
    StudentID s1{1001}, s2{1002}, s3{1001};
    auto sr = s1 <=> s2;
    std::cout << "学号 1001 vs 1002: "
              << (sr < 0 ? "小于" : sr > 0 ? "大于" : "等于") << "\n";
    std::cout << "学号 1001 vs 1001: "
              << ((s1 <=> s3) == std::strong_ordering::equal ? "相等" : "不相等") << "\n";

    // weak_ordering 演示
    std::cout << "\n-- weak_ordering (弱序/等价) --\n";
    CaseInsensitiveString a{"Hello"}, b{"hello"}, c{"World"};
    std::cout << "\"Hello\" vs \"hello\": "
              << ((a <=> b) == std::weak_ordering::equivalent ? "等价" : "不等价") << "\n";
    std::cout << "\"Hello\" < \"World\": "
              << ((a <=> c) < 0 ? "是" : "否") << "\n";

    // partial_ordering 演示
    std::cout << "\n-- partial_ordering (偏序) --\n";
    Temperature t1{36.5}, t2{37.0};
    Temperature t_nan{std::numeric_limits<double>::quiet_NaN()};

    std::cout << "36.5°C vs 37.0°C: "
              << ((t1 <=> t2) < 0 ? "小于" : "不小于") << "\n";

    auto nan_result = t1 <=> t_nan;
    if (nan_result == std::partial_ordering::unordered) {
        std::cout << "36.5°C vs NaN: 不可比较 (unordered)\n";
    }

    // 在容器中使用
    std::cout << "\n-- 在有序容器中使用 --\n";
    std::set<Point> point_set = {{1, 2}, {3, 4}, {1, 1}, {2, 2}};
    std::cout << "set 中的点（自动有序）: ";
    for (const auto& p : point_set) {
        std::cout << "(" << p.x << "," << p.y << ") ";
    }
    std::cout << "\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：= default 的 <=> 可以自动生成 ==，但自定义的不行
    std::cout << "1. 自定义 <=> 不会自动生成 == 和 !=\n";
    std::cout << "   如果自定义了 operator<=>，需要额外声明 operator==\n";
    std::cout << "   通常: bool operator==(const T&) const = default;\n\n";

    // 陷阱2：ordering 类型的层级关系
    std::cout << "2. ordering 类型的隐式转换:\n";
    std::cout << "   strong_ordering → weak_ordering → partial_ordering\n";
    std::cout << "   反向转换不允许（不能从弱序得到强序）\n\n";

    // 陷阱3：成员中有指针时 = default 可能不符合预期
    std::cout << "3. 如果成员中有指针，= default 比较的是指针地址，不是指向的内容\n";
    std::cout << "   这种情况需要自定义 <=> 来比较指向的值\n\n";

    // 陷阱4：浮点数成员会导致 partial_ordering
    std::cout << "4. 如果类成员中包含 double/float，\n";
    std::cout << "   = default 的 <=> 返回 partial_ordering（因为 NaN 不可比较）\n";

    struct HasDouble {
        int id;
        double score;
        auto operator<=>(const HasDouble&) const = default;
    };

    // 验证：返回类型是 partial_ordering
    using result_type = decltype(HasDouble{} <=> HasDouble{});
    std::cout << "   HasDouble 的 <=> 是 partial_ordering: "
              << std::boolalpha
              << std::is_same_v<result_type, std::partial_ordering> << "\n\n";

    // 陷阱5：比较顺序
    std::cout << "5. = default 的 <=> 按成员声明顺序比较\n";
    std::cout << "   如果想按不同顺序，必须自定义 <=>\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== 三路比较运算符 <=> - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 三路比较运算符 (Spaceship Operator `<=>`)](https://en.cppreference.com/w/cpp/language/default_comparisons)
