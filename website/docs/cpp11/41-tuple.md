---
sidebar_position: 42
slug: 41-tuple
title: "std::tuple、std::tie、std::get"
description: "C++11 std::tuple、std::tie、std::get 教学示例"
tags: [cpp11, tuple]
---

# std::tuple、std::tie、std::get

C++11 标准引入的 std::tuple、std::tie、std::get 特性完整教学示例。

## 完整源码

```cpp title="cpp11/41_tuple.cpp"
/**
 * ============================================================
 * 特性名称: std::tuple、std::tie、std::get
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: std::tuple是一个固定大小的异构容器，可以存储不同类型的多个值。
 *           配合std::tie和std::get，提供了方便的创建、访问和解包操作。
 *
 * [为什么引入]: C++03中返回多个值通常需要定义struct或使用输出参数，
 *               tuple提供了轻量级的方式来组合和传递多个不同类型的值。
 *
 * [关键要点]:
 *   - std::tuple 可存储任意数量、任意类型的值
 *   - std::get<N> 通过编译期索引访问元素
 *   - std::tie 可以将 tuple 解包到多个变量中
 *   - std::make_tuple 自动推导类型创建 tuple
 *   - std::tuple_size 获取 tuple 元素个数
 *   - 常用于函数返回多个值的场景
 * ============================================================
 */

#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. 创建 tuple
    std::cout << "[1] 创建 tuple:\n";
    {
        // 直接构造
        std::tuple<int, double, std::string> t1(1, 3.14, "你好");

        // 使用 make_tuple（自动推导类型）
        auto t2 = std::make_tuple(42, 2.718, std::string("世界"));

        std::cout << "  t1 创建成功 (int, double, string)" << std::endl;
        std::cout << "  t2 创建成功 (auto推导)" << std::endl;
    }
    std::cout << std::endl;

    // 2. 用 std::get 访问元素
    std::cout << "[2] 用 std::get<N> 访问元素:\n";
    {
        auto t = std::make_tuple(100, 3.14, std::string("C++11"));

        std::cout << "  get<0>: " << std::get<0>(t) << std::endl;
        std::cout << "  get<1>: " << std::get<1>(t) << std::endl;
        std::cout << "  get<2>: " << std::get<2>(t) << std::endl;

        // get 也可以修改值
        std::get<0>(t) = 200;
        std::cout << "  修改后 get<0>: " << std::get<0>(t) << std::endl;
    }
    std::cout << std::endl;

    // 3. 用 std::tie 解包
    std::cout << "[3] 用 std::tie 解包 tuple:\n";
    {
        auto t = std::make_tuple(1, 2.5, std::string("解包测试"));

        int a;
        double b;
        std::string c;
        std::tie(a, b, c) = t;

        std::cout << "  a = " << a << std::endl;
        std::cout << "  b = " << b << std::endl;
        std::cout << "  c = " << c << std::endl;

        // 使用 std::ignore 忽略不需要的元素
        int x;
        std::string z;
        std::tie(x, std::ignore, z) = t;
        std::cout << "  忽略中间值: x=" << x << ", z=" << z << std::endl;
    }
    std::cout << std::endl;

    // 4. tuple_size 获取元素个数
    std::cout << "[4] tuple_size 获取元素个数:\n";
    {
        using MyTuple = std::tuple<int, double, std::string, char>;
        std::cout << "  tuple<int, double, string, char> 的大小: "
                  << std::tuple_size<MyTuple>::value << std::endl;
    }
    std::cout << std::endl;

    // 5. 用作函数多返回值
    std::cout << "[5] tuple 用作函数多返回值:\n";
    {
        // 定义一个返回多个值的lambda
        auto divmod = [](int a, int b) -> std::tuple<int, int, bool> {
            if (b == 0) return std::make_tuple(0, 0, false);
            return std::make_tuple(a / b, a % b, true);
        };

        int quotient, remainder;
        bool success;
        std::tie(quotient, remainder, success) = divmod(17, 5);

        if (success) {
            std::cout << "  17 / 5 = " << quotient
                      << " 余 " << remainder << std::endl;
        }

        std::tie(quotient, remainder, success) = divmod(10, 0);
        std::cout << "  10 / 0 成功: " << (success ? "是" : "否") << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. tuple 比较运算
    std::cout << "[1] tuple 的比较运算（字典序）:\n";
    {
        auto t1 = std::make_tuple(1, 2, 3);
        auto t2 = std::make_tuple(1, 2, 4);
        auto t3 = std::make_tuple(1, 2, 3);

        std::cout << "  (1,2,3) == (1,2,3): " << (t1 == t3 ? "是" : "否") << std::endl;
        std::cout << "  (1,2,3) <  (1,2,4): " << (t1 < t2 ? "是" : "否") << std::endl;
        std::cout << "  (1,2,4) >  (1,2,3): " << (t2 > t1 ? "是" : "否") << std::endl;
    }
    std::cout << std::endl;

    // 2. tuple_cat 连接多个 tuple
    std::cout << "[2] tuple_cat 连接 tuple:\n";
    {
        auto t1 = std::make_tuple(1, 2.0);
        auto t2 = std::make_tuple(std::string("ABC"), 'x');
        auto t3 = std::tuple_cat(t1, t2);

        std::cout << "  连接后: ("
                  << std::get<0>(t3) << ", "
                  << std::get<1>(t3) << ", "
                  << std::get<2>(t3) << ", "
                  << std::get<3>(t3) << ")" << std::endl;
        std::cout << "  大小: " << std::tuple_size<decltype(t3)>::value << std::endl;
    }
    std::cout << std::endl;

    // 3. 利用 tie 实现结构体比较
    std::cout << "[3] 利用 tie 实现结构体的比较运算:\n";
    {
        struct Student {
            std::string name;
            int age;
            double score;

            bool operator<(const Student& other) const {
                // 用 tie 实现字典序比较：先比分数(降序)，再比年龄，最后比名字
                return std::tie(score, age, name) >
                       std::tie(other.score, other.age, other.name);
            }
        };

        std::vector<Student> students = {
            {"张三", 20, 85.5},
            {"李四", 19, 92.0},
            {"王五", 20, 92.0},
            {"赵六", 21, 78.0}
        };

        std::sort(students.begin(), students.end());

        std::cout << "  按分数降序排名:\n";
        for (const auto& s : students) {
            std::cout << "    " << s.name
                      << " 年龄:" << s.age
                      << " 分数:" << s.score << std::endl;
        }
    }
    std::cout << std::endl;

    // 4. tuple_element 获取元素类型
    std::cout << "[4] tuple_element 获取元素类型:\n";
    {
        using MyTuple = std::tuple<int, double, std::string>;

        // tuple_element 在编译期获取类型
        std::tuple_element<0, MyTuple>::type a = 42;
        std::tuple_element<1, MyTuple>::type b = 3.14;
        std::tuple_element<2, MyTuple>::type c = "类型萃取";

        std::cout << "  a (int): " << a << std::endl;
        std::cout << "  b (double): " << b << std::endl;
        std::cout << "  c (string): " << c << std::endl;
    }
    std::cout << std::endl;

    // 5. forward_as_tuple 转发引用
    std::cout << "[5] forward_as_tuple 转发引用构造:\n";
    {
        std::string name = "原始字符串";
        // forward_as_tuple 创建引用的 tuple，不拷贝
        auto t = std::forward_as_tuple(name, 42);
        std::get<0>(t) = "已修改";
        std::cout << "  通过 tuple 引用修改: name = " << name << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. get 的索引必须是编译期常量
    std::cout << "[1] get<N> 的 N 必须是编译期常量:\n";
    std::cout << "  // int i = 0;\n";
    std::cout << "  // std::get<i>(t);  // 编译错误！i不是编译期常量\n";
    std::cout << "  // 正确: std::get<0>(t);  // 必须是字面量或constexpr\n\n";

    // 2. tie 解包时类型必须匹配
    std::cout << "[2] tie 解包时注意类型匹配:\n";
    {
        auto t = std::make_tuple(1, std::string("hello"));
        int a;
        std::string b;
        std::tie(a, b) = t;  // 正确
        std::cout << "  正确解包: a=" << a << ", b=" << b << std::endl;
        // double x;
        // std::tie(x, b) = t;  // 可以编译但可能精度损失
    }
    std::cout << std::endl;

    // 3. make_tuple 与引用
    std::cout << "[3] make_tuple 默认按值存储（不是引用）:\n";
    {
        int x = 10;
        auto t = std::make_tuple(x);       // 存的是x的副本
        std::get<0>(t) = 20;
        std::cout << "  make_tuple 后修改 tuple: x=" << x
                  << " (未改变，因为是副本)" << std::endl;

        // 如需引用，用 std::ref
        auto t2 = std::make_tuple(std::ref(x));
        std::get<0>(t2) = 30;
        std::cout << "  用 ref 后修改 tuple:   x=" << x
                  << " (已改变，因为是引用)" << std::endl;
    }
    std::cout << std::endl;

    // 4. 空 tuple
    std::cout << "[4] 空 tuple 是合法的:\n";
    {
        std::tuple<> empty;
        std::cout << "  空 tuple 大小: "
                  << std::tuple_size<std::tuple<>>::value << std::endl;
        (void)empty;
    }
    std::cout << std::endl;

    // 5. tuple 不支持迭代
    std::cout << "[5] tuple 不支持运行时迭代:\n";
    std::cout << "  // for (auto& elem : t) {}  // 编译错误！\n";
    std::cout << "  // tuple 元素类型不同，无法用统一的迭代器遍历\n";
    std::cout << "  // 需要逐个 get<N> 或使用模板递归/C++17 fold expression\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::tuple / std::tie / std::get - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::tuple、std::tie、std::get](https://en.cppreference.com/w/cpp/utility/tuple)
