---
sidebar_position: 2
title: "结构化绑定 (Structured Bindings)"
description: "C++17 结构化绑定 (Structured Bindings) 教学示例"
tags: [cpp17, structured_bindings]
---

# 结构化绑定 (Structured Bindings)

C++17 标准引入的 结构化绑定 (Structured Bindings) 特性完整教学示例。

## 完整源码

```cpp title="cpp17/01_structured_bindings.cpp"
/**
 * ============================================================
 * 特性名称: 结构化绑定 (Structured Bindings)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: 允许用 auto [x, y, z] = expr; 的语法将一个复合对象
 *          (pair/tuple/struct/数组) 的成员一次性绑定到多个变量上。
 *
 * [为什么引入]: C++11/14 中从 pair/tuple 取值需要 .first/.second
 *              或 std::get<>，代码繁琐且可读性差。结构化绑定让
 *              解构操作像 Python 的元组拆包一样简洁。
 *
 * [关键要点]:
 *   - 可解构 std::pair、std::tuple、原生数组、聚合体 struct
 *   - 在 range-based for 中使用 auto [k, v] 遍历 map 极为方便
 *   - 绑定的变量默认是值拷贝；用 auto& 可绑定为引用
 *   - 与 std::tie 的对比：tie 需要预先声明变量，且不能绑定数组
 * ============================================================
 */

#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <array>
#include <utility>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 解构 std::pair
    std::pair<int, std::string> p{42, "hello"};
    auto [num, str] = p;
    std::cout << "pair -> num=" << num << ", str=" << str << "\n";

    // 2. 解构 std::tuple
    std::tuple<int, double, std::string> t{1, 3.14, "pi"};
    auto [i, d, s] = t;
    std::cout << "tuple -> i=" << i << ", d=" << d << ", s=" << s << "\n";

    // 3. 解构原生数组
    int arr[] = {10, 20, 30};
    auto [a, b, c] = arr;
    std::cout << "array -> a=" << a << ", b=" << b << ", c=" << c << "\n";

    // 4. 解构聚合体 struct
    struct Point { double x; double y; };
    Point pt{1.5, 2.5};
    auto [x, y] = pt;
    std::cout << "struct -> x=" << x << ", y=" << y << "\n";

    // 5. 在 range-based for 中遍历 map
    std::map<std::string, int> scores{{"Alice", 90}, {"Bob", 85}};
    for (const auto& [name, score] : scores) {
        std::cout << name << " 的成绩: " << score << "\n";
    }
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. 引用绑定 —— 修改原始值
    std::pair<int, int> coord{3, 4};
    auto& [cx, cy] = coord;
    cx = 100;
    cy = 200;
    std::cout << "修改后 coord: (" << coord.first << ", " << coord.second << ")\n";

    // 2. 与 std::tie 的对比
    //    C++11 方式：需要预先声明变量
    int a_tie;
    std::string b_tie;
    std::tie(a_tie, b_tie) = std::make_pair(7, std::string("world"));
    std::cout << "tie -> a=" << a_tie << ", b=" << b_tie << "\n";

    //    C++17 方式：一行搞定
    auto [a17, b17] = std::make_pair(7, std::string("world"));
    std::cout << "structured binding -> a=" << a17 << ", b=" << b17 << "\n";

    // 3. 用于 map::insert 的返回值
    std::map<std::string, int> m;
    auto [iter, success] = m.insert({"key", 42});
    if (success) {
        std::cout << "插入成功: " << iter->first << " = " << iter->second << "\n";
    }

    // 4. 解构嵌套 —— 需要分步
    std::pair<int, std::pair<double, std::string>> nested{1, {2.0, "inner"}};
    auto [outer, inner_pair] = nested;
    auto [val, name] = inner_pair;
    std::cout << "嵌套解构: outer=" << outer
              << ", val=" << val << ", name=" << name << "\n";

    // 5. 配合 std::array 使用
    std::array<int, 3> sa = {100, 200, 300};
    auto [x, y, z] = sa;
    std::cout << "std::array -> " << x << ", " << y << ", " << z << "\n";
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: auto [x,y] = ... 默认是拷贝，不会影响原始对象
    std::pair<int, int> p{1, 2};
    auto [a, b] = p;       // 拷贝
    a = 999;
    std::cout << "陷阱1 - 拷贝: p.first 仍然是 " << p.first
              << " (没有被修改)\n";

    // 陷阱 2: 绑定数量必须与成员数量完全匹配
    // auto [x] = p;        // 编译错误！pair 有两个成员
    // auto [x,y,z] = p;    // 编译错误！pair 只有两个成员
    std::cout << "陷阱2 - 绑定数量必须与成员数量完全匹配\n";

    // 陷阱 3: 不能对带有私有成员的 class 使用结构化绑定
    //         (除非该类提供 tuple-like 接口)
    std::cout << "陷阱3 - 不能对私有成员的class直接使用结构化绑定\n";

    // 陷阱 4: const auto& 绑定时不能修改
    const auto& [ca, cb] = p;
    // ca = 10;  // 编译错误！const 引用不可修改
    std::cout << "陷阱4 - const auto& 绑定不可修改: ca=" << ca << "\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 结构化绑定 (Structured Bindings) - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 结构化绑定 (Structured Bindings)](https://en.cppreference.com/w/cpp/language/structured_binding)
