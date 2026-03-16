---
sidebar_position: 41
slug: 40-optional
title: "std::optional (可选值)"
description: "C++17 std::optional (可选值) 教学示例"
tags: [cpp17, optional]
---

# std::optional (可选值)

C++17 标准引入的 std::optional (可选值) 特性完整教学示例。

## 完整源码

```cpp title="cpp17/40_optional.cpp"
/**
 * ============================================================
 * 特性名称: std::optional (可选值)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: 一个可以「有值」或「无值」的包装类型。表示一个值可能存在
 *          也可能不存在，类似其他语言中的 Option/Maybe 类型。
 *
 * [为什么引入]: C++14 中表示「可能无值」通常用指针(nullptr)、
 *              哨兵值(-1, "")或 pair<bool,T>，都不够类型安全
 *              且语义不明确。optional 提供了统一、安全的解决方案。
 *
 * [关键要点]:
 *   - std::optional<T> 要么持有一个 T 值，要么是 std::nullopt
 *   - .value() 在无值时抛出 std::bad_optional_access 异常
 *   - .value_or(default) 无值时返回默认值，不抛异常
 *   - 可以用 if (opt) 或 if (opt.has_value()) 检查是否有值
 *   - 不会有堆分配——值直接存储在 optional 对象内部
 * ============================================================
 */

#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <map>
#include <cmath>

// ============================================================
// 基础用法
// ============================================================

// 函数返回 optional —— 替代返回指针或哨兵值
std::optional<int> find_first_even(const std::vector<int>& vec) {
    for (int v : vec) {
        if (v % 2 == 0) return v;  // 隐式构造 optional
    }
    return std::nullopt;  // 表示未找到
}

// 另一个例子：安全的除法
std::optional<double> safe_divide(double a, double b) {
    if (b == 0.0) return std::nullopt;
    return a / b;
}

void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 创建 optional
    std::optional<int> opt1;                  // 空，nullopt
    std::optional<int> opt2 = 42;             // 有值
    std::optional<int> opt3 = std::nullopt;   // 显式空
    std::optional<std::string> opt4{"hello"}; // 有值

    // 2. 检查是否有值
    std::cout << "opt1 有值: " << opt1.has_value() << "\n";
    std::cout << "opt2 有值: " << opt2.has_value() << "\n";
    std::cout << "opt3 有值: " << (opt3 ? "是" : "否") << "\n";  // bool 转换

    // 3. 访问值
    std::cout << "opt2.value() = " << opt2.value() << "\n";
    std::cout << "*opt2        = " << *opt2 << "\n";  // 解引用

    // 4. value_or —— 提供默认值
    std::cout << "opt1.value_or(0) = " << opt1.value_or(0) << "\n";
    std::cout << "opt2.value_or(0) = " << opt2.value_or(0) << "\n";

    // 5. 函数返回 optional
    std::vector<int> nums{1, 3, 5, 8, 9};
    if (auto result = find_first_even(nums)) {
        std::cout << "第一个偶数: " << *result << "\n";
    } else {
        std::cout << "没有偶数\n";
    }

    std::vector<int> odds{1, 3, 5, 7};
    auto result2 = find_first_even(odds);
    std::cout << "全是奇数时: " << result2.value_or(-1) << "\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 1. optional 做类成员 —— 表示可选字段
struct UserProfile {
    std::string name;
    std::optional<std::string> email;       // 可选
    std::optional<int> age;                  // 可选
    std::optional<std::string> nickname;     // 可选

    void print() const {
        std::cout << "  姓名: " << name << "\n";
        std::cout << "  邮箱: " << email.value_or("未设置") << "\n";
        std::cout << "  年龄: ";
        if (age) std::cout << *age << "\n";
        else     std::cout << "未设置\n";
        std::cout << "  昵称: " << nickname.value_or(name) << "\n";
    }
};

// 2. optional 链式操作（模拟 monadic 操作，C++23 才有 and_then）
std::optional<std::string> get_config_value(const std::string& key) {
    std::map<std::string, std::string> config{
        {"host", "localhost"}, {"port", "8080"}
    };
    if (auto it = config.find(key); it != config.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<int> parse_int(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return std::nullopt;
    }
}

// 3. emplace —— 就地构造
// 4. 与指针方案的对比

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 可选字段的结构体
    UserProfile user1{"张三", "zhangsan@email.com", 25, std::nullopt};
    UserProfile user2{"李四", std::nullopt, std::nullopt, "小四"};

    std::cout << "用户1:\n";
    user1.print();
    std::cout << "用户2:\n";
    user2.print();

    // 链式查询
    auto port_str = get_config_value("port");
    std::optional<int> port;
    if (port_str) {
        port = parse_int(*port_str);
    }
    std::cout << "端口: " << port.value_or(80) << "\n";

    auto missing = get_config_value("timeout");
    std::cout << "超时: " << missing.value_or("5000") << "\n";

    // emplace —— 就地构造，避免临时对象
    std::optional<std::string> opt;
    opt.emplace(5, 'A');  // 构造 string("AAAAA")
    std::cout << "emplace 构造: " << *opt << "\n";

    // reset —— 清除值
    opt.reset();
    std::cout << "reset 后有值: " << opt.has_value() << "\n";

    // optional 比较
    std::optional<int> a = 5;
    std::optional<int> b = 10;
    std::optional<int> c;
    std::cout << "5 < 10: " << (a < b) << "\n";
    std::cout << "nullopt < 5: " << (c < a) << "\n";  // nullopt 小于任何值
}

// ============================================================
// 与旧方案的对比
// ============================================================
void compare_with_old() {
    std::cout << "\n--- 与旧方案对比 ---\n";

    // 方案 1: 指针方案 —— 需要动态分配或引用外部变量
    // int* find_value_old(...) { return nullptr; }
    // 问题：所有权不明确，可能悬垂

    // 方案 2: 哨兵值 —— -1 表示未找到
    // int find_value_old(...) { return -1; }
    // 问题：-1 可能是合法值

    // 方案 3: pair<bool, T>
    // pair<bool, int> find_value_old(...) { return {false, 0}; }
    // 问题：false 时 second 的值无意义但仍需构造

    // 方案 4: optional —— 最佳方案
    auto result = safe_divide(10.0, 3.0);
    std::cout << "10/3 = " << result.value_or(0.0) << "\n";

    auto bad = safe_divide(10.0, 0.0);
    std::cout << "10/0 = " << (bad ? std::to_string(*bad) : "除零错误") << "\n";
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: 对空 optional 调用 value() 会抛异常
    std::optional<int> empty;
    try {
        auto v = empty.value();  // 抛出 bad_optional_access
        (void)v;
    } catch (const std::bad_optional_access& e) {
        std::cout << "陷阱1 - 空 optional 调用 value(): " << e.what() << "\n";
    }

    // 陷阱 2: 对空 optional 解引用是 UB（未定义行为）
    // auto v = *empty;  // UB！不会抛异常，但行为未定义
    std::cout << "陷阱2 - 对空 optional 用 * 解引用是 UB，不会抛异常\n";

    // 陷阱 3: optional<bool> 的条件判断容易混淆
    std::optional<bool> opt_false = false;
    if (opt_false) {
        // 这里会进入！因为 optional 有值（虽然值是 false）
        std::cout << "陷阱3 - optional<bool>(false) 转 bool 为 true"
                  << " (因为有值)，值=" << *opt_false << "\n";
    }

    // 陷阱 4: optional 不支持引用
    // std::optional<int&> ref_opt;  // 编译错误！
    // 替代方案：使用 std::reference_wrapper
    std::cout << "陷阱4 - optional 不支持引用类型 optional<T&>\n";

    // 陷阱 5: optional 有额外存储开销
    std::cout << "陷阱5 - sizeof(int)=" << sizeof(int)
              << ", sizeof(optional<int>)=" << sizeof(std::optional<int>)
              << " (多了 bool 标志 + 对齐填充)\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::optional (可选值) - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    compare_with_old();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::optional (可选值)](https://en.cppreference.com/w/cpp/utility/optional)
