---
sidebar_position: 3
title: "范围for循环 (Range-based for loop)"
description: "C++11 范围for循环 (Range-based for loop) 教学示例"
tags: [cpp11, range_based_for]
---

# 范围for循环 (Range-based for loop)

C++11 标准引入的 范围for循环 (Range-based for loop) 特性完整教学示例。

## 完整源码

```cpp title="cpp11/02_range_based_for.cpp"
/**
 * ============================================================
 * 特性名称: 范围for循环 (Range-based for loop)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: 一种简洁的循环语法，用于遍历容器、数组或任何
 *          提供begin()/end()的对象。
 *
 * [为什么引入]: C++03中遍历容器需要手写迭代器或下标循环，
 *   代码冗长且容易出错。范围for循环让遍历变得简洁且不易出错。
 *
 * [关键要点]:
 *   - 语法: for (declaration : expression) { ... }
 *   - 用值拷贝遍历、用引用修改元素、用const引用只读高效访问
 *   - 可作用于数组、STL容器、initializer_list
 *   - 自定义类型需提供begin()和end()方法或对应的自由函数
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <initializer_list>

// ============================================================
// 自定义可遍历类型：简单的整数范围 [start, end)
// ============================================================
class IntRange {
public:
    // 迭代器类（最简单的前向迭代器）
    class Iterator {
    public:
        explicit Iterator(int val) : value_(val) {}
        int operator*() const { return value_; }
        Iterator& operator++() { ++value_; return *this; }
        bool operator!=(const Iterator& other) const { return value_ != other.value_; }
    private:
        int value_;
    };

    IntRange(int start, int end) : start_(start), end_(end) {}

    Iterator begin() const { return Iterator(start_); }
    Iterator end() const { return Iterator(end_); }

private:
    int start_;
    int end_;
};

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. 遍历STL容器
    std::vector<std::string> fruits = {"苹果", "香蕉", "橘子", "葡萄"};
    std::cout << "水果列表: ";
    for (const auto& fruit : fruits) {
        std::cout << fruit << " ";
    }
    std::cout << std::endl;

    // 2. 遍历C风格数组
    int scores[] = {90, 85, 78, 92, 88};
    std::cout << "成绩: ";
    for (auto score : scores) {
        std::cout << score << " ";
    }
    std::cout << std::endl;

    // 3. 遍历初始化列表（临时的initializer_list）
    std::cout << "直接遍历字面量: ";
    for (auto x : {10, 20, 30, 40}) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    // 4. 遍历std::map（结构化绑定是C++17，C++11用pair）
    std::map<std::string, int> ages = {{"张三", 25}, {"李四", 30}, {"王五", 28}};
    std::cout << "年龄表:" << std::endl;
    for (const auto& pair : ages) {
        std::cout << "  " << pair.first << ": " << pair.second << "岁" << std::endl;
    }

    // 5. 遍历字符串中的字符
    std::string text = "Hello";
    std::cout << "字符: ";
    for (char c : text) {
        std::cout << c << ' ';
    }
    std::cout << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 用引用修改容器元素
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::cout << "修改前: ";
    for (auto n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    for (auto& n : numbers) {
        n *= 2; // 通过引用直接修改原始元素
    }

    std::cout << "修改后(全部翻倍): ";
    for (const auto& n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    // 2. 使用自定义的IntRange类
    std::cout << "IntRange(1, 10): ";
    for (auto i : IntRange(1, 10)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // 3. 嵌套范围for循环
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    std::cout << "矩阵:" << std::endl;
    for (const auto& row : matrix) {
        std::cout << "  ";
        for (auto val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // 4. 使用auto&&（万能引用）—— 能绑定左值和右值
    std::vector<std::string> words = {"你好", "世界"};
    for (auto&& word : words) {
        std::cout << word << " ";
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: 在范围for循环中修改容器大小（添加/删除元素）
    // 这会导致未定义行为，因为迭代器会失效！
    std::vector<int> vec = {1, 2, 3, 4, 5};
    // 错误示例（不要这样做！）：
    // for (auto& v : vec) {
    //     if (v == 3) vec.push_back(99); // 未定义行为!
    // }
    std::cout << "警告: 范围for循环中不能添加/删除容器元素!" << std::endl;

    // 陷阱2: 对大型对象用值拷贝遍历（性能问题）
    std::vector<std::string> big_strings = {"很长很长的字符串一", "很长很长的字符串二"};
    // 不好 —— 每次迭代都拷贝一个string
    // for (auto s : big_strings) { ... }
    // 好 —— 用const引用避免拷贝
    for (const auto& s : big_strings) {
        std::cout << s << std::endl;
    }

    // 陷阱3: 范围for不提供索引
    // 如果你需要索引，应该用传统for循环或手动维护计数器
    std::vector<std::string> items = {"甲", "乙", "丙"};
    int index = 0;
    for (const auto& item : items) {
        std::cout << "  索引 " << index++ << ": " << item << std::endl;
    }

    // 陷阱4: 不能直接用于动态数组（指针）
    // int* arr = new int[5]{1,2,3,4,5};
    // for (auto x : arr) {} // 编译错误! 指针没有begin/end
    // delete[] arr;
    std::cout << "注意: 动态数组(指针)不能用范围for遍历" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: 范围for循环" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 范围for循环 (Range-based for loop)](https://en.cppreference.com/w/cpp/language/range-for)
