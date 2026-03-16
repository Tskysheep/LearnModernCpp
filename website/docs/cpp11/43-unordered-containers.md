---
sidebar_position: 44
title: "无序容器 (unordered_map / unordered_set)"
description: "C++11 无序容器 (unordered_map / unordered_set) 教学示例"
tags: [cpp11, unordered_containers]
---

# 无序容器 (unordered_map / unordered_set)

C++11 标准引入的 无序容器 (unordered_map / unordered_set) 特性完整教学示例。

## 完整源码

```cpp title="cpp11/43_unordered_containers.cpp"
/**
 * ============================================================
 * 特性名称: 无序容器 (unordered_map / unordered_set)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: 基于哈希表实现的关联容器，提供平均O(1)的查找、插入和删除操作。
 *           包括 unordered_map、unordered_set、unordered_multimap、unordered_multiset。
 *
 * [为什么引入]: std::map/set基于红黑树，操作为O(log n)。在不需要有序遍历时，
 *               哈希容器的平均O(1)性能更优，尤其适合大量查找操作的场景。
 *
 * [关键要点]:
 *   - 基于哈希表，平均O(1)查找/插入/删除（最坏O(n)）
 *   - 元素无序存储，不保证遍历顺序
 *   - 支持自定义哈希函数和相等比较函数
 *   - 提供桶(bucket)操作接口，可观察内部哈希结构
 *   - 对于不需要有序的场景，性能通常优于 map/set
 * ============================================================
 */

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <functional>
#include <chrono>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. unordered_map 基本操作
    std::cout << "[1] unordered_map 基本操作:\n";
    {
        std::unordered_map<std::string, int> scores;

        // 插入
        scores["张三"] = 95;
        scores["李四"] = 88;
        scores.insert({"王五", 92});
        scores.emplace("赵六", 76);

        // 访问
        std::cout << "  张三的分数: " << scores["张三"] << std::endl;
        std::cout << "  元素个数: " << scores.size() << std::endl;

        // 查找
        auto it = scores.find("李四");
        if (it != scores.end()) {
            std::cout << "  找到 李四: " << it->second << std::endl;
        }

        // count（0或1）
        std::cout << "  包含 '王五': " << scores.count("王五") << std::endl;
        std::cout << "  包含 '陈七': " << scores.count("陈七") << std::endl;

        // 遍历（注意：顺序不确定）
        std::cout << "  遍历（顺序不确定）:\n";
        for (const auto& pair : scores) {
            std::cout << "    " << pair.first << ": " << pair.second << std::endl;
        }

        // 删除
        scores.erase("赵六");
        std::cout << "  删除赵六后大小: " << scores.size() << std::endl;
    }
    std::cout << std::endl;

    // 2. unordered_set 基本操作
    std::cout << "[2] unordered_set 基本操作:\n";
    {
        std::unordered_set<int> numbers = {5, 3, 1, 4, 2, 5, 3};  // 重复元素自动去除

        std::cout << "  元素个数(去重后): " << numbers.size() << std::endl;
        std::cout << "  包含 3: " << (numbers.count(3) ? "是" : "否") << std::endl;
        std::cout << "  包含 6: " << (numbers.count(6) ? "是" : "否") << std::endl;

        numbers.insert(6);
        numbers.insert(6);  // 重复插入无效
        std::cout << "  插入6两次后大小: " << numbers.size() << std::endl;

        std::cout << "  遍历: ";
        for (int n : numbers) std::cout << n << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 3. unordered_multimap（允许重复键）
    std::cout << "[3] unordered_multimap（允许重复键）:\n";
    {
        std::unordered_multimap<std::string, std::string> phonebook;
        phonebook.insert({"张三", "13800001111"});
        phonebook.insert({"张三", "13800002222"});  // 同一个键，不同值
        phonebook.insert({"李四", "13900003333"});

        std::cout << "  张三的电话数量: " << phonebook.count("张三") << std::endl;
        auto range = phonebook.equal_range("张三");
        for (auto it = range.first; it != range.second; ++it) {
            std::cout << "    " << it->first << ": " << it->second << std::endl;
        }
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. 自定义哈希函数
    std::cout << "[1] 自定义哈希函数:\n";
    {
        struct Point {
            int x, y;
            bool operator==(const Point& other) const {
                return x == other.x && y == other.y;
            }
        };

        // 自定义哈希函数
        struct PointHash {
            std::size_t operator()(const Point& p) const {
                auto h1 = std::hash<int>{}(p.x);
                auto h2 = std::hash<int>{}(p.y);
                return h1 ^ (h2 << 1);  // 简单的哈希组合
            }
        };

        std::unordered_set<Point, PointHash> points;
        points.insert({1, 2});
        points.insert({3, 4});
        points.insert({1, 2});  // 重复，不会插入

        std::cout << "  Point集合大小: " << points.size() << std::endl;
        for (const auto& p : points) {
            std::cout << "    (" << p.x << ", " << p.y << ")" << std::endl;
        }

        // 也可以用 lambda 作为哈希函数
        auto lambdaHash = [](const Point& p) -> std::size_t {
            return std::hash<int>{}(p.x) * 31 + std::hash<int>{}(p.y);
        };
        std::unordered_set<Point, decltype(lambdaHash)> points2(10, lambdaHash);
        points2.insert({5, 6});
        std::cout << "  lambda哈希集合大小: " << points2.size() << std::endl;
    }
    std::cout << std::endl;

    // 2. 桶操作
    std::cout << "[2] 桶(bucket)操作:\n";
    {
        std::unordered_map<std::string, int> map;
        map["苹果"] = 1;
        map["香蕉"] = 2;
        map["橙子"] = 3;
        map["葡萄"] = 4;
        map["西瓜"] = 5;

        std::cout << "  桶数量: " << map.bucket_count() << std::endl;
        std::cout << "  元素数量: " << map.size() << std::endl;
        std::cout << "  负载因子: " << map.load_factor() << std::endl;
        std::cout << "  最大负载因子: " << map.max_load_factor() << std::endl;

        // 查看每个桶的内容
        std::cout << "  各桶情况:\n";
        for (size_t i = 0; i < map.bucket_count(); ++i) {
            if (map.bucket_size(i) > 0) {
                std::cout << "    桶[" << i << "] 大小="
                          << map.bucket_size(i) << ": ";
                for (auto it = map.begin(i); it != map.end(i); ++it) {
                    std::cout << it->first << " ";
                }
                std::cout << std::endl;
            }
        }

        // 查询特定键在哪个桶
        std::cout << "  '苹果' 在桶: " << map.bucket("苹果") << std::endl;

        // 手动调整桶数量
        map.rehash(20);
        std::cout << "  rehash(20)后桶数量: " << map.bucket_count() << std::endl;

        // reserve 预留空间
        map.reserve(100);
        std::cout << "  reserve(100)后桶数量: " << map.bucket_count() << std::endl;
    }
    std::cout << std::endl;

    // 3. 与有序容器性能对比
    std::cout << "[3] 与有序容器性能对比:\n";
    {
        const int N = 100000;

        // unordered_map 插入
        auto start = std::chrono::steady_clock::now();
        std::unordered_map<int, int> umap;
        umap.reserve(N);
        for (int i = 0; i < N; ++i) {
            umap[i] = i;
        }
        auto end = std::chrono::steady_clock::now();
        auto umap_insert = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // map 插入
        start = std::chrono::steady_clock::now();
        std::map<int, int> omap;
        for (int i = 0; i < N; ++i) {
            omap[i] = i;
        }
        end = std::chrono::steady_clock::now();
        auto omap_insert = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "  插入 " << N << " 个元素:\n";
        std::cout << "    unordered_map: " << umap_insert << " 微秒\n";
        std::cout << "    map:           " << omap_insert << " 微秒\n";

        // 查找性能对比
        start = std::chrono::steady_clock::now();
        volatile int sum1 = 0;
        for (int i = 0; i < N; ++i) {
            sum1 += umap[i];
        }
        end = std::chrono::steady_clock::now();
        auto umap_find = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        start = std::chrono::steady_clock::now();
        volatile int sum2 = 0;
        for (int i = 0; i < N; ++i) {
            sum2 += omap[i];
        }
        end = std::chrono::steady_clock::now();
        auto omap_find = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << "  查找 " << N << " 次:\n";
        std::cout << "    unordered_map: " << umap_find << " 微秒\n";
        std::cout << "    map:           " << omap_find << " 微秒\n";
        std::cout << "  (unordered 通常更快，但实际结果受缓存等因素影响)\n";
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. 元素顺序不确定
    std::cout << "[1] 遍历顺序不确定（不要依赖顺序）:\n";
    {
        std::unordered_map<int, std::string> map = {
            {1, "一"}, {2, "二"}, {3, "三"}, {4, "四"}
        };
        std::cout << "  遍历: ";
        for (const auto& p : map) {
            std::cout << p.first << " ";
        }
        std::cout << "\n  (顺序可能每次运行不同)\n";
    }
    std::cout << std::endl;

    // 2. operator[] 会插入默认值
    std::cout << "[2] operator[] 对不存在的键会插入默认值:\n";
    {
        std::unordered_map<std::string, int> map;
        map["存在"] = 42;

        // 访问不存在的键，会自动插入默认值0
        int val = map["不存在"];
        std::cout << "  访问不存在的键: " << val << " (自动插入了默认值0)" << std::endl;
        std::cout << "  此时 map 大小: " << map.size() << " (多了一个元素!)" << std::endl;
        std::cout << "  正确做法: 用 find() 或 count() 先检查\n";
    }
    std::cout << std::endl;

    // 3. 哈希冲突影响性能
    std::cout << "[3] 糟糕的哈希函数会导致性能退化:\n";
    std::cout << "  如果所有元素哈希到同一个桶，查找退化为O(n)\n";
    std::cout << "  应确保哈希函数分布均匀\n\n";

    // 4. 迭代器失效
    std::cout << "[4] rehash 会使所有迭代器失效:\n";
    std::cout << "  插入操作可能触发 rehash\n";
    std::cout << "  rehash 后所有迭代器、指针、引用都失效\n";
    std::cout << "  可以通过 reserve() 预留空间来避免\n\n";

    // 5. 自定义类型必须提供哈希和相等比较
    std::cout << "[5] 自定义类型必须提供哈希函数和 operator==:\n";
    std::cout << "  // struct MyType { int a, b; };\n";
    std::cout << "  // unordered_set<MyType> s;  // 编译错误！缺少哈希和==\n";
    std::cout << "  // 需要: 1) 定义 operator==  2) 定义哈希函数或特化 std::hash\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 无序容器 (unordered_map/set) - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 无序容器 (unordered_map / unordered_set)](https://en.cppreference.com/w/cpp/container/unordered_map)
