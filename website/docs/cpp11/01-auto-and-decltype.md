---
sidebar_position: 2
title: "auto类型推导与decltype"
description: "C++11 auto类型推导与decltype 教学示例"
tags: [cpp11, auto_and_decltype]
---

# auto类型推导与decltype

C++11 标准引入的 auto类型推导与decltype 特性完整教学示例。

## 完整源码

```cpp title="cpp11/01_auto_and_decltype.cpp"
/**
 * ============================================================
 * 特性名称: auto类型推导与decltype
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: auto让编译器根据初始化表达式自动推导变量类型；
 *          decltype在不求值的情况下推导表达式的类型。
 *
 * [为什么引入]: C++03中，冗长的类型名（如迭代器）让代码难以
 *   阅读和维护。auto和decltype大幅简化了类型声明，同时保持
 *   强类型安全。
 *
 * [关键要点]:
 *   - auto会忽略顶层const和引用，需要时须显式写auto&/const auto&
 *   - decltype保留表达式的完整类型（包括引用和const）
 *   - 尾置返回类型(trailing return type)配合decltype可推导函数返回类型
 *   - auto不能用于函数参数（C++11）和非静态成员变量
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <typeinfo>

// ============================================================
// 辅助：尾置返回类型 —— 让返回类型依赖于参数类型
// ============================================================
template <typename T, typename U>
auto add(T a, U b) -> decltype(a + b) {
    return a + b;
}

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. auto 推导基本类型
    auto i = 42;          // int
    auto d = 3.14;        // double
    auto s = std::string("你好"); // std::string（注意不是const char*）

    std::cout << "i = " << i << ", 类型: " << typeid(i).name() << std::endl;
    std::cout << "d = " << d << ", 类型: " << typeid(d).name() << std::endl;
    std::cout << "s = " << s << ", 类型: " << typeid(s).name() << std::endl;

    // 2. auto 简化迭代器 —— 这是auto最经典的用途
    std::map<std::string, std::vector<int>> data;
    data["成绩"] = {90, 85, 92};

    // C++03 写法：std::map<std::string, std::vector<int>>::iterator it = data.begin();
    // C++11 写法：
    for (auto it = data.begin(); it != data.end(); ++it) {
        std::cout << it->first << ": ";
        for (auto val : it->second) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // 3. decltype 推导表达式类型
    int x = 10;
    decltype(x) y = 20;           // y 是 int
    decltype(x + 0.5) z = 1.5;   // z 是 double（int + double -> double）
    std::cout << "y = " << y << ", z = " << z << std::endl;

    // 4. 尾置返回类型
    auto result = add(1, 2.5);    // result 是 double
    std::cout << "add(1, 2.5) = " << result << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. auto 与 const/引用的交互
    int value = 100;
    const int& ref = value;

    auto a = ref;          // a 是 int（顶层const和引用被丢弃）
    auto& b = ref;         // b 是 const int&（保留引用和底层const）
    const auto& c = value; // c 是 const int&（显式加上const和引用）

    a = 999; // OK, a是独立的int
    // b = 999; // 错误! b是const int&
    std::cout << "a = " << a << ", value = " << value << std::endl;

    // 2. decltype 对左值表达式加括号会推导为引用
    int x = 42;
    decltype(x)   d1 = x;   // d1 是 int（x是变量名）
    decltype((x)) d2 = x;   // d2 是 int&（(x)是左值表达式）
    d2 = 100;
    std::cout << "修改d2后, x = " << x << std::endl; // x变成100

    // 3. decltype 用于推导容器元素类型
    std::vector<int> vec = {1, 2, 3};
    decltype(vec)::value_type elem = 10; // 等价于 int elem = 10;
    std::cout << "elem = " << elem << std::endl;

    // 4. auto 配合 new 表达式
    auto ptr = new std::vector<std::string>{"甲", "乙", "丙"};
    for (const auto& s : *ptr) {
        std::cout << s << " ";
    }
    std::cout << std::endl;
    delete ptr;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: auto 遇到花括号初始化 -> std::initializer_list
    auto il = {1, 2, 3}; // il 是 std::initializer_list<int>，不是vector!
    std::cout << "花括号初始化的类型: " << typeid(il).name() << std::endl;

    // 陷阱2: auto 推导代理对象
    std::vector<bool> flags = {true, false, true};
    auto flag = flags[0]; // flag 不是 bool! 而是 std::vector<bool>::reference（代理类）
    // 正确做法：
    bool real_flag = flags[0];
    std::cout << "flag类型: " << typeid(flag).name()
              << ", real_flag类型: " << typeid(real_flag).name() << std::endl;

    // 陷阱3: decltype 对变量名和表达式的行为不同
    int arr[5] = {1, 2, 3, 4, 5};
    // decltype(arr) 是 int[5]
    // decltype(arr + 0) 是 int*   （数组退化为指针）

    // 陷阱4: auto 不能用于未初始化的变量
    // auto x;  // 编译错误! auto必须有初始化表达式

    // 陷阱5: 不要滥用auto导致可读性下降
    // auto result = doSomething(); // 读者不知道result是什么类型
    // 建议: 当类型不明显时，显式写出类型更好

    std::cout << "提示: auto是工具，不是万能药。类型不清晰时请显式声明。" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: auto与decltype" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: auto类型推导与decltype](https://en.cppreference.com/w/cpp/language/auto)
