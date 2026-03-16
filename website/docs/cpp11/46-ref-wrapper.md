---
sidebar_position: 47
slug: 46-ref-wrapper
title: "std::ref、std::cref 与 reference_wrapper"
description: "C++11 std::ref、std::cref 与 reference_wrapper 教学示例"
tags: [cpp11, ref_wrapper]
---

# std::ref、std::cref 与 reference_wrapper

C++11 标准引入的 std::ref、std::cref 与 reference_wrapper 特性完整教学示例。

## 完整源码

```cpp title="cpp11/46_ref_wrapper.cpp"
/**
 * ============================================================
 * 特性名称: std::ref、std::cref 与 reference_wrapper
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: std::reference_wrapper 是对引用的包装，使引用可以被拷贝和存储。
 *           std::ref 和 std::cref 是创建 reference_wrapper 的辅助函数。
 *
 * [为什么引入]: C++的引用不能存储在容器中，也不能通过值传递给bind/thread等
 *               按值接收参数的函数。reference_wrapper 解决了这些问题。
 *
 * [关键要点]:
 *   - std::ref(x) 创建 reference_wrapper<T>，std::cref(x) 创建 const 版本
 *   - reference_wrapper 可以隐式转换为 T&
 *   - 用于 std::bind、std::thread 等需要按引用传递的场景
 *   - 可以在 vector 等容器中存储"引用"
 *   - 包装后的对象仍然可以像原始引用一样使用
 * ============================================================
 */

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <string>
#include <thread>
#include <numeric>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. std::ref 和 reference_wrapper 基础
    std::cout << "[1] std::ref 和 reference_wrapper 基础:\n";
    {
        int x = 10;
        std::reference_wrapper<int> ref = std::ref(x);

        std::cout << "  x = " << x << std::endl;
        std::cout << "  ref = " << ref << std::endl;  // 隐式转换为 int&

        // 通过 reference_wrapper 修改原始值
        ref.get() = 20;
        std::cout << "  通过ref修改后 x = " << x << std::endl;

        // 也可以隐式转换后赋值
        int& r = ref;
        r = 30;
        std::cout << "  通过隐式转换修改后 x = " << x << std::endl;
    }
    std::cout << std::endl;

    // 2. std::cref (const 引用包装)
    std::cout << "[2] std::cref (const 引用包装):\n";
    {
        int x = 42;
        auto cref = std::cref(x);

        std::cout << "  cref = " << cref.get() << std::endl;
        // cref.get() = 100;  // 编译错误！const引用不可修改

        x = 100;
        std::cout << "  修改x后 cref = " << cref.get() << " (跟随变化)" << std::endl;
    }
    std::cout << std::endl;

    // 3. 在 bind 中传引用
    std::cout << "[3] 在 std::bind 中传引用:\n";
    {
        auto increment = [](int& value) {
            ++value;
        };

        int x = 0;

        // 不用 ref：bind 按值拷贝，修改不影响原始变量
        auto fn1 = std::bind(increment, x);
        fn1();
        std::cout << "  不用ref, bind后调用: x = " << x << " (未改变)" << std::endl;

        // 用 ref：bind 持有引用，修改影响原始变量
        auto fn2 = std::bind(increment, std::ref(x));
        fn2();
        std::cout << "  用ref, bind后调用:   x = " << x << " (已改变)" << std::endl;

        fn2();
        fn2();
        std::cout << "  再调用两次:          x = " << x << std::endl;
    }
    std::cout << std::endl;

    // 4. reference_wrapper 的拷贝和赋值
    std::cout << "[4] reference_wrapper 可以拷贝和重新绑定:\n";
    {
        int a = 10, b = 20;
        auto ref = std::ref(a);

        std::cout << "  ref 绑定 a: " << ref.get() << std::endl;

        // 重新绑定到 b（不是修改a的值，而是ref指向b）
        ref = std::ref(b);
        std::cout << "  ref 重新绑定 b: " << ref.get() << std::endl;
        std::cout << "  a 的值: " << a << " (未改变)" << std::endl;

        // 拷贝
        auto ref2 = ref;
        ref2.get() = 30;
        std::cout << "  通过ref2修改后 b = " << b << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. 在容器中存储"引用"
    std::cout << "[1] 在容器中存储'引用':\n";
    {
        int a = 10, b = 20, c = 30;

        // vector<int&> 不合法！但 vector<reference_wrapper<int>> 可以
        std::vector<std::reference_wrapper<int>> refs = {
            std::ref(a), std::ref(b), std::ref(c)
        };

        std::cout << "  原始值: ";
        for (const auto& r : refs) std::cout << r.get() << " ";
        std::cout << std::endl;

        // 通过容器修改原始值
        for (auto& r : refs) {
            r.get() *= 2;
        }

        std::cout << "  翻倍后: a=" << a << " b=" << b << " c=" << c << std::endl;

        // 排序引用
        std::sort(refs.begin(), refs.end(),
                  [](const auto& x, const auto& y) {
                      return x.get() > y.get();
                  });

        std::cout << "  降序排列: ";
        for (const auto& r : refs) std::cout << r.get() << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // 2. 线程中传引用
    std::cout << "[2] std::thread 中传引用:\n";
    {
        int result = 0;

        auto worker = [](int& out, int a, int b) {
            out = a + b;
        };

        // 不用 ref：编译可能通过但行为不正确（thread按值传递）
        // 用 ref：正确传递引用
        std::thread t(worker, std::ref(result), 10, 20);
        t.join();

        std::cout << "  线程计算结果: " << result << std::endl;
    }
    std::cout << std::endl;

    // 3. 多线程累加
    std::cout << "[3] 多线程中共享引用:\n";
    {
        std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int sum1 = 0, sum2 = 0;

        auto partial_sum = [](std::vector<int>::iterator begin,
                              std::vector<int>::iterator end,
                              int& result) {
            result = std::accumulate(begin, end, 0);
        };

        auto mid = data.begin() + data.size() / 2;

        std::thread t1(partial_sum, data.begin(), mid, std::ref(sum1));
        std::thread t2(partial_sum, mid, data.end(), std::ref(sum2));

        t1.join();
        t2.join();

        std::cout << "  前半部分和: " << sum1 << std::endl;
        std::cout << "  后半部分和: " << sum2 << std::endl;
        std::cout << "  总和: " << sum1 + sum2 << std::endl;
    }
    std::cout << std::endl;

    // 4. reference_wrapper 作为函数参数包装器
    std::cout << "[4] 用 reference_wrapper 包装可调用对象:\n";
    {
        struct Counter {
            int count = 0;
            void operator()() {
                ++count;
            }
        };

        Counter counter;

        // 如果直接传 counter 给 thread，thread 内部会拷贝它
        // 用 ref 则传递引用，修改原始对象
        std::thread t1(std::ref(counter));
        t1.join();
        std::thread t2(std::ref(counter));
        t2.join();
        std::thread t3(std::ref(counter));
        t3.join();

        std::cout << "  三个线程调用后计数: " << counter.count << std::endl;
    }
    std::cout << std::endl;

    // 5. 与算法配合
    std::cout << "[5] 与STL算法配合:\n";
    {
        std::vector<int> data = {5, 2, 8, 1, 9, 3};

        // 创建引用向量并排序，不影响原始数据顺序
        std::vector<std::reference_wrapper<int>> sorted_refs(
            data.begin(), data.end());

        std::sort(sorted_refs.begin(), sorted_refs.end());

        std::cout << "  原始数据: ";
        for (int v : data) std::cout << v << " ";
        std::cout << std::endl;

        std::cout << "  引用排序: ";
        for (const auto& r : sorted_refs) std::cout << r.get() << " ";
        std::cout << std::endl;
        // 注意：由于 sort 实际上交换了引用包装，原始数据的值也被交换了
        // 如果想保持原始不变，应该只对 reference_wrapper 排序而不交换值
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. reference_wrapper 不是引用
    std::cout << "[1] reference_wrapper 是对象，不是引用:\n";
    std::cout << "  int x = 10;\n";
    std::cout << "  auto rw = std::ref(x);  // rw 是 reference_wrapper<int>\n";
    std::cout << "  // rw = 20;  // 这是重新绑定，不是修改 x！\n";
    std::cout << "  // rw.get() = 20;  // 这才是修改 x\n";
    {
        int x = 10, y = 20;
        auto rw = std::ref(x);
        rw = std::ref(y);  // 重新绑定到 y，不是修改 x
        std::cout << "  x = " << x << ", y = " << y
                  << " (赋值是重新绑定，不是修改值)" << std::endl;
    }
    std::cout << std::endl;

    // 2. 悬空引用
    std::cout << "[2] 注意悬空引用:\n";
    std::cout << "  // std::reference_wrapper<int> rw;\n";
    std::cout << "  // {\n";
    std::cout << "  //     int x = 10;\n";
    std::cout << "  //     rw = std::ref(x);\n";
    std::cout << "  // }  // x 析构\n";
    std::cout << "  // rw.get();  // 未定义行为！悬空引用\n\n";

    // 3. thread 必须用 ref 传引用
    std::cout << "[3] std::thread 传引用必须用 std::ref:\n";
    std::cout << "  // void fn(int& x) { x = 42; }\n";
    std::cout << "  // int val = 0;\n";
    std::cout << "  // std::thread t(fn, val);      // 可能编译错误或拷贝\n";
    std::cout << "  // std::thread t(fn, std::ref(val));  // 正确！\n\n";

    // 4. auto 推导的类型
    std::cout << "[4] auto 推导出 reference_wrapper 而非引用:\n";
    {
        int x = 10;
        auto ref = std::ref(x);  // ref 的类型是 reference_wrapper<int>，不是 int&

        // 需要显式转换或用 .get()
        int& real_ref = ref;  // 隐式转换
        real_ref = 20;
        std::cout << "  auto推导为reference_wrapper, 需隐式转换为引用\n";
        std::cout << "  x = " << x << std::endl;
    }
    std::cout << std::endl;

    // 5. 不能对临时对象使用 ref
    std::cout << "[5] 不能对临时对象/右值使用 std::ref:\n";
    std::cout << "  // std::ref(42);          // 编译错误！\n";
    std::cout << "  // std::ref(std::string(\"tmp\"));  // 编译错误！\n";
    std::cout << "  // 原因: 临时对象生命周期短，引用会悬空\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::ref / std::cref / reference_wrapper - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::ref、std::cref 与 reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper)
