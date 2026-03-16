---
sidebar_position: 82
title: "⟨random⟩ 随机数库"
description: "C++11 ⟨random⟩ 随机数库 教学示例"
tags: [cpp11, random]
---

# `<random>` 随机数库

C++11 标准引入的 `<random>` 随机数库 特性完整教学示例。

## 完整源码

```cpp title="cpp11/81_random.cpp"
/**
 * ============================================================
 * 特性名称: <random> 随机数库
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: C++11提供了现代的随机数生成库，包含随机数引擎（生成伪随机序列）
 *           和分布（将随机序列映射为所需的概率分布），替代C语言的 rand()。
 *
 * [为什么引入]: C语言的 rand() 质量低、不可移植、缺乏分布支持、
 *               RAND_MAX 在不同平台不同。新的 <random> 库提供了高质量、
 *               可控的随机数生成机制。
 *
 * [关键要点]:
 *   - 引擎(Engine): 生成伪随机整数序列，如 mt19937、default_random_engine
 *   - 分布(Distribution): 将引擎输出映射为目标分布，如 uniform、normal
 *   - 种子(Seed): 初始化引擎，同一种子产生同一序列
 *   - std::random_device: 真随机数（如果硬件支持），常用于提供种子
 *   - 引擎和分布是分离的，可以自由组合
 * ============================================================
 */

#include <iostream>
#include <random>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <functional>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. 最基本的使用模式
    std::cout << "[1] 基本使用模式（引擎 + 分布）:\n";
    {
        // 1. 创建随机设备（真随机源）
        std::random_device rd;

        // 2. 用随机设备初始化伪随机引擎
        std::mt19937 gen(rd());

        // 3. 创建分布
        std::uniform_int_distribution<int> dist(1, 6);  // [1, 6] 均匀分布

        // 4. 生成随机数
        std::cout << "  掷骰子6次: ";
        for (int i = 0; i < 6; ++i) {
            std::cout << dist(gen) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 2. 常用引擎
    std::cout << "[2] 常用随机引擎:\n";
    {
        std::random_device rd;

        // mt19937: 梅森旋转算法，周期 2^19937-1，最常用
        std::mt19937 mt(rd());
        std::cout << "  mt19937:      " << mt() << std::endl;

        // mt19937_64: 64位版本
        std::mt19937_64 mt64(rd());
        std::cout << "  mt19937_64:   " << mt64() << std::endl;

        // default_random_engine: 实现定义的默认引擎
        std::default_random_engine dre(rd());
        std::cout << "  default_engine: " << dre() << std::endl;

        // minstd_rand: 线性同余引擎，速度快但质量一般
        std::minstd_rand lcg(rd());
        std::cout << "  minstd_rand:  " << lcg() << std::endl;
    }
    std::cout << std::endl;

    // 3. 均匀分布
    std::cout << "[3] 均匀分布 (uniform_distribution):\n";
    {
        std::mt19937 gen(42);  // 固定种子，可重现

        // 整数均匀分布 [a, b]
        std::uniform_int_distribution<int> int_dist(1, 100);
        std::cout << "  整数均匀 [1,100]: ";
        for (int i = 0; i < 8; ++i) std::cout << int_dist(gen) << " ";
        std::cout << std::endl;

        // 实数均匀分布 [a, b)
        std::uniform_real_distribution<double> real_dist(0.0, 1.0);
        std::cout << "  实数均匀 [0,1): ";
        for (int i = 0; i < 5; ++i) {
            std::cout << std::fixed << std::setprecision(3)
                      << real_dist(gen) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 4. 正态分布
    std::cout << "[4] 正态分布 (normal_distribution):\n";
    {
        std::mt19937 gen(42);
        std::normal_distribution<double> dist(100.0, 15.0);  // 均值100, 标准差15

        std::cout << "  正态分布(均值100, 标准差15): ";
        for (int i = 0; i < 8; ++i) {
            std::cout << std::fixed << std::setprecision(1)
                      << dist(gen) << " ";
        }
        std::cout << std::endl;

        // 统计验证
        const int N = 100000;
        double sum = 0;
        for (int i = 0; i < N; ++i) sum += dist(gen);
        std::cout << "  " << N << "个样本的平均值: "
                  << std::setprecision(2) << sum / N
                  << " (期望100.0)" << std::endl;
    }
    std::cout << std::endl;

    // 5. 伯努利分布
    std::cout << "[5] 伯努利分布 (bernoulli_distribution):\n";
    {
        std::mt19937 gen(42);
        std::bernoulli_distribution coin(0.5);  // 50%概率为true

        int heads = 0, tails = 0;
        for (int i = 0; i < 1000; ++i) {
            if (coin(gen)) ++heads;
            else ++tails;
        }
        std::cout << "  抛硬币1000次: 正面=" << heads
                  << ", 反面=" << tails << std::endl;

        // 偏置硬币
        std::bernoulli_distribution biased(0.3);  // 30%概率为true
        heads = 0;
        for (int i = 0; i < 1000; ++i) {
            if (biased(gen)) ++heads;
        }
        std::cout << "  偏置硬币(30%): 正面=" << heads << "/1000" << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. 种子与可重现性
    std::cout << "[1] 种子与可重现性:\n";
    {
        // 相同种子 -> 相同序列
        std::mt19937 gen1(12345);
        std::mt19937 gen2(12345);

        std::uniform_int_distribution<int> dist(1, 100);

        std::cout << "  gen1(种子12345): ";
        for (int i = 0; i < 5; ++i) std::cout << dist(gen1) << " ";
        std::cout << std::endl;

        std::cout << "  gen2(种子12345): ";
        for (int i = 0; i < 5; ++i) std::cout << dist(gen2) << " ";
        std::cout << std::endl;
        std::cout << "  (两者完全相同，可用于调试和测试)\n";
    }
    std::cout << std::endl;

    // 2. 更多分布类型
    std::cout << "[2] 其他分布类型:\n";
    {
        std::mt19937 gen(42);

        // 泊松分布
        std::poisson_distribution<int> poisson(4.0);  // 均值4
        std::cout << "  泊松分布(均值4): ";
        for (int i = 0; i < 10; ++i) std::cout << poisson(gen) << " ";
        std::cout << std::endl;

        // 指数分布
        std::exponential_distribution<double> exp_dist(1.0);
        std::cout << "  指数分布(lambda=1): ";
        for (int i = 0; i < 5; ++i) {
            std::cout << std::fixed << std::setprecision(2) << exp_dist(gen) << " ";
        }
        std::cout << std::endl;

        // 离散分布（加权随机）
        std::discrete_distribution<int> weighted({1, 2, 3, 4});
        // 权重: 索引0=1, 索引1=2, 索引2=3, 索引3=4
        std::map<int, int> counts;
        for (int i = 0; i < 10000; ++i) ++counts[weighted(gen)];
        std::cout << "  离散分布(权重1:2:3:4): ";
        for (auto& p : counts) {
            std::cout << p.first << ":" << p.second << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 3. 分布直方图可视化
    std::cout << "[3] 正态分布直方图:\n";
    {
        std::mt19937 gen(42);
        std::normal_distribution<double> dist(50.0, 10.0);

        std::map<int, int> histogram;
        for (int i = 0; i < 10000; ++i) {
            int val = static_cast<int>(std::round(dist(gen)));
            int bucket = (val / 5) * 5;  // 按5分组
            ++histogram[bucket];
        }

        for (auto& p : histogram) {
            if (p.first >= 20 && p.first <= 80) {
                std::cout << "  " << std::setw(3) << p.first << " | ";
                int bar = p.second / 50;
                for (int i = 0; i < bar; ++i) std::cout << "*";
                std::cout << std::endl;
            }
        }
    }
    std::cout << std::endl;

    // 4. 与 rand() 对比
    std::cout << "[4] 与 C语言 rand() 对比:\n";
    {
        std::cout << "  rand() 的问题:\n";
        std::cout << "    - 周期短 (通常 2^32)\n";
        std::cout << "    - 低位随机性差\n";
        std::cout << "    - 全局状态，不线程安全\n";
        std::cout << "    - 不同平台结果不同\n";
        std::cout << "    - rand() % N 分布不均匀\n\n";

        std::cout << "  <random> 的优势:\n";
        std::cout << "    - mt19937 周期 2^19937-1\n";
        std::cout << "    - 统计性质优秀\n";
        std::cout << "    - 每个引擎独立，线程安全\n";
        std::cout << "    - 可移植（同种子同结果）\n";
        std::cout << "    - 多种分布可选\n";

        // rand() % 6 的不均匀性演示
        std::cout << "\n  rand() % 6 vs uniform_int_distribution:\n";
        // 这里不实际调用 rand() 避免影响全局状态
        std::cout << "    当 RAND_MAX 不是6的倍数时，rand()%6 分布不均\n";
        std::cout << "    uniform_int_distribution 始终均匀\n";
    }
    std::cout << std::endl;

    // 5. 随机洗牌和采样
    std::cout << "[5] 随机洗牌 (shuffle):\n";
    {
        std::mt19937 gen(42);

        std::vector<int> cards = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::cout << "  原始: ";
        for (int c : cards) std::cout << c << " ";
        std::cout << std::endl;

        std::shuffle(cards.begin(), cards.end(), gen);
        std::cout << "  洗牌: ";
        for (int c : cards) std::cout << c << " ";
        std::cout << std::endl;

        std::shuffle(cards.begin(), cards.end(), gen);
        std::cout << "  再洗: ";
        for (int c : cards) std::cout << c << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. 不要每次调用都创建引擎
    std::cout << "[1] 不要每次调用都创建新引擎:\n";
    std::cout << "  // 错误:\n";
    std::cout << "  // int roll() {\n";
    std::cout << "  //     std::mt19937 gen(rd());  // 每次创建！\n";
    std::cout << "  //     return dist(gen);\n";
    std::cout << "  // }\n";
    std::cout << "  // 正确: 引擎应该创建一次，重复使用\n";
    std::cout << "  // static std::mt19937 gen(rd());  // 只创建一次\n\n";

    // 2. random_device 的注意事项
    std::cout << "[2] random_device 的注意事项:\n";
    std::cout << "  - 某些实现中 random_device 是确定性的\n";
    std::cout << "  - random_device 通常很慢，只用于生成种子\n";
    std::cout << "  - 不要直接用 random_device 生成大量随机数\n\n";

    // 3. 分布对象有状态
    std::cout << "[3] 分布对象可能有内部状态:\n";
    std::cout << "  某些分布会缓存值（如 normal_distribution 一次生成两个值）\n";
    std::cout << "  重置状态: dist.reset();\n\n";

    // 4. 线程安全
    std::cout << "[4] 引擎不是线程安全的:\n";
    std::cout << "  每个线程应有自己的引擎实例\n";
    std::cout << "  // thread_local std::mt19937 gen(rd());\n\n";

    // 5. uniform_int_distribution 的范围
    std::cout << "[5] 注意分布的范围定义:\n";
    {
        std::mt19937 gen(42);

        // uniform_int_distribution: [a, b] 闭区间
        std::uniform_int_distribution<int> int_dist(1, 6);
        std::cout << "  uniform_int_distribution(1, 6): 范围 [1, 6] 闭区间\n";

        // uniform_real_distribution: [a, b) 半开区间
        std::uniform_real_distribution<double> real_dist(0.0, 1.0);
        std::cout << "  uniform_real_distribution(0, 1): 范围 [0, 1) 半开区间\n";

        std::cout << "  注意: 整数分布包含两端，实数分布不包含右端\n";
    }
    std::cout << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== <random> 随机数库 - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: `<random>` 随机数库](https://en.cppreference.com/w/cpp/numeric/random)
