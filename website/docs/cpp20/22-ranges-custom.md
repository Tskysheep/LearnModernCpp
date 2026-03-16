---
sidebar_position: 23
title: "自定义范围和视图适配器 (Custom Ranges and View Adaptors)"
description: "C++20 自定义范围和视图适配器 (Custom Ranges and View Adaptors) 教学示例"
tags: [cpp20, ranges_custom]
---

# 自定义范围和视图适配器 (Custom Ranges and View Adaptors)

C++20 标准引入的 自定义范围和视图适配器 (Custom Ranges and View Adaptors) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/22_ranges_custom.cpp"
/**
 * ============================================================
 * 特性名称: 自定义范围和视图适配器 (Custom Ranges and View Adaptors)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: 程序员可以创建自己的类型来满足 range concept，
 *          也可以创建自定义的视图适配器参与管道操作。
 *
 * [为什么引入]: 标准库的视图不能覆盖所有场景，
 *             自定义范围和视图适配器允许扩展 Ranges 库的能力。
 *
 * [关键要点]:
 *   - 满足 std::ranges::range 的类型需要提供 begin() 和 end()
 *   - 自定义视图通常继承自 std::ranges::view_interface
 *   - 视图应该是轻量级的（O(1) 复制）
 *   - 可以创建支持管道操作符的视图适配器
 * ============================================================
 */

#include <algorithm>
#include <concepts>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

// ===================== basic_usage() =====================

// 自定义范围类型：整数区间 [from, to)
class IntRange {
public:
    // 简单的前向迭代器
    class Iterator {
    public:
        // C++20 要求的迭代器类型别名
        using iterator_category = std::forward_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using pointer = const int*;
        using reference = int;

        Iterator() : value_(0) {}
        explicit Iterator(int val) : value_(val) {}

        int operator*() const { return value_; }
        Iterator& operator++() { ++value_; return *this; }
        Iterator operator++(int) { auto tmp = *this; ++value_; return tmp; }
        bool operator==(const Iterator& other) const { return value_ == other.value_; }
        bool operator!=(const Iterator& other) const { return value_ != other.value_; }

    private:
        int value_;
    };

    IntRange(int from, int to) : from_(from), to_(to) {}

    Iterator begin() const { return Iterator(from_); }
    Iterator end() const { return Iterator(to_); }

    // 可选：提供 size()
    std::size_t size() const {
        return static_cast<std::size_t>(to_ > from_ ? to_ - from_ : 0);
    }

private:
    int from_;
    int to_;
};

// 验证 IntRange 满足 range concept
static_assert(std::ranges::range<IntRange>);

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 使用自定义范围
    IntRange range(1, 11);

    std::cout << "IntRange(1, 11): ";
    for (int n : range) {
        std::cout << n << " ";
    }
    std::cout << "\n";
    std::cout << "大小: " << range.size() << "\n";

    // 自定义范围可以直接用于 ranges 算法
    auto it = std::ranges::find(range, 5);
    std::cout << "find(5): " << *it << "\n";

    auto cnt = std::ranges::count_if(range, [](int n) { return n % 2 == 0; });
    std::cout << "偶数个数: " << cnt << "\n";

    // 可以与视图组合
    std::cout << "偶数: ";
    for (int n : range | std::views::filter([](int n) { return n % 2 == 0; })) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    std::cout << "平方(前5个): ";
    for (int n : range
                 | std::views::transform([](int n) { return n * n; })
                 | std::views::take(5)) {
        std::cout << n << " ";
    }
    std::cout << "\n";
}

// ===================== advanced_usage() =====================

// 使用 view_interface 创建自定义视图：步进视图 (stride view)
// 每隔 N 个元素取一个
template<std::ranges::input_range R>
class stride_view : public std::ranges::view_interface<stride_view<R>> {
public:
    class Iterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = std::ranges::range_value_t<R>;
        using difference_type = std::ranges::range_difference_t<R>;
        using reference = std::ranges::range_reference_t<R>;

        Iterator() = default;
        Iterator(std::ranges::iterator_t<R> current,
                 std::ranges::sentinel_t<R> end,
                 std::size_t step)
            : current_(current), end_(end), step_(step) {}

        reference operator*() const { return *current_; }

        Iterator& operator++() {
            for (std::size_t i = 0; i < step_ && current_ != end_; ++i) {
                ++current_;
            }
            return *this;
        }

        Iterator operator++(int) {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return current_ == other.current_;
        }

        bool operator!=(const Iterator& other) const {
            return current_ != other.current_;
        }

        // 哨兵比较
        bool operator==(std::ranges::sentinel_t<R> sent) const {
            return current_ == sent;
        }

    private:
        std::ranges::iterator_t<R> current_{};
        std::ranges::sentinel_t<R> end_{};
        std::size_t step_ = 1;
    };

    stride_view() = default;
    stride_view(R base, std::size_t step) : base_(std::move(base)), step_(step) {}

    Iterator begin() {
        return Iterator(std::ranges::begin(base_), std::ranges::end(base_), step_);
    }

    // 使用默认哨兵
    auto end() {
        return Iterator(std::ranges::end(base_), std::ranges::end(base_), step_);
    }

private:
    R base_{};
    std::size_t step_ = 1;
};

