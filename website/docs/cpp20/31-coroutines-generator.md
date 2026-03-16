---
sidebar_position: 32
title: "Generator 类型 (co_yield 生成器)"
description: "C++20 Generator 类型 (co_yield 生成器) 教学示例"
tags: [cpp20, coroutines_generator]
---

# Generator 类型 (co_yield 生成器)

C++20 标准引入的 Generator 类型 (co_yield 生成器) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/31_coroutines_generator.cpp"
/**
 * ============================================================
 * 特性名称: Generator 类型 (co_yield 生成器)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: Generator 是协程最经典的应用之一，
 *          使用 co_yield 逐个产出值，实现惰性求值的序列生成。
 *
 * [为什么引入]: 传统实现惰性序列需要手写迭代器类，
 *             代码繁琐。Generator 协程让代码看起来像普通函数，
 *             但可以按需逐个产出值。
 *
 * [关键要点]:
 *   - co_yield 产出一个值并暂停协程
 *   - Generator 类型需要自定义 promise_type 和迭代器
 *   - 惰性求值：只有在迭代时才计算下一个值
 *   - 可以生成无限序列
 * ============================================================
 */

#include <coroutine>
#include <iostream>
#include <iterator>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <cmath>

// ===================== Generator 类型实现 =====================

template<typename T>
class Generator {
public:
    struct promise_type {
        T current_value;

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        // 初始暂停：创建后不立即执行，等待第一次迭代
        std::suspend_always initial_suspend() noexcept { return {}; }

        // 最终暂停：暂停以便析构函数销毁
        std::suspend_always final_suspend() noexcept { return {}; }

        // co_yield 的处理：存储值并暂停
        std::suspend_always yield_value(T value) {
            current_value = std::move(value);
            return {};
        }

        void return_void() {}

        void unhandled_exception() {
            std::terminate();
        }
    };

    // 迭代器类型（支持 range-based for）
    class Iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        Iterator() = default;
        explicit Iterator(std::coroutine_handle<promise_type> handle)
            : handle_(handle) {}

        // 解引用获取当前值
        const T& operator*() const {
            return handle_.promise().current_value;
        }

        const T* operator->() const {
            return &handle_.promise().current_value;
        }

        // 前进到下一个值
        Iterator& operator++() {
            handle_.resume();
            if (handle_.done()) {
                handle_ = nullptr;
            }
            return *this;
        }

        Iterator operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return handle_ == other.handle_;
        }

        bool operator!=(const Iterator& other) const {
            return handle_ != other.handle_;
        }

    private:
        std::coroutine_handle<promise_type> handle_ = nullptr;
    };

    // Generator 构造和析构
    explicit Generator(std::coroutine_handle<promise_type> h) : handle_(h) {}

    ~Generator() {
        if (handle_) handle_.destroy();
    }

    // 禁止复制
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

    // 允许移动
    Generator(Generator&& other) noexcept
        : handle_(std::exchange(other.handle_, nullptr)) {}

    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = std::exchange(other.handle_, nullptr);
        }
        return *this;
    }

    // 支持 range-based for
    Iterator begin() {
        if (handle_) {
            handle_.resume();  // 执行到第一个 co_yield
            if (handle_.done()) return Iterator{};
        }
        return Iterator{handle_};
    }

    Iterator end() {
        return Iterator{};
    }

    // 手动获取下一个值
    std::optional<T> next() {
        if (handle_ && !handle_.done()) {
            handle_.resume();
            if (!handle_.done()) {
                return handle_.promise().current_value;
            }
        }
        return std::nullopt;
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

// ===================== basic_usage() =====================

// 简单的数字序列生成器
Generator<int> range(int start, int end) {
    for (int i = start; i < end; ++i) {
        co_yield i;
    }
}

// 斐波那契数列生成器（无限序列）
Generator<long long> fibonacci() {
    long long a = 0, b = 1;
    while (true) {
        co_yield a;
        auto next = a + b;
        a = b;
        b = next;
    }
}

// 字符串生成器
Generator<std::string> greetings() {
    co_yield "你好";
    co_yield "世界";
    co_yield "C++20";
    co_yield "协程";
}

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 有限序列
    std::cout << "range(1, 10): ";
    for (int n : range(1, 10)) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // 斐波那契（取前15个）
    std::cout << "\n斐波那契(前15个): ";
    int count = 0;
    for (long long n : fibonacci()) {
        std::cout << n << " ";
        if (++count >= 15) break;
    }
    std::cout << "\n";

    // 字符串序列
    std::cout << "\n问候语: ";
    for (const auto& s : greetings()) {
        std::cout << s << " ";
    }
    std::cout << "\n";
}

// ===================== advanced_usage() =====================

