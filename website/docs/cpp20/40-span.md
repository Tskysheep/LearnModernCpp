---
sidebar_position: 41
slug: 40-span
title: "std::span"
description: "C++20 std::span 教学示例"
tags: [cpp20, span]
---

# std::span

C++20 标准引入的 std::span 特性完整教学示例。

## 完整源码

```cpp title="cpp20/40_span.cpp"
/**
 * ============================================================
 * 特性名称: std::span
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: std::span 是一个轻量级的、非拥有的连续内存视图，
 *          可以引用数组、vector 或其他连续容器的一部分或全部。
 *
 * [为什么引入]: 传统 C++ 中传递数组需要传 (指针, 大小) 两个参数，
 *             容易出错。span 将两者封装为一个对象，
 *             既安全又高效。
 *
 * [关键要点]:
 *   - 不拥有数据，只是一个视图（类似 string_view 之于 string）
 *   - 支持动态 extent (std::dynamic_extent) 和静态 extent
 *   - 可以从数组、vector、array 等容器隐式转换
 *   - O(1) 复制，非常轻量
 * ============================================================
 */

#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <span>
#include <string>
#include <vector>

// ===================== basic_usage() =====================

// 传统方式：需要传指针和大小
void print_array_old(const int* data, std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << "\n";
}

// C++20 方式：使用 span
void print_array(std::span<const int> data) {
    for (int n : data) {
        std::cout << n << " ";
    }
    std::cout << "\n";
}

// span 可以接受各种连续容器
void process(std::span<const double> values) {
    double sum = 0;
    for (double v : values) sum += v;
    double avg = values.empty() ? 0.0 : sum / static_cast<double>(values.size());
    std::cout << "  大小: " << values.size()
              << ", 总和: " << sum
              << ", 平均: " << avg << "\n";
}

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // span 可以从不同来源创建
    // 1. C 数组
    int c_array[] = {1, 2, 3, 4, 5};
    std::cout << "C 数组: ";
    print_array(c_array);

    // 2. std::array
    std::array<int, 4> std_array = {10, 20, 30, 40};
    std::cout << "std::array: ";
    print_array(std_array);

    // 3. std::vector
    std::vector<int> vec = {100, 200, 300};
    std::cout << "vector: ";
    print_array(vec);

    // 4. 初始化列表（需要显式构造）
    // print_array({1, 2, 3});  // 不能直接传初始化列表

    // span 的基本操作
    std::span<int> s(vec);
    std::cout << "\nspan 基本操作:\n";
    std::cout << "  size(): " << s.size() << "\n";
    std::cout << "  front(): " << s.front() << "\n";
    std::cout << "  back(): " << s.back() << "\n";
    std::cout << "  [1]: " << s[1] << "\n";
    std::cout << "  data(): " << s.data() << "\n";
    std::cout << "  empty(): " << std::boolalpha << s.empty() << "\n";
    std::cout << "  size_bytes(): " << s.size_bytes() << "\n";

    // 统一接口处理不同容器
    std::cout << "\n统一处理不同容器:\n";
    double arr[] = {1.0, 2.0, 3.0};
    std::array<double, 4> sarr = {10.0, 20.0, 30.0, 40.0};
    std::vector<double> vd = {100.0, 200.0};

    std::cout << "C 数组: "; process(arr);
    std::cout << "std::array: "; process(sarr);
    std::cout << "vector: "; process(vd);
}

// ===================== advanced_usage() =====================

// 静态 extent：编译期已知大小
void process_fixed(std::span<const int, 3> data) {
    std::cout << "固定大小 span (3个元素): ";
    for (int n : data) std::cout << n << " ";
    std::cout << "\n";
}

// 使用 span 修改数据
void double_values(std::span<int> data) {
    for (int& n : data) {
        n *= 2;
    }
}

// 矩阵行视图
void print_matrix_row(std::span<const int> row, int row_num) {
    std::cout << "  行 " << row_num << ": ";
    for (int n : row) std::cout << n << "\t";
    std::cout << "\n";
}

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 子视图：first、last、subspan
    std::vector<int> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::span<int> full(data);

    std::cout << "-- 子视图 --\n";
    std::cout << "完整: "; print_array(full);

    std::cout << "前3个 (first): "; print_array(full.first(3));
    std::cout << "后4个 (last): "; print_array(full.last(4));
    std::cout << "子范围 [2,5) (subspan): "; print_array(full.subspan(2, 3));

    // 静态 extent
    std::cout << "\n-- 静态 extent --\n";
    int arr3[3] = {10, 20, 30};
    process_fixed(arr3);

    // 静态 extent 到动态 extent 的隐式转换
    std::span<const int, 3> static_span(arr3);
    std::span<const int> dynamic_span = static_span;  // 合法：静态 → 动态
    std::cout << "静态→动态转换: "; print_array(dynamic_span);

    // 通过 span 修改原始数据
    std::cout << "\n-- 通过 span 修改数据 --\n";
    std::vector<int> values = {1, 2, 3, 4, 5};
    std::cout << "修改前: "; print_array(values);
    double_values(values);
    std::cout << "修改后: "; print_array(values);

    // 用 span 表示矩阵行
    std::cout << "\n-- 矩阵视图 --\n";
    int matrix[] = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12
    };
    constexpr int cols = 4;
    constexpr int rows = 3;
    std::span<int> mat(matrix, rows * cols);

    for (int r = 0; r < rows; ++r) {
        print_matrix_row(mat.subspan(static_cast<std::size_t>(r * cols),
                                      static_cast<std::size_t>(cols)), r);
    }

    // span 与算法配合
    std::cout << "\n-- 与算法配合 --\n";
    std::vector<int> nums = {5, 3, 8, 1, 9, 2, 7};
    std::span<int> nums_span(nums);

    std::ranges::sort(nums_span);
    std::cout << "排序后: "; print_array(nums_span);

    auto it = std::ranges::find(nums_span, 7);
    if (it != nums_span.end()) {
        std::cout << "找到 7\n";
    }

    // span<byte> 用于字节级操作
    std::cout << "\n-- 字节视图 --\n";
    int value = 0x12345678;
    auto bytes = std::as_bytes(std::span<int, 1>(&value, 1));
    std::cout << "0x12345678 的字节: ";
    for (auto b : bytes) {
        std::cout << std::hex << static_cast<unsigned>(static_cast<unsigned char>(b)) << " ";
    }
    std::cout << std::dec << "\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：span 不拥有数据
    std::cout << "1. span 不拥有数据（类似 string_view）:\n";
    std::cout << "   auto bad_span = std::span(get_temp_vector());\n";
    std::cout << "   // 临时 vector 销毁后，span 悬空!\n\n";

    // 陷阱2：不检查越界
    std::cout << "2. operator[] 不检查越界（与 vector 一样）:\n";
    std::cout << "   span<int> s(vec);\n";
    std::cout << "   s[100];  // 未定义行为（无越界检查）\n\n";

    // 陷阱3：动态 extent 和静态 extent 不能随意转换
    std::cout << "3. 动态 extent → 静态 extent 不能隐式转换:\n";
    std::cout << "   span<int> dynamic_s(vec);          // 动态\n";
    std::cout << "   span<int, 3> static_s = dynamic_s;  // 编译错误!\n";
    std::cout << "   需要: span<int, 3> static_s(dynamic_s.data(), 3);\n\n";

    // 陷阱4：const 正确性
    std::cout << "4. span<int> 允许修改元素，span<const int> 不允许:\n";
    std::vector<int> v = {1, 2, 3};
    std::span<int> mutable_span(v);
    std::span<const int> const_span(v);
    mutable_span[0] = 100;  // 合法
    // const_span[0] = 100;  // 编译错误
    std::cout << "   span<int>: 可写;  span<const int>: 只读\n\n";

    // 陷阱5：span 与 string_view 的区别
    std::cout << "5. span 与 string_view 的区别:\n";
    std::cout << "   string_view: 专门用于字符序列，有 find、substr 等方法\n";
    std::cout << "   span: 通用的连续内存视图，适用于任何类型\n";
    std::cout << "   两者不能互换使用\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== std::span - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::span](https://en.cppreference.com/w/cpp/container/span)