// 推导指引
template<typename R>
stride_view(R&&, std::size_t) -> stride_view<std::views::all_t<R>>;

// 自定义视图适配器闭包（可用于管道操作符）
struct stride_fn {
    std::size_t step;

    template<std::ranges::viewable_range R>
    auto operator()(R&& r) const {
        return stride_view(std::forward<R>(r), step);
    }

    // 支持管道操作符: range | stride(3)
    template<std::ranges::viewable_range R>
    friend auto operator|(R&& r, const stride_fn& s) {
        return s(std::forward<R>(r));
    }
};

// 工厂函数
auto stride(std::size_t step) {
    return stride_fn{step};
}

// 简单的自定义视图：enumerate (给元素加索引)
template<std::ranges::input_range R>
class enumerate_view : public std::ranges::view_interface<enumerate_view<R>> {
public:
    class Iterator {
    public:
        using value_type = std::pair<std::size_t, std::ranges::range_reference_t<R>>;
        using difference_type = std::ranges::range_difference_t<R>;
        using iterator_category = std::input_iterator_tag;

        Iterator() = default;
        Iterator(std::ranges::iterator_t<R> it, std::size_t idx)
            : it_(it), index_(idx) {}

        auto operator*() const {
            return std::pair<std::size_t, decltype(*it_)>{index_, *it_};
        }

        Iterator& operator++() { ++it_; ++index_; return *this; }
        Iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }
        bool operator==(const Iterator& other) const { return it_ == other.it_; }
        bool operator!=(const Iterator& other) const { return it_ != other.it_; }

    private:
        std::ranges::iterator_t<R> it_{};
        std::size_t index_ = 0;
    };

    enumerate_view() = default;
    explicit enumerate_view(R base) : base_(std::move(base)) {}

    Iterator begin() { return Iterator(std::ranges::begin(base_), 0); }
    Iterator end() { return Iterator(std::ranges::end(base_), 0); }

private:
    R base_{};
};

template<typename R>
enumerate_view(R&&) -> enumerate_view<std::views::all_t<R>>;

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 步进视图
    std::cout << "-- 自定义 stride 视图 --\n";
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "每隔2个: ";
    for (int n : stride_view(std::views::all(nums), 2)) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    std::cout << "每隔3个: ";
    for (int n : stride_view(std::views::all(nums), 3)) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // 使用管道操作符
    std::cout << "管道: ";
    for (int n : nums | stride(4)) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // enumerate 视图
    std::cout << "\n-- 自定义 enumerate 视图 --\n";
    std::vector<std::string> names = {"张三", "李四", "王五"};

    for (auto [idx, name] : enumerate_view(std::views::all(names))) {
        std::cout << "  [" << idx << "] " << name << "\n";
    }

    // 组合使用
    std::cout << "\n-- 组合自定义视图与标准视图 --\n";
    std::cout << "1-20 的偶数，每隔2个取: ";
    for (int n : std::views::iota(1, 21)
                 | std::views::filter([](int n) { return n % 2 == 0; })
                 | stride(2)) {
        std::cout << n << " ";
    }
    std::cout << "\n";

    // view_interface 提供的便利方法
    std::cout << "\n-- view_interface 提供的方法 --\n";
    auto sv = stride_view(std::views::all(nums), 2);
    std::cout << "首元素: " << *sv.begin() << "\n";

    // 验证满足 range concept
    static_assert(std::ranges::range<stride_view<std::views::all_t<std::vector<int>&>>>);
    std::cout << "stride_view 满足 range concept: true\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：视图应该是轻量级的
    std::cout << "1. 视图应该是 O(1) 复制的:\n";
    std::cout << "   视图不应该拥有大量数据\n";
    std::cout << "   通常只存储迭代器/引用 + 少量参数\n\n";

    // 陷阱2：view_interface 的要求
    std::cout << "2. 继承 view_interface<Derived> 使用 CRTP 模式:\n";
    std::cout << "   class MyView : public view_interface<MyView> { ... };\n";
    std::cout << "   需要提供 begin() 和 end()\n";
    std::cout << "   view_interface 自动提供 empty(), operator bool(), front() 等\n\n";

    // 陷阱3：迭代器类型别名
    std::cout << "3. 自定义迭代器必须提供必要的类型别名:\n";
    std::cout << "   - value_type\n";
    std::cout << "   - difference_type\n";
    std::cout << "   - iterator_category (或 iterator_concept)\n";
    std::cout << "   缺少这些会导致 concept 检查失败\n\n";

    // 陷阱4：管道操作符的实现
    std::cout << "4. 自定义管道操作符需要实现 friend operator|:\n";
    std::cout << "   通常创建一个闭包对象，重载 | 运算符\n";
    std::cout << "   C++23 的 range_adaptor_closure 简化了这个过程\n\n";

    // 陷阱5：视图的生命周期
    std::cout << "5. 视图不拥有数据，必须确保底层范围的生命周期:\n";
    std::cout << "   auto view = get_temp_vector() | views::take(5);\n";
    std::cout << "   // 悬空! 临时 vector 已销毁\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== 自定义范围和视图适配器 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 自定义范围和视图适配器 (Custom Ranges and View Adaptors)](https://en.cppreference.com/w/cpp/ranges)
