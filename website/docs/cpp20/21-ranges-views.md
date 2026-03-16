---
sidebar_position: 22
slug: 21-ranges-views
title: "Ranges 视图 (Views: filter/transform/take 等)"
description: "C++20 Ranges 视图 (Views: filter/transform/take 等) 教学示例"
tags: [cpp20, ranges_views]
---

# Ranges 视图 (Views: filter/transform/take 等)

C++20 标准引入的 Ranges 视图 (Views: filter/transform/take 等) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/21_ranges_views.cpp"
/**
 * ============================================================
 * 特性名称: Ranges 视图 (Views: filter/transform/take 等)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: 视图（View）是轻量级、惰性求值的范围适配器，
 *          可以通过管道操作符 | 组合使用，对数据进行筛选、变换等操作。
 *
 * [为什么引入]: 传统 STL 算法需要中间容器存储结果，
 *             视图通过惰性求值避免了不必要的内存分配和复制。
 *
 * [关键要点]:
 *   - 惰性求值：只在迭代时才计算
 *   - 管道操作符 | 可以链式组合多个视图
 *   - 视图本身不拥有数据，只是对底层范围的一种"视角"
 *   - 常用视图：filter、transform、take、drop、reverse、iota 等
 * ============================================================
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

// ===================== basic_usage() =====================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 1. views::filter — 筛选
    std::cout << "偶数: ";
    for (int n : nums | std::views::filter([](int n) { return n % 2 == 0; })) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // 2. views::transform — 变换
    std::cout << "平方: ";
    for (int n : nums | std::views::transform([](int n) { return n * n; })) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // 3. views::take — 取前 N 个
    std::cout << "前5个: ";
    for (int n : nums | std::views::take(5)) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // 4. views::drop — 跳过前 N 个
    std::cout << "跳过前3个: ";
    for (int n : nums | std::views::drop(3)) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // 5. views::reverse — 反转
    std::cout << "反转: ";
    for (int n : nums | std::views::reverse) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // 6. views::iota — 生成序列
    std::cout << "iota(1, 11): ";
    for (int n : std::views::iota(1, 11)) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // 无限序列 + take
    std::cout << "无限序列取前10个: ";
    for (int n : std::views::iota(1) | std::views::take(10)) {
        std::cout << n << " ";
    }
    std::cout << "\n";
}

// ===================== advanced_usage() =====================
void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // ---- 管道组合多个视图 ----
    std::cout << "-- 管道组合 --\n";

    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    // 筛选偶数 → 平方 → 取前3个
    std::cout << "偶数的平方(前3个): ";
    auto pipeline = nums
        | std::views::filter([](int n) { return n % 2 == 0; })
        | std::views::transform([](int n) { return n * n; })
        | std::views::take(3);

    for (int n : pipeline) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // 每3个一组
    std::cout << "\n3个一组(keys):\n";
    auto chunked = nums | std::views::take(9);
    int group = 0;
    int count = 0;
    for (int n : chunked) {
        if (count % 3 == 0) std::cout << "  组" << ++group << ": ";
        std::cout << n << " ";
        if (++count % 3 == 0) std::cout << "\n";
    }

    // ---- 惰性求值演示 ----
    std::cout << "\n-- 惰性求值 --\n";

    auto lazy_view = std::views::iota(1)
        | std::views::filter([](int n) {
            // 这个 Lambda 只在迭代时被调用
            return n % 3 == 0;
        })
        | std::views::transform([](int n) {
            return n * n;
        })
        | std::views::take(5);

    std::cout << "3的倍数的平方(前5个): ";
    for (int n : lazy_view) {
        std::cout << n << " ";
    }
    std::cout << "\n";
    std::cout << "(惰性求值：只计算了需要的元素，无限序列也没问题)\n";

    // ---- 字符串视图操作 ----
    std::cout << "\n-- 字符串操作 --\n";

    std::string text = "Hello, World!";
    std::cout << "原始: " << text << "\n";

    // 转换为大写
    std::cout << "大写: ";
    for (char c : text | std::views::transform([](char c) {
        return (c >= 'a' && c <= 'z') ? static_cast<char>(c - 32) : c;
    })) {
        std::cout << c;
    }
    std::cout << "\n";

    // 只保留字母
    std::cout << "只留字母: ";
    for (char c : text | std::views::filter([](char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    })) {
        std::cout << c;
    }
    std::cout << "\n";

    // ---- 视图变量复用 ----
    std::cout << "\n-- 视图复用 --\n";

    auto evens = std::views::filter([](int n) { return n % 2 == 0; });
    auto square = std::views::transform([](int n) { return n * n; });

    std::vector<int> v1 = {1, 2, 3, 4, 5};
    std::vector<int> v2 = {10, 20, 30, 40, 50};

    std::cout << "v1 偶数平方: ";
    for (int n : v1 | evens | square) std::cout << n << " ";
    std::cout << "\n";

    std::cout << "v2 偶数平方: ";
    for (int n : v2 | evens | square) std::cout << n << " ";
    std::cout << "\n";

    // ---- 将视图结果收集到容器 ----
    std::cout << "\n-- 收集到容器 --\n";
    auto view_result = nums
        | std::views::filter([](int n) { return n > 5; })
        | std::views::transform([](int n) { return n * 2; });

    // 方法1：使用 ranges::copy
    std::vector<int> collected;
    std::ranges::copy(view_result, std::back_inserter(collected));
    std::cout << "收集结果: ";
    for (int n : collected) std::cout << n << " ";
    std::cout << "\n";

    // 方法2：使用构造函数 + ranges::to (C++23) 或手动
    std::vector<int> collected2(std::ranges::begin(view_result),
                                 std::ranges::end(view_result));
    std::cout << "构造收集: ";
    for (int n : collected2) std::cout << n << " ";
    std::cout << "\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：视图不拥有数据
    std::cout << "1. 视图不拥有数据，底层数据必须存活:\n";
    std::cout << "   auto bad_view = get_temp_vec() | views::filter(...);\n";
    std::cout << "   // 临时 vector 已销毁，视图悬空!\n\n";

    // 陷阱2：视图是惰性的，每次迭代都重新计算
    std::cout << "2. 视图每次迭代都重新计算:\n";
    std::vector<int> v = {1, 2, 3};
    int call_count = 0;
    auto counted_view = v | std::views::transform([&call_count](int n) {
        ++call_count;
        return n * 2;
    });

    call_count = 0;
    for ([[maybe_unused]] int n : counted_view) {}  // 第一次迭代
    std::cout << "   第一次迭代调用次数: " << call_count << "\n";
    for ([[maybe_unused]] int n : counted_view) {}  // 第二次迭代
    std::cout << "   第二次迭代调用次数: " << call_count << "\n";
    std::cout << "   (每次迭代都重新调用 transform)\n\n";

    // 陷阱3：filter 视图修改元素要小心
    std::cout << "3. 通过 filter 视图修改元素时，不要改变过滤条件涉及的值:\n";
    std::cout << "   否则可能导致未定义行为（元素不再满足过滤条件）\n\n";

    // 陷阱4：views::reverse 需要 bidirectional_range
    std::cout << "4. 不是所有视图都支持所有操作:\n";
    std::cout << "   views::reverse 需要 bidirectional_range\n";
    std::cout << "   filter 之后的视图不一定是 random_access\n\n";

    // 陷阱5：管道操作符的顺序
    std::cout << "5. 管道操作符从左到右求值:\n";
    std::cout << "   data | filter(p) | take(3)   → 先过滤，再取前3个\n";
    std::cout << "   data | take(3) | filter(p)   → 先取前3个，再过滤\n";
    std::cout << "   结果可能完全不同!\n";

    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    std::cout << "   原数据: 1 2 3 4 5 6\n";
    std::cout << "   filter(偶数) | take(2): ";
    for (int n : data | std::views::filter([](int n) { return n % 2 == 0; })
                      | std::views::take(2))
        std::cout << n << " ";
    std::cout << "\n   take(2) | filter(偶数): ";
    for (int n : data | std::views::take(2)
                      | std::views::filter([](int n) { return n % 2 == 0; }))
        std::cout << n << " ";
    std::cout << "\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== Ranges 视图 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: Ranges 视图 (Views: filter/transform/take 等)](https://en.cppreference.com/w/cpp/ranges)
