---
sidebar_position: 91
slug: 90-attributes
title: "C++20 属性 ([[likely]], [[unlikely]], [[no_unique_address]])"
description: "C++20 C++20 属性 ([[likely]], [[unlikely]], [[no_unique_address]]) 教学示例"
tags: [cpp20, attributes]
---

# C++20 属性 ([[likely]], [[unlikely]], [[no_unique_address]])

C++20 标准引入的 C++20 属性 ([[likely]], [[unlikely]], [[no_unique_address]]) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/90_attributes.cpp"
/**
 * ============================================================
 * 特性名称: C++20 属性 ([[likely]], [[unlikely]], [[no_unique_address]])
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: C++20 新增了三个标准属性：
 *          - [[likely]] / [[unlikely]]: 分支预测提示
 *          - [[no_unique_address]]: 空类成员优化
 *
 * [为什么引入]:
 *   - [[likely]]/[[unlikely]] 帮助编译器优化分支预测，
 *     替代了 GCC 的 __builtin_expect 非标准扩展。
 *   - [[no_unique_address]] 允许空类成员不占用额外空间，
 *     解决了空基类优化（EBO）不适用于成员的问题。
 *
 * [关键要点]:
 *   - [[likely]] 标记更可能执行的分支
 *   - [[unlikely]] 标记不太可能执行的分支
 *   - [[no_unique_address]] 允许空成员与其他成员共享地址
 *   - 这些都是优化提示，不影响程序正确性
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <functional>
#include <memory>
#include <type_traits>

// ===================== basic_usage() =====================

// [[likely]] 和 [[unlikely]] 用于分支预测
int process_error_code(int code) {
    if (code == 0) [[likely]] {
        // 大多数情况下，代码执行成功
        return 1;
    } else [[unlikely]] {
        // 错误路径不太可能执行
        std::cout << "  错误代码: " << code << "\n";
        return -1;
    }
}

// 在 switch 语句中使用
std::string classify_char(char c) {
    switch (c) {
    case 'a' ... 'z': [[fallthrough]];
    case 'A' ... 'Z':
        [[likely]] return "字母";
    case '0' ... '9':
        return "数字";
    case ' ':
    case '\t':
    case '\n':
        return "空白";
    default:
        [[unlikely]] return "特殊字符";
    }
}

// [[no_unique_address]] 用于空类成员优化
struct Empty {};  // 空类，sizeof 通常为 1

// 没有 [[no_unique_address]]
struct WithoutOptimization {
    int value;
    Empty empty;  // 占用额外的 1 字节（加上对齐可能更多）
};

// 有 [[no_unique_address]]
struct WithOptimization {
    int value;
    [[no_unique_address]] Empty empty;  // 不占用额外空间
};

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // [[likely]] / [[unlikely]]
    std::cout << "-- [[likely]] / [[unlikely]] --\n";
    std::cout << "process_error_code(0): " << process_error_code(0) << "\n";
    std::cout << "process_error_code(404): " << process_error_code(404) << "\n";

    std::cout << "\n字符分类:\n";
    std::cout << "  'A': " << classify_char('A') << "\n";
    std::cout << "  '5': " << classify_char('5') << "\n";
    std::cout << "  ' ': " << classify_char(' ') << "\n";
    std::cout << "  '@': " << classify_char('@') << "\n";

    // [[no_unique_address]]
    std::cout << "\n-- [[no_unique_address]] --\n";
    std::cout << "sizeof(Empty): " << sizeof(Empty) << "\n";
    std::cout << "sizeof(int): " << sizeof(int) << "\n";
    std::cout << "sizeof(WithoutOptimization): " << sizeof(WithoutOptimization) << "\n";
    std::cout << "sizeof(WithOptimization): " << sizeof(WithOptimization) << "\n";
    std::cout << "(使用 [[no_unique_address]] 后，空成员不占用额外空间)\n";
}

// ===================== advanced_usage() =====================

// 实际应用：allocator-aware 容器中的空分配器优化
// 类似 std::vector 的简化版
template<typename T, typename Allocator = std::allocator<T>>
class SimpleContainer {
    T* data_ = nullptr;
    std::size_t size_ = 0;
    // 分配器通常是无状态的空类
    // 使用 [[no_unique_address]] 避免浪费空间
    [[no_unique_address]] Allocator alloc_;

public:
    SimpleContainer() = default;
    std::size_t size() const { return size_; }
    static constexpr std::size_t struct_size() { return sizeof(SimpleContainer); }
};

// 对比版本：没有优化
template<typename T, typename Allocator = std::allocator<T>>
class SimpleContainerNoOpt {
    T* data_ = nullptr;
    std::size_t size_ = 0;
    Allocator alloc_;  // 没有 [[no_unique_address]]

public:
    SimpleContainerNoOpt() = default;
    static constexpr std::size_t struct_size() { return sizeof(SimpleContainerNoOpt); }
};

// [[no_unique_address]] 在策略模式中的应用
struct DefaultPolicy {};
struct CustomPolicy { int config = 42; };

template<typename Policy = DefaultPolicy>
class Engine {
    int state_ = 0;
    [[no_unique_address]] Policy policy_;

public:
    Engine() = default;
    explicit Engine(Policy p) : policy_(std::move(p)) {}
    static constexpr std::size_t struct_size() { return sizeof(Engine); }
};

// [[likely]]/[[unlikely]] 在性能关键路径上的应用
class FastMap {
    std::vector<std::pair<int, std::string>> data_;

public:
    void insert(int key, std::string value) {
        data_.emplace_back(key, std::move(value));
    }

    const std::string* find(int key) const {
        for (const auto& [k, v] : data_) {
            if (k == key) [[unlikely]] {  // 在大量查找中，匹配是少数情况
                return &v;
            }
        }
        return nullptr;
    }

    void process_all(std::function<void(int, const std::string&)> callback) const {
        for (const auto& [k, v] : data_) {
            if (v.empty()) [[unlikely]] {
                continue;  // 空值很少见
            }
            callback(k, v);
        }
    }
};

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 空分配器优化
    std::cout << "-- 空分配器优化 --\n";
    std::cout << "sizeof(std::allocator<int>): " << sizeof(std::allocator<int>) << "\n";
    std::cout << "SimpleContainer<int> (有优化): "
              << SimpleContainer<int>::struct_size() << " 字节\n";
    std::cout << "SimpleContainerNoOpt<int> (无优化): "
              << SimpleContainerNoOpt<int>::struct_size() << " 字节\n";

    // 策略模式中的空类优化
    std::cout << "\n-- 策略模式空类优化 --\n";
    std::cout << "Engine<DefaultPolicy> (空策略): "
              << Engine<DefaultPolicy>::struct_size() << " 字节\n";
    std::cout << "Engine<CustomPolicy> (有状态策略): "
              << Engine<CustomPolicy>::struct_size() << " 字节\n";

    // 分支预测在循环中的使用
    std::cout << "\n-- 循环中的分支预测 --\n";
    std::vector<int> data(100, 1);
    data[50] = 0;  // 只有一个零值

    int sum = 0;
    for (int val : data) {
        if (val != 0) [[likely]] {
            sum += val;
        } else [[unlikely]] {
            std::cout << "  发现零值!\n";
        }
    }
    std::cout << "  总和: " << sum << "\n";

    // while 循环中的 [[likely]]
    std::cout << "\n-- while 循环中使用 --\n";
    int count = 0;
    int i = 0;
    while (i < 100) [[likely]] {  // 循环条件大部分时候为 true
        ++count;
        ++i;
    }
    std::cout << "  循环执行了 " << count << " 次\n";

    // for 循环中的 [[likely]]
    int total = 0;
    for (int n = 0; n < 1000; ++n) [[likely]] {
        total += n;
    }
    std::cout << "  累加结果: " << total << "\n";

    // 实际应用演示
    std::cout << "\n-- 实际应用 --\n";
    FastMap fm;
    fm.insert(1, "一");
    fm.insert(2, "二");
    fm.insert(3, "三");

    if (auto* val = fm.find(2)) {
        std::cout << "  找到: " << *val << "\n";
    }

    fm.process_all([](int k, const std::string& v) {
        std::cout << "  处理: " << k << " → " << v << "\n";
    });
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：[[likely]]/[[unlikely]] 只是提示
    std::cout << "1. [[likely]]/[[unlikely]] 只是优化提示:\n";
    std::cout << "   编译器可以忽略它们\n";
    std::cout << "   不影响程序正确性，只可能影响性能\n";
    std::cout << "   滥用可能适得其反（误导编译器）\n\n";

    // 陷阱2：[[no_unique_address]] 对非空类无效
    std::cout << "2. [[no_unique_address]] 对非空类无效:\n";
    struct NonEmpty { int x; };
    struct Test {
        int a;
        [[no_unique_address]] NonEmpty b;
    };
    std::cout << "   sizeof(NonEmpty): " << sizeof(NonEmpty) << "\n";
    std::cout << "   sizeof(Test): " << sizeof(Test) << "\n";
    std::cout << "   非空类成员仍然占用空间\n\n";

    // 陷阱3：MSVC 对 [[no_unique_address]] 的支持
    std::cout << "3. MSVC 兼容性:\n";
    std::cout << "   MSVC 使用 [[msvc::no_unique_address]] 作为替代\n";
    std::cout << "   标准 [[no_unique_address]] 在 MSVC 中可能不生效\n";
    std::cout << "   可以用条件编译:\n";
    std::cout << "   #ifdef _MSC_VER\n";
    std::cout << "     #define NO_UNIQUE_ADDR [[msvc::no_unique_address]]\n";
    std::cout << "   #else\n";
    std::cout << "     #define NO_UNIQUE_ADDR [[no_unique_address]]\n";
    std::cout << "   #endif\n\n";

    // 陷阱4：[[likely]] 的位置
    std::cout << "4. [[likely]]/[[unlikely]] 应标记在语句上:\n";
    std::cout << "   if (cond) [[likely]] { ... }   // 正确：标记在语句\n";
    std::cout << "   [[likely]] if (cond) { ... }   // 可能不被支持\n\n";

    // 陷阱5：[[no_unique_address]] 和相同类型
    std::cout << "5. 两个相同类型的空成员不能同时优化:\n";
    struct TwoEmpty {
        [[no_unique_address]] Empty a;
        [[no_unique_address]] Empty b;
    };
    std::cout << "   struct { [[no_unique_address]] Empty a, b; }\n";
    std::cout << "   sizeof: " << sizeof(TwoEmpty) << "\n";
    std::cout << "   (a 和 b 不能有相同地址，至少一个占用空间)\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== C++20 属性 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: C++20 属性 ([[likely]], [[unlikely]], [[no_unique_address]])](https://en.cppreference.com/w/cpp/language/attributes)
