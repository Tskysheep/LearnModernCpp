---
sidebar_position: 56
title: "并行执行策略 (Parallel Execution Policies)"
description: "C++17 并行执行策略 (Parallel Execution Policies) 教学示例"
tags: [cpp17, parallel_algorithms]
---

# 并行执行策略 (Parallel Execution Policies)

C++17 标准引入的 并行执行策略 (Parallel Execution Policies) 特性完整教学示例。

## 完整源码

```cpp title="cpp17/55_parallel_algorithms.cpp"
/**
 * ============================================================
 * 特性名称: 并行执行策略 (Parallel Execution Policies)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: C++17 为 <algorithm> 和 <numeric> 中的算法添加了
 *          执行策略参数，允许算法以顺序、并行或向量化方式执行。
 *          三种策略：seq（顺序）、par（并行）、par_unseq（并行+向量化）。
 *
 * [为什么引入]: 现代 CPU 都是多核的，但标准算法默认是单线程的。
 *              并行执行策略让标准算法自动利用多核，无需手动管理线程。
 *
 * [关键要点]:
 *   - std::execution::seq —— 顺序执行（与无策略版本相同）
 *   - std::execution::par —— 并行执行（多线程）
 *   - std::execution::par_unseq —— 并行+向量化（最激进）
 *   - 注意：MinGW/GCC 可能不完全支持，MSVC 支持较好
 *   - 并行算法中不能有数据竞争，需要用原子操作或互斥锁
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <cmath>

// 检测并行算法支持
#if defined(__cpp_lib_parallel_algorithm)
    #include <execution>
    #define HAS_PARALLEL 1
#elif defined(_MSC_VER)
    // MSVC 可能不定义宏但支持
    #include <execution>
    #define HAS_PARALLEL 1
#else
    #define HAS_PARALLEL 0
#endif

// ============================================================
// 计时辅助
// ============================================================
class Timer {
public:
    Timer(const std::string& name) : name_(name), start_(std::chrono::high_resolution_clock::now()) {}
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start_).count();
        std::cout << "  [" << name_ << "] 耗时: " << ms << " 微秒\n";
    }
private:
    std::string name_;
    std::chrono::high_resolution_clock::time_point start_;
};

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

#if HAS_PARALLEL
    std::cout << "并行算法可用!\n\n";

    // 准备数据
    const size_t N = 1'000'000;
    std::vector<int> data(N);
    std::iota(data.begin(), data.end(), 1);  // 1, 2, 3, ..., N

    // 1. std::sort 的三种执行策略
    std::cout << "排序 " << N << " 个元素:\n";
    {
        auto v = data;
        std::shuffle(v.begin(), v.end(), std::mt19937{42});
        Timer t("seq 顺序排序");
        std::sort(std::execution::seq, v.begin(), v.end());
    }
    {
        auto v = data;
        std::shuffle(v.begin(), v.end(), std::mt19937{42});
        Timer t("par 并行排序");
        std::sort(std::execution::par, v.begin(), v.end());
    }
    {
        auto v = data;
        std::shuffle(v.begin(), v.end(), std::mt19937{42});
        Timer t("par_unseq 并行+向量化排序");
        std::sort(std::execution::par_unseq, v.begin(), v.end());
    }

    // 2. std::for_each —— 并行遍历
    std::cout << "\nfor_each 并行计算:\n";
    std::vector<double> results(N);
    {
        Timer t("seq for_each");
        std::for_each(std::execution::seq, data.begin(), data.end(),
            [&results](int i) { results[i-1] = std::sqrt(static_cast<double>(i)); });
    }
    {
        Timer t("par for_each");
        std::for_each(std::execution::par, data.begin(), data.end(),
            [&results](int i) { results[i-1] = std::sqrt(static_cast<double>(i)); });
    }

    // 3. std::transform —— 并行转换
    std::cout << "\ntransform 并行转换:\n";
    std::vector<double> output(N);
    {
        Timer t("seq transform");
        std::transform(std::execution::seq, data.begin(), data.end(),
                       output.begin(), [](int x) { return x * x * 0.001; });
    }
    {
        Timer t("par transform");
        std::transform(std::execution::par, data.begin(), data.end(),
                       output.begin(), [](int x) { return x * x * 0.001; });
    }

    // 4. std::reduce —— 并行归约（替代 accumulate）
    std::cout << "\nreduce 并行求和:\n";
    {
        Timer t("seq accumulate");
        auto sum = std::accumulate(data.begin(), data.end(), 0LL);
        std::cout << "  顺序求和 = " << sum << "\n";
    }
    {
        Timer t("par reduce");
        auto sum = std::reduce(std::execution::par, data.begin(), data.end(), 0LL);
        std::cout << "  并行求和 = " << sum << "\n";
    }

#else
    std::cout << "并行算法不可用 (编译器不支持)\n";
    std::cout << "MSVC 通常支持，GCC 需要 TBB，MinGW 可能不支持\n\n";

    // 提供非并行版本的演示
    const size_t N = 100'000;
    std::vector<int> data(N);
    std::iota(data.begin(), data.end(), 1);

    // 普通排序
    {
        auto v = data;
        std::shuffle(v.begin(), v.end(), std::mt19937{42});
        Timer t("普通排序");
        std::sort(v.begin(), v.end());
    }

    // 普通求和
    {
        Timer t("普通求和");
        auto sum = std::accumulate(data.begin(), data.end(), 0LL);
        std::cout << "  求和 = " << sum << "\n";
    }
#endif
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

#if HAS_PARALLEL
    const size_t N = 500'000;
    std::vector<int> data(N);
    std::iota(data.begin(), data.end(), 1);

    // 1. std::count_if —— 并行计数
    std::cout << "count_if 统计偶数:\n";
    {
        Timer t("par count_if");
        auto count = std::count_if(std::execution::par,
            data.begin(), data.end(),
            [](int x) { return x % 2 == 0; });
        std::cout << "  偶数个数: " << count << "\n";
    }

    // 2. std::find_if —— 注意：并行 find 不保证找到第一个
    //    但标准要求返回第一个匹配的元素
    std::cout << "\nfind_if 查找:\n";
    {
        Timer t("par find_if");
        auto it = std::find_if(std::execution::par,
            data.begin(), data.end(),
            [](int x) { return x > 400000 && x % 7 == 0; });
        if (it != data.end()) {
            std::cout << "  找到: " << *it << "\n";
        }
    }

    // 3. std::all_of / any_of / none_of 并行版
    {
        Timer t("par all_of");
        bool all_positive = std::all_of(std::execution::par,
            data.begin(), data.end(),
            [](int x) { return x > 0; });
        std::cout << "\n全部为正: " << all_positive << "\n";
    }

    // 4. std::transform_reduce —— 并行 map-reduce
    std::cout << "\ntransform_reduce (点积):\n";
    std::vector<double> v1(N, 1.0), v2(N, 2.0);
    {
        Timer t("par transform_reduce");
        double dot = std::transform_reduce(std::execution::par,
            v1.begin(), v1.end(), v2.begin(), 0.0);
        std::cout << "  点积 = " << dot << "\n";
    }

    // 5. std::exclusive_scan —— 并行前缀和
    std::cout << "\nexclusive_scan (前缀和):\n";
    std::vector<int> small_data{1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> prefix_sum(small_data.size());
    std::exclusive_scan(std::execution::par,
        small_data.begin(), small_data.end(),
        prefix_sum.begin(), 0);
    std::cout << "  输入: ";
    for (int x : small_data) std::cout << x << " ";
    std::cout << "\n  前缀和: ";
    for (int x : prefix_sum) std::cout << x << " ";
    std::cout << "\n";

#else
    std::cout << "并行算法不可用，跳过进阶演示\n";

    // 展示等价的顺序版本
    std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto count = std::count_if(data.begin(), data.end(),
        [](int x) { return x % 2 == 0; });
    std::cout << "偶数个数 (顺序): " << count << "\n";

    // 手动求和（等价于 transform_reduce）
    double sum = 0;
    for (auto x : data) sum += x * x;
    std::cout << "平方和 (顺序): " << sum << "\n";
#endif
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: 数据竞争
    //   并行算法中的 lambda 不能有数据竞争
    std::cout << "陷阱1 - 并行 lambda 中不能有数据竞争\n";
    std::cout << "  错误: int sum=0; par for_each([&sum](int x){sum+=x;});\n";
    std::cout << "  正确: 使用 reduce/transform_reduce 或 atomic\n";

#if HAS_PARALLEL
    // 演示正确做法
    std::vector<int> data{1, 2, 3, 4, 5};
    // 错误: 数据竞争
    // int bad_sum = 0;
    // std::for_each(std::execution::par, data.begin(), data.end(),
    //     [&bad_sum](int x) { bad_sum += x; });  // 数据竞争！

    // 正确: 使用 reduce
    auto good_sum = std::reduce(std::execution::par, data.begin(), data.end(), 0);
    std::cout << "  正确求和: " << good_sum << "\n";
#endif

    // 陷阱 2: 并行算法的回调不能抛异常（par_unseq）
    std::cout << "\n陷阱2 - par_unseq 中的操作不能抛异常\n";
    std::cout << "  par_unseq 可能在单线程中交错执行，异常会导致 UB\n";

    // 陷阱 3: 小数据量不值得并行
    std::cout << "\n陷阱3 - 数据量小时并行开销可能超过收益\n";
    std::cout << "  并行有线程创建/同步开销，小数据用 seq 更快\n";

    // 陷阱 4: reduce vs accumulate
    //   reduce 要求运算可交换可结合（因为并行时顺序不确定）
    //   accumulate 保证从左到右顺序执行
    std::cout << "\n陷阱4 - reduce 要求运算可交换可结合\n";
    std::cout << "  整数加法 OK，浮点加法有精度差异，字符串拼接不行\n";

    // 陷阱 5: 编译器支持差异
    std::cout << "\n陷阱5 - 编译器支持差异:\n";
    std::cout << "  MSVC: 完整支持\n";
    std::cout << "  GCC:  需要链接 TBB (-ltbb)\n";
    std::cout << "  Clang: 部分支持\n";
    std::cout << "  MinGW: 可能不支持\n";

#ifdef __cpp_lib_parallel_algorithm
    std::cout << "  当前: __cpp_lib_parallel_algorithm = "
              << __cpp_lib_parallel_algorithm << "\n";
#else
    std::cout << "  当前: __cpp_lib_parallel_algorithm 未定义\n";
#endif

    // 陷阱 6: 并行版本的迭代器要求
    //   并行算法通常要求 RandomAccessIterator
    //   list/set 等容器的迭代器不满足要求
    std::cout << "\n陷阱6 - 并行算法通常要求随机访问迭代器\n";
    std::cout << "  vector/array/deque OK, list/set/map 不行\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 并行执行策略 (Parallel Algorithms) - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 并行执行策略 (Parallel Execution Policies)](https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag)
