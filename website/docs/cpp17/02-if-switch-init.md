---
sidebar_position: 3
title: "if/switch 中的初始化语句 (Init Statements)"
description: "C++17 if/switch 中的初始化语句 (Init Statements) 教学示例"
tags: [cpp17, if_switch_init]
---

# if/switch 中的初始化语句 (Init Statements)

C++17 标准引入的 if/switch 中的初始化语句 (Init Statements) 特性完整教学示例。

## 完整源码

```cpp title="cpp17/02_if_switch_init.cpp"
/**
 * ============================================================
 * 特性名称: if/switch 中的初始化语句 (Init Statements)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: 允许在 if 和 switch 的条件部分之前添加一个初始化语句，
 *          语法为 if (init; condition) 和 switch (init; condition)。
 *
 * [为什么引入]: 在 C++14 中，为了限制变量作用域，通常需要额外的花括号
 *              包裹代码块。C++17 的 init-statement 让变量的作用域
 *              自然地限制在 if/switch 块内，代码更简洁安全。
 *
 * [关键要点]:
 *   - if (auto x = expr; condition) —— x 在 if/else 块内可见
 *   - switch (auto x = expr; x) —— x 在整个 switch 块内可见
 *   - 特别适合与 map::find、lock_guard、optional 等配合使用
 *   - init 部分可以是任意语句，不限于变量定义
 * ============================================================
 */

#include <iostream>
#include <map>
#include <string>
#include <mutex>
#include <sstream>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. if 中的初始化语句
    //    C++14 写法：
    {
        auto val = 42;
        if (val > 0) {
            std::cout << "C++14 写法: val=" << val << " 是正数\n";
        }
    }
    //    C++17 写法：
    if (auto val = 42; val > 0) {
        std::cout << "C++17 写法: val=" << val << " 是正数\n";
    }
    // val 在此处已不可见，作用域被限制在 if 块内

    // 2. if-else 中初始化语句的变量在 else 中也可见
    if (auto result = 10 - 15; result >= 0) {
        std::cout << "非负数: " << result << "\n";
    } else {
        std::cout << "负数: " << result << "\n";
    }

    // 3. switch 中的初始化语句
    switch (auto code = 2; code) {
        case 1:
            std::cout << "代码 1\n";
            break;
        case 2:
            std::cout << "代码 2 (命中)\n";
            break;
        default:
            std::cout << "未知代码: " << code << "\n";
            break;
    }
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. 与 map::find 配合 —— 限制迭代器作用域
    std::map<std::string, int> scores{
        {"Alice", 90}, {"Bob", 85}, {"Charlie", 78}
    };

    if (auto it = scores.find("Bob"); it != scores.end()) {
        std::cout << "找到 Bob 的成绩: " << it->second << "\n";
    } else {
        std::cout << "没有找到 Bob\n";
    }
    // it 在此处已不可见

    // 2. 与 map::insert 配合（结合结构化绑定）
    if (auto [iter, ok] = scores.insert({"David", 92}); ok) {
        std::cout << "插入成功: " << iter->first << "=" << iter->second << "\n";
    } else {
        std::cout << "插入失败，键已存在\n";
    }

    // 3. 与 mutex/lock_guard 配合
    std::mutex mtx;
    if (std::lock_guard<std::mutex> lock(mtx); true) {
        std::cout << "在锁保护下执行操作\n";
        // lock 在 if 块结束时自动释放
    }

    // 4. 与字符串解析配合
    std::string input = "123abc";
    if (size_t pos = 0; auto val = std::stoi(input, &pos)) {
        std::cout << "解析的数字: " << val
                  << ", 剩余位置: " << pos << "\n";
    }

    // 5. 多重判断中限制作用域
    std::map<int, std::string> names{{1, "一"}, {2, "二"}, {3, "三"}};
    for (int key : {1, 4, 3}) {
        if (auto it = names.find(key); it != names.end()) {
            std::cout << key << " -> " << it->second << "\n";
        } else {
            std::cout << key << " -> 未找到\n";
        }
    }
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: init 语句中的变量只在 if/else 块内可见
    if (auto x = 10; x > 5) {
        std::cout << "陷阱1 - x 在 if 块内可见: " << x << "\n";
    }
    // std::cout << x;  // 编译错误！x 已不可见

    // 陷阱 2: 注意初始化语句和条件之间用分号分隔
    //         不要用逗号！
    // if (auto x = 10, x > 5)  // 错误语法！
    // if (auto x = 10; x > 5)  // 正确语法
    std::cout << "陷阱2 - 用分号(;)分隔初始化和条件，不是逗号(,)\n";

    // 陷阱 3: init 部分可以有副作用，但要注意求值顺序
    int counter = 0;
    if (auto prev = counter++; prev == 0) {
        std::cout << "陷阱3 - prev=" << prev
                  << ", counter=" << counter << "\n";
    }

    // 陷阱 4: switch 的 init 变量在所有 case 中可见
    //         但要注意跨 case 的变量声明问题
    switch (auto val = 3; val) {
        case 1:
            std::cout << "val 是 1\n";
            break;
        case 3: {
            // 如果在 case 中声明变量，最好用花括号
            auto msg = "val 是 3";
            std::cout << "陷阱4 - " << msg << "\n";
            break;
        }
        default:
            std::cout << "其他值: " << val << "\n";
            break;
    }
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== if/switch 初始化语句 - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: if/switch 中的初始化语句 (Init Statements)](https://en.cppreference.com/w/cpp)
