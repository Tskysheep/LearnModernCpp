---
sidebar_position: 4
slug: 03-ctad
title: "类模板参数推导 CTAD (Class Template Argument Deduction)"
description: "C++17 类模板参数推导 CTAD (Class Template Argument Deduction) 教学示例"
tags: [cpp17, ctad]
---

# 类模板参数推导 CTAD (Class Template Argument Deduction)

C++17 标准引入的 类模板参数推导 CTAD (Class Template Argument Deduction) 特性完整教学示例。

## 完整源码

```cpp title="cpp17/03_ctad.cpp"
/**
 * ============================================================
 * 特性名称: 类模板参数推导 CTAD (Class Template Argument Deduction)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: 编译器可以从构造函数的参数自动推导类模板参数，
 *          无需显式指定模板参数。例如 std::pair p{1, 2.0};
 *          自动推导为 std::pair<int, double>。
 *
 * [为什么引入]: C++14 中使用 pair<int,double>{1, 2.0} 或
 *              make_pair(1, 2.0) 都不够简洁。CTAD 让类模板
 *              的使用体验和函数模板一样自然。
 *
 * [关键要点]:
 *   - 标准库类型 pair/tuple/vector/optional 等都支持 CTAD
 *   - 可以为自定义类编写「推导指引」(deduction guide)
 *   - CTAD 让 make_xxx 系列辅助函数不再必要
 *   - 注意：std::vector v{1,2,3} 推导为 vector<int>，
 *     但 std::vector v{"hello"} 推导为 vector<const char*>
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <tuple>
#include <optional>
#include <array>
#include <mutex>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. std::pair —— 不需要写 pair<int, double>
    std::pair p{42, 3.14};     // 推导为 pair<int, double>
    std::cout << "pair: " << p.first << ", " << p.second << "\n";

    // 2. std::tuple —— 不需要写 tuple<int, double, string>
    std::tuple t{1, 2.0, std::string("hello")};
    std::cout << "tuple: " << std::get<0>(t) << ", "
              << std::get<1>(t) << ", " << std::get<2>(t) << "\n";

    // 3. std::vector —— 从初始化列表推导元素类型
    std::vector v{1, 2, 3, 4, 5};  // 推导为 vector<int>
    std::cout << "vector<int> 大小: " << v.size() << "\n";

    // 4. std::optional
    std::optional opt{42};  // 推导为 optional<int>
    std::cout << "optional: " << opt.value() << "\n";

    // 5. std::array（C++17 支持推导）
    std::array arr{1, 2, 3};  // 推导为 array<int, 3>
    std::cout << "array 大小: " << arr.size() << "\n";

    // 6. 与 make_xxx 的对比
    //    C++14 写法：
    auto p14 = std::make_pair(42, 3.14);
    auto t14 = std::make_tuple(1, 2.0, std::string("hello"));
    //    C++17 写法：直接构造，无需辅助函数
    std::pair p17{42, 3.14};
    std::tuple t17{1, 2.0, std::string("hello")};
    std::cout << "make_pair 与 CTAD 结果相同: "
              << (p14 == p17 ? "是" : "否") << "\n";
}

// ============================================================
// 自定义类的推导指引
// ============================================================

// 简单容器类
template<typename T>
class MyContainer {
public:
    MyContainer(T value, int count)
        : value_(value), count_(count) {}

    void print() const {
        for (int i = 0; i < count_; ++i) {
            std::cout << value_ << " ";
        }
        std::cout << "\n";
    }

private:
    T value_;
    int count_;
};

// 自定义推导指引：从 C 字符串推导为 std::string 版本
MyContainer(const char*, int) -> MyContainer<std::string>;

// 另一个例子：加法器
template<typename T>
struct Adder {
    T value;
    Adder(T v) : value(v) {}
    T add(T other) const { return value + other; }
};
// 推导指引（这里其实编译器能自动生成，只是展示语法）
template<typename T>
Adder(T) -> Adder<T>;

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. 自定义类的 CTAD
    MyContainer c1{42, 3};           // 推导为 MyContainer<int>
    MyContainer c2{"hello", 2};      // 通过推导指引 -> MyContainer<std::string>
    std::cout << "MyContainer<int>: ";
    c1.print();
    std::cout << "MyContainer<string>: ";
    c2.print();

    // 2. Adder 的 CTAD
    Adder a{3.14};  // 推导为 Adder<double>
    std::cout << "Adder: " << a.add(1.86) << "\n";

    // 3. std::lock_guard 也支持 CTAD
    std::mutex mtx;
    {
        std::lock_guard lock{mtx};  // 不需要写 lock_guard<mutex>
        std::cout << "lock_guard CTAD: 不需要指定 mutex 类型\n";
    }

    // 4. 拷贝推导 —— 从同类型容器推导
    std::vector original{1, 2, 3};
    std::vector copy{original};  // 推导为 vector<int>，拷贝构造
    std::cout << "拷贝推导: 大小=" << copy.size() << "\n";

    // 5. pair 嵌套推导
    std::pair nested{std::pair{1, 2}, std::string("nested")};
    std::cout << "嵌套 pair: ((" << nested.first.first
              << "," << nested.first.second
              << "), " << nested.second << ")\n";
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: const char* vs std::string
    std::vector v1{"hello", "world"};  // vector<const char*>，不是 vector<string>！
    // 如果想要 vector<string>，需要显式指定：
    std::vector<std::string> v2{"hello", "world"};
    std::cout << "陷阱1 - v1 是 vector<const char*>，v2 是 vector<string>\n";

    // 陷阱 2: 单元素 vector 的歧义
    std::vector v3{5};       // vector<int>，包含一个元素 5
    std::vector v4(5, 0);    // vector<int>，包含五个 0
    std::cout << "陷阱2 - v3 大小=" << v3.size()
              << ", v4 大小=" << v4.size() << "\n";

    // 陷阱 3: 不能对空容器使用 CTAD（无法推导类型）
    // std::vector v{};      // 编译错误！无法推导元素类型
    // std::optional o{};    // 编译错误！
    std::cout << "陷阱3 - 空容器不能用 CTAD，必须显式指定类型\n";

    // 陷阱 4: 初始化列表 vs 单值
    std::vector v5{std::vector{1, 2, 3}};
    // v5 是 vector<int>（拷贝），不是 vector<vector<int>>！
    std::cout << "陷阱4 - 拷贝推导优先: v5 大小=" << v5.size() << "\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 类模板参数推导 CTAD - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 类模板参数推导 CTAD (Class Template Argument Deduction)](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction)
