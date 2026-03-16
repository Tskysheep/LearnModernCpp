---
sidebar_position: 43
title: "std::array"
description: "C++11 std::array 教学示例"
tags: [cpp11, array]
---

# std::array

C++11 标准引入的 std::array 特性完整教学示例。

## 完整源码

```cpp title="cpp11/42_array.cpp"
/**
 * ============================================================
 * 特性名称: std::array
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: std::array 是固定大小的数组容器，是对C风格数组的安全封装，
 *           存储在栈上，大小在编译期确定，兼容STL算法和迭代器。
 *
 * [为什么引入]: C风格数组存在退化为指针、无法获取大小、无边界检查等问题。
 *               std::array 在保持零开销的前提下，提供了安全性和STL兼容性。
 *
 * [关键要点]:
 *   - 大小固定，编译期确定，存储在栈上（零开销封装）
 *   - 支持迭代器、begin/end，与STL算法完全兼容
 *   - at() 提供边界检查（越界抛出 std::out_of_range）
 *   - 支持 fill()、swap()、比较运算等操作
 *   - 不会退化为指针，保留大小信息
 * ============================================================
 */

#include <iostream>
#include <array>
#include <algorithm>
#include <numeric>
#include <string>
#include <stdexcept>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. 声明和初始化
    std::cout << "[1] 声明和初始化:\n";
    {
        std::array<int, 5> arr1 = {1, 2, 3, 4, 5};        // 列表初始化
        std::array<int, 5> arr2 = {{10, 20, 30, 40, 50}};  // 双大括号也可以
        std::array<int, 3> arr3{};                           // 值初始化（全0）
        std::array<double, 4> arr4 = {1.1, 2.2};            // 部分初始化，其余为0

        std::cout << "  arr1: ";
        for (const auto& v : arr1) std::cout << v << " ";
        std::cout << std::endl;

        std::cout << "  arr3 (值初始化): ";
        for (const auto& v : arr3) std::cout << v << " ";
        std::cout << std::endl;

        std::cout << "  arr4 (部分初始化): ";
        for (const auto& v : arr4) std::cout << v << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 2. 访问元素
    std::cout << "[2] 访问元素:\n";
    {
        std::array<int, 5> arr = {10, 20, 30, 40, 50};

        std::cout << "  arr[2] (operator[]): " << arr[2] << std::endl;
        std::cout << "  arr.at(3) (带边界检查): " << arr.at(3) << std::endl;
        std::cout << "  arr.front(): " << arr.front() << std::endl;
        std::cout << "  arr.back(): " << arr.back() << std::endl;
        std::cout << "  arr.data() (底层指针): " << arr.data() << std::endl;
    }
    std::cout << std::endl;

    // 3. at() 边界检查
    std::cout << "[3] at() 边界检查:\n";
    {
        std::array<int, 3> arr = {1, 2, 3};
        try {
            std::cout << "  尝试 arr.at(10)..." << std::endl;
            arr.at(10);  // 越界！
        } catch (const std::out_of_range& e) {
            std::cout << "  捕获异常: " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;

    // 4. 大小和容量
    std::cout << "[4] 大小和容量:\n";
    {
        std::array<int, 5> arr = {1, 2, 3, 4, 5};
        std::array<int, 0> empty_arr = {};

        std::cout << "  arr.size(): " << arr.size() << std::endl;
        std::cout << "  arr.max_size(): " << arr.max_size() << std::endl;
        std::cout << "  arr.empty(): " << (arr.empty() ? "是" : "否") << std::endl;
        std::cout << "  empty_arr.empty(): " << (empty_arr.empty() ? "是" : "否") << std::endl;
    }
    std::cout << std::endl;

    // 5. fill 和 swap
    std::cout << "[5] fill() 和 swap():\n";
    {
        std::array<int, 5> arr1, arr2;
        arr1.fill(7);
        arr2.fill(3);

        std::cout << "  arr1 fill(7): ";
        for (const auto& v : arr1) std::cout << v << " ";
        std::cout << std::endl;

        arr1.swap(arr2);

        std::cout << "  swap后 arr1: ";
        for (const auto& v : arr1) std::cout << v << " ";
        std::cout << std::endl;
        std::cout << "  swap后 arr2: ";
        for (const auto& v : arr2) std::cout << v << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. 与STL算法兼容
    std::cout << "[1] 与STL算法兼容:\n";
    {
        std::array<int, 8> arr = {5, 2, 8, 1, 9, 3, 7, 4};

        // 排序
        std::sort(arr.begin(), arr.end());
        std::cout << "  排序后: ";
        for (const auto& v : arr) std::cout << v << " ";
        std::cout << std::endl;

        // 反转
        std::reverse(arr.begin(), arr.end());
        std::cout << "  反转后: ";
        for (const auto& v : arr) std::cout << v << " ";
        std::cout << std::endl;

        // 查找
        auto it = std::find(arr.begin(), arr.end(), 5);
        if (it != arr.end()) {
            std::cout << "  找到 5，位置: " << std::distance(arr.begin(), it) << std::endl;
        }

        // 累加
        int sum = std::accumulate(arr.begin(), arr.end(), 0);
        std::cout << "  所有元素之和: " << sum << std::endl;

        // 最大最小
        auto minmax = std::minmax_element(arr.begin(), arr.end());
        std::cout << "  最小值: " << *minmax.first
                  << ", 最大值: " << *minmax.second << std::endl;
    }
    std::cout << std::endl;

    // 2. 与C数组对比
    std::cout << "[2] 与C数组对比:\n";
    {
        // C数组的问题
        int c_arr[] = {1, 2, 3, 4, 5};
        // sizeof(c_arr) / sizeof(c_arr[0]) 获取大小，但传入函数后退化为指针
        std::cout << "  C数组: sizeof获取大小=" << sizeof(c_arr)/sizeof(c_arr[0]) << std::endl;
        std::cout << "  C数组传入函数后退化为指针，丢失大小信息\n";

        // std::array 保留大小信息
        std::array<int, 5> std_arr = {1, 2, 3, 4, 5};
        std::cout << "  std::array: size()=" << std_arr.size() << " (始终可用)\n";

        // std::array 支持赋值
        std::array<int, 5> std_arr2;
        std_arr2 = std_arr;  // C数组不支持直接赋值
        std::cout << "  std::array 支持直接赋值（C数组不支持）\n";

        // std::array 支持比较
        std::cout << "  arr == arr2: " << (std_arr == std_arr2 ? "是" : "否") << std::endl;
    }
    std::cout << std::endl;

    // 3. 作为函数参数（保留大小信息）
    std::cout << "[3] 作为函数参数（模板 + 保留大小）:\n";
    {
        auto printArray = [](const std::array<int, 5>& arr) {
            std::cout << "  函数中 size()=" << arr.size() << ": ";
            for (const auto& v : arr) std::cout << v << " ";
            std::cout << std::endl;
        };

        std::array<int, 5> arr = {10, 20, 30, 40, 50};
        printArray(arr);
    }
    std::cout << std::endl;

    // 4. 多维 array
    std::cout << "[4] 多维 array:\n";
    {
        // 二维数组：3行4列
        std::array<std::array<int, 4>, 3> matrix = {{
            {{1, 2, 3, 4}},
            {{5, 6, 7, 8}},
            {{9, 10, 11, 12}}
        }};

        std::cout << "  3x4 矩阵:\n";
        for (const auto& row : matrix) {
            std::cout << "    ";
            for (const auto& val : row) {
                std::cout << val << "\t";
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

    // 5. 比较运算
    std::cout << "[5] 比较运算（字典序）:\n";
    {
        std::array<int, 3> a = {1, 2, 3};
        std::array<int, 3> b = {1, 2, 4};
        std::array<int, 3> c = {1, 2, 3};

        std::cout << "  {1,2,3} == {1,2,3}: " << (a == c ? "是" : "否") << std::endl;
        std::cout << "  {1,2,3} <  {1,2,4}: " << (a < b ? "是" : "否") << std::endl;
        std::cout << "  {1,2,3} != {1,2,4}: " << (a != b ? "是" : "否") << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. 大小必须是编译期常量
    std::cout << "[1] 大小必须是编译期常量:\n";
    std::cout << "  // int n = 5;\n";
    std::cout << "  // std::array<int, n> arr;  // 编译错误！n不是constexpr\n";
    std::cout << "  // constexpr int n = 5;\n";
    std::cout << "  // std::array<int, n> arr;  // 正确！\n\n";

    // 2. 未初始化的 array 包含垃圾值
    std::cout << "[2] 未初始化的 array（POD类型）包含未定义值:\n";
    {
        std::array<int, 3> uninit;  // 未初始化！值未定义
        std::cout << "  未初始化的 array 值不确定（可能是垃圾值）\n";
        std::cout << "  正确做法: std::array<int, 3> arr{};  // 值初始化为0\n";
        (void)uninit;
    }
    std::cout << std::endl;

    // 3. operator[] 不做边界检查
    std::cout << "[3] operator[] 不做边界检查:\n";
    std::cout << "  // std::array<int, 3> arr = {1, 2, 3};\n";
    std::cout << "  // arr[10];  // 未定义行为！不会抛异常\n";
    std::cout << "  // arr.at(10);  // 抛出 std::out_of_range\n\n";

    // 4. 不同大小的 array 是不同类型
    std::cout << "[4] 不同大小的 array 是不同类型:\n";
    std::cout << "  // std::array<int, 3> a;\n";
    std::cout << "  // std::array<int, 5> b;\n";
    std::cout << "  // a = b;  // 编译错误！类型不同\n";
    std::cout << "  // 这意味着不能写通用函数接收不同大小的array，\n";
    std::cout << "  // 除非用模板参数化大小\n\n";

    // 5. 大的 array 可能导致栈溢出
    std::cout << "[5] 大的 array 在栈上分配，可能导致栈溢出:\n";
    std::cout << "  // std::array<int, 10000000> huge;  // 约40MB在栈上，危险！\n";
    std::cout << "  // 大数组应使用 std::vector（堆上分配）\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::array - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::array](https://en.cppreference.com/w/cpp/container/array)