// 素数生成器
Generator<int> primes() {
    co_yield 2;

    for (int n = 3; ; n += 2) {
        bool is_prime = true;
        for (int i = 3; i * i <= n; i += 2) {
            if (n % i == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            co_yield n;
        }
    }
}

// 带状态的生成器：累积和
Generator<double> running_sum(const std::vector<double>& data) {
    double sum = 0.0;
    for (double val : data) {
        sum += val;
        co_yield sum;
    }
}

// 带状态的生成器：移动平均
Generator<double> moving_average(const std::vector<double>& data, int window) {
    std::vector<double> buffer;
    for (double val : data) {
        buffer.push_back(val);
        if (static_cast<int>(buffer.size()) > window) {
            buffer.erase(buffer.begin());
        }
        double sum = 0;
        for (double v : buffer) sum += v;
        co_yield sum / static_cast<double>(buffer.size());
    }
}

// 树的遍历（用生成器简化）
struct TreeNode {
    int value;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

// 中序遍历生成器
Generator<int> inorder(TreeNode* node) {
    if (node == nullptr) co_return;

    // 注意：C++20 不支持直接的 co_yield from（递归 co_yield）
    // 需要手动迭代子生成器
    if (node->left) {
        for (int val : inorder(node->left)) {
            co_yield val;
        }
    }

    co_yield node->value;

    if (node->right) {
        for (int val : inorder(node->right)) {
            co_yield val;
        }
    }
}

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 素数生成器
    std::cout << "-- 素数生成器 --\n";
    std::cout << "前20个素数: ";
    int count = 0;
    for (int p : primes()) {
        std::cout << p << " ";
        if (++count >= 20) break;
    }
    std::cout << "\n";

    // 累积和
    std::cout << "\n-- 累积和 --\n";
    std::vector<double> data = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::cout << "数据: ";
    for (double v : data) std::cout << v << " ";
    std::cout << "\n累积和: ";
    for (double s : running_sum(data)) {
        std::cout << s << " ";
    }
    std::cout << "\n";

    // 移动平均
    std::cout << "\n-- 移动平均 (窗口=3) --\n";
    std::vector<double> prices = {10, 11, 12, 11, 13, 14, 12, 15};
    std::cout << "价格: ";
    for (double v : prices) std::cout << v << " ";
    std::cout << "\n均线: ";
    for (double avg : moving_average(prices, 3)) {
        std::cout << avg << " ";
    }
    std::cout << "\n";

    // 树的遍历
    std::cout << "\n-- 树的中序遍历 --\n";
    //       4
    //      / \
    //     2   6
    //    / \ / \
    //   1  3 5  7
    TreeNode n1{1}, n3{3}, n5{5}, n7{7};
    TreeNode n2{2, &n1, &n3};
    TreeNode n6{6, &n5, &n7};
    TreeNode root{4, &n2, &n6};

    std::cout << "中序遍历: ";
    for (int val : inorder(&root)) {
        std::cout << val << " ";
    }
    std::cout << "\n";

    // 手动使用 next()
    std::cout << "\n-- 手动调用 next() --\n";
    auto gen = range(1, 6);
    while (auto val = gen.next()) {
        std::cout << "得到: " << *val << "\n";
    }
    std::cout << "序列结束\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：Generator 只能遍历一次
    std::cout << "1. Generator 通常只能遍历一次:\n";
    std::cout << "   auto gen = range(1, 5);\n";
    std::cout << "   for (int n : gen) { ... }  // 第一次：1 2 3 4\n";
    std::cout << "   for (int n : gen) { ... }  // 第二次：空! 协程已结束\n\n";

    // 陷阱2：引用参数的生命周期
    std::cout << "2. 协程中引用参数可能在暂停后失效:\n";
    std::cout << "   Generator<int> bad(const vector<int>& v) {\n";
    std::cout << "       for (int n : v) co_yield n;  // v 可能已销毁!\n";
    std::cout << "   }\n";
    std::cout << "   auto gen = bad(get_temp_vector());  // 悬垂引用!\n";
    std::cout << "   建议：按值传递或确保引用的生命周期\n\n";

    // 陷阱3：递归 co_yield 不高效
    std::cout << "3. C++20 没有 co_yield from（Python 的 yield from）:\n";
    std::cout << "   递归生成器中的 for(auto v : sub_gen) co_yield v;\n";
    std::cout << "   每层递归都会创建新的协程帧，有性能开销\n";
    std::cout << "   C++23 的 std::generator 改善了这个问题\n\n";

    // 陷阱4：无限序列必须用 break 或 take 来终止
    std::cout << "4. 无限序列必须有终止条件:\n";
    std::cout << "   for (int n : fibonacci()) { ... }  // 死循环!\n";
    std::cout << "   for (int n : fibonacci()) { if(...) break; }  // 正确\n\n";

    // 陷阱5：co_yield 和 co_return 不能混用
    std::cout << "5. 使用 co_yield 的协程的 promise_type:\n";
    std::cout << "   必须有 yield_value() 成员\n";
    std::cout << "   可以有 return_void()（隐式结束）\n";
    std::cout << "   但不能有 return_value()（不能 co_return 一个值）\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== Generator 类型 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: Generator 类型 (co_yield 生成器)](https://en.cppreference.com/w/cpp/language/coroutines)
