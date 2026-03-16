---
sidebar_position: 91
slug: 90-attributes
title: "[[assume]] 属性（编译器优化假设）"
description: "C++23 [[assume]] 属性（编译器优化假设） 教学示例"
tags: [cpp23, attributes]
---

# [[assume]] 属性（编译器优化假设）

C++23 标准引入的 [[assume]] 属性（编译器优化假设） 特性完整教学示例。

## 完整源码

```cpp title="cpp23/90_attributes.cpp"
/**
 * ============================================================
 * 特性名称: [[assume]] 属性（编译器优化假设）
 * 所属标准: C++23
 * ============================================================
 *
 * [是什么]: [[assume(expr)]] 是一种属性，告诉编译器某个表达式在
 *          该点一定为 true。编译器可以基于这个假设进行优化。
 *          如果假设在运行时不成立，行为是未定义的。
 *
 * [为什么引入]: 程序员常常拥有编译器无法推断的知识（如"这个值
 *              一定是正数"、"这个指针一定非空"）。[[assume]] 让
 *              程序员把这些知识传达给编译器，从而使编译器生成更
 *              高效的代码。
 *
 * [关键要点]:
 *   - [[assume(expr)]]: expr 表达式不会被求值，仅供编译器优化
 *   - 如果假设在运行时不成立，是未定义行为（UB）
 *   - 与 assert 不同：assert 在调试时检查，assume 是优化提示
 *   - 替代了各编译器的非标准扩展（__builtin_assume、__assume）
 *   - 只能用于空语句（属性放在分号前）
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <cstdint>
#include <cassert>

// ============================================================
// 特性检测
// ============================================================
#if defined(__has_cpp_attribute)
#if __has_cpp_attribute(assume) >= 202207L
#define HAS_ASSUME 1
#else
#define HAS_ASSUME 0
#endif
#else
#define HAS_ASSUME 0
#endif

// 提供跨平台的替代方案
#if HAS_ASSUME
#define ASSUME(expr) [[assume(expr)]]
#elif defined(__clang__)
#define ASSUME(expr) __builtin_assume(expr)
#elif defined(__GNUC__)
// GCC 13+ 支持 __attribute__((assume(expr)))
#if __GNUC__ >= 13
#define ASSUME(expr) __attribute__((assume(expr)))
#else
#define ASSUME(expr) do { if (!(expr)) __builtin_unreachable(); } while(0)
#endif
#elif defined(_MSC_VER)
#define ASSUME(expr) __assume(expr)
#else
#define ASSUME(expr) /* 无操作 */
#endif

// ============================================================
// 基础用法
// ============================================================
namespace basic_usage {

// 场景1：告诉编译器值的范围
int divide_by_positive(int a, int b) {
    // 我们知道 b > 0（由调用者保证）
    // 编译器可以利用这个信息避免除零检查，优化除法指令
#if HAS_ASSUME
    [[assume(b > 0)]];
#else
    ASSUME(b > 0);
#endif
    return a / b;
}

// 场景2：指针非空假设
int dereference_safely(const int* ptr) {
    // 调用者保证 ptr 非空
#if HAS_ASSUME
    [[assume(ptr != nullptr)]];
#else
    ASSUME(ptr != nullptr);
#endif
    return *ptr;
}

// 场景3：对齐假设
void process_aligned_data(const char* data, int size) {
    // 假设 size 是 4 的倍数
#if HAS_ASSUME
    [[assume(size % 4 == 0)]];
    [[assume(size > 0)]];
#else
    ASSUME(size % 4 == 0);
    ASSUME(size > 0);
#endif
    // 编译器可以生成更高效的循环（如展开、向量化）
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += data[i];
    }
    std::cout << "  处理了 " << size << " 字节，和 = " << sum << std::endl;
}

void demonstrate() {
    std::cout << "[基础用法]" << std::endl;

#if HAS_ASSUME
    std::cout << "  [使用标准 [[assume]] 属性]" << std::endl;
#else
    std::cout << "  [使用编译器特定的 assume 替代]" << std::endl;
#endif

    // 正值除法
    std::cout << "  -- 正值除法优化 --" << std::endl;
    std::cout << "  divide_by_positive(100, 7) = " << divide_by_positive(100, 7) << std::endl;
    std::cout << "  divide_by_positive(-50, 3) = " << divide_by_positive(-50, 3) << std::endl;

    // 指针解引用
    std::cout << "  -- 指针非空假设 --" << std::endl;
    int value = 42;
    std::cout << "  dereference_safely(&value) = " << dereference_safely(&value) << std::endl;

    // 对齐数据处理
    std::cout << "  -- 对齐数据处理 --" << std::endl;
    char data[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    process_aligned_data(data, 16);

    std::cout << std::endl;
}

} // namespace basic_usage

// ============================================================
// 进阶用法
// ============================================================
namespace advanced_usage {

// 场景1：枚举值范围假设
enum class Color : uint8_t { Red = 0, Green = 1, Blue = 2 };

const char* color_name(Color c) {
    // 假设 c 的值在合法范围内
#if HAS_ASSUME
    [[assume(static_cast<uint8_t>(c) <= 2)]];
#else
    ASSUME(static_cast<uint8_t>(c) <= 2);
#endif
    switch (c) {
        case Color::Red:   return "红色";
        case Color::Green: return "绿色";
        case Color::Blue:  return "蓝色";
    }
    // 因为 assume 的存在，编译器知道不会到这里
    // 可以省去 default 分支的代码生成
    return "";
}

// 场景2：循环优化
void sum_array_optimized(const int* arr, int n) {
    // 假设 n 是正数且是 8 的倍数
#if HAS_ASSUME
    [[assume(n > 0)]];
    [[assume(n % 8 == 0)]];
#else
    ASSUME(n > 0);
    ASSUME(n % 8 == 0);
#endif
    // 编译器可以：
    // - 跳过 n<=0 的检查
    // - 使用 8 路展开或 SIMD 向量化
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += arr[i];
    }
    std::cout << "  数组和 (n=" << n << ") = " << sum << std::endl;
}

// 场景3：与 assert 配合使用的模式
int safe_process(int x) {
    // 调试时用 assert 检查
    assert(x >= 0 && "x 不能为负数");
    // 发布时用 assume 优化
#if HAS_ASSUME
    [[assume(x >= 0)]];
#else
    ASSUME(x >= 0);
#endif
    // 编译器知道 x >= 0，可能消除某些条件分支
    if (x < 0) {
        return -1;  // 编译器可能删除这个分支
    }
    return x * x;
}

void demonstrate() {
    std::cout << "[进阶用法]" << std::endl;

    // 枚举假设
    std::cout << "  -- 枚举值范围假设 --" << std::endl;
    std::cout << "  color_name(Red)   = " << color_name(Color::Red) << std::endl;
    std::cout << "  color_name(Green) = " << color_name(Color::Green) << std::endl;
    std::cout << "  color_name(Blue)  = " << color_name(Color::Blue) << std::endl;

    // 循环优化
    std::cout << "  -- 循环优化 --" << std::endl;
    int arr[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    sum_array_optimized(arr, 16);

    // 与 assert 配合
    std::cout << "  -- assert + assume 模式 --" << std::endl;
    std::cout << "  safe_process(5) = " << safe_process(5) << std::endl;

    // 语法说明
    std::cout << "  -- [[assume]] 语法说明 --" << std::endl;
    std::cout << R"(
    // 标准 C++23 语法：
    [[assume(expr)]];           // 空语句上的属性

    // 编译器特定替代（C++23 之前）：
    __builtin_assume(expr);     // Clang
    __assume(expr);             // MSVC
    if (!(expr)) __builtin_unreachable();  // GCC 技巧
)" << std::endl;

    // 与其他特性对比
    std::cout << "  -- 与相关特性对比 --" << std::endl;
    std::cout << "  [[assume(expr)]]:     优化提示，expr 不求值，违反是 UB" << std::endl;
    std::cout << "  assert(expr):         调试检查，expr 会求值，NDEBUG 时移除" << std::endl;
    std::cout << "  std::unreachable():   标记不可达代码点" << std::endl;
    std::cout << "  if constexpr(expr):   编译期条件，丢弃 false 分支" << std::endl;

    std::cout << std::endl;
}

} // namespace advanced_usage

// ============================================================
// 易错点与陷阱
// ============================================================
namespace pitfalls {

void demonstrate() {
    std::cout << "[易错点与陷阱]" << std::endl;

    // 陷阱1：假设表达式不被求值
    std::cout << "  陷阱1: [[assume]] 中的表达式不被求值" << std::endl;
    std::cout << "    - [[assume(++x > 0)]]; // x 不会被增加！" << std::endl;
    std::cout << "    - 不要在 assume 中放有副作用的表达式" << std::endl;

    // 演示：下面的 x 不会改变
    int x = 5;
    // 如果编译器支持，下面的 x++ 不会执行
#if HAS_ASSUME
    [[assume(x > 0)]];  // x 不变
#endif
    std::cout << "    assume 后 x 仍然 = " << x << std::endl;

    // 陷阱2：错误的假设导致 UB
    std::cout << "  陷阱2: 错误的假设导致未定义行为" << std::endl;
    std::cout << "    - [[assume(ptr != nullptr)]]; 如果 ptr 为 null -> UB" << std::endl;
    std::cout << "    - 编译器可能基于错误假设优化掉安全检查" << std::endl;
    std::cout << "    - 这比没有 assume 更危险！" << std::endl;
    std::cout << "    - 例如:" << std::endl;
    std::cout << R"(      int* p = nullptr;
      [[assume(p != nullptr)]];
      if (p == nullptr) {
          handle_null();  // 编译器可能删除这个分支！
      }
      *p = 42;  // 崩溃)" << std::endl;

    // 陷阱3：不能用于所有位置
    std::cout << "  陷阱3: [[assume]] 只能用于空语句" << std::endl;
    std::cout << "    - [[assume(x > 0)]];               // 正确：空语句" << std::endl;
    std::cout << "    - [[assume(x > 0)]] int y = x;     // 错误！" << std::endl;
    std::cout << "    - [[assume(x > 0)]] if (x) {...}   // 错误！" << std::endl;

    // 陷阱4：assume 不是 contract
    std::cout << "  陷阱4: [[assume]] 不是合约（contract）" << std::endl;
    std::cout << "    - 合约（C++26 可能引入）有检查模式" << std::endl;
    std::cout << "    - assume 永远不检查，永远是优化提示" << std::endl;
    std::cout << "    - 推荐模式: 调试用 assert，发布用 assume" << std::endl;

    // 陷阱5：过度使用反而有害
    std::cout << "  陷阱5: 过度使用 assume 可能有害" << std::endl;
    std::cout << "    - 每个 assume 都是一个 UB 隐患" << std::endl;
    std::cout << "    - 只在性能关键且确信正确时使用" << std::endl;
    std::cout << "    - 用 assert 先验证你的假设是否真的成立" << std::endl;

    // 推荐模式
    std::cout << "  -- 推荐的使用模式 --" << std::endl;
    std::cout << R"(
    void process(int* data, int n) {
        // 1. 调试时检查前提条件
        assert(data != nullptr);
        assert(n > 0);

        // 2. 发布时提供优化提示
        [[assume(data != nullptr)]];
        [[assume(n > 0)]];

        // 3. 或者用宏统一：
        // #define PRECONDITION(expr) assert(expr); [[assume(expr)]]
        // PRECONDITION(data != nullptr);
    }
)" << std::endl;

    std::cout << std::endl;
}

} // namespace pitfalls

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== [[assume]] 属性（编译器优化假设） - C++23 =====\n" << std::endl;

#if HAS_ASSUME
    std::cout << "[信息] 编译器支持标准 [[assume]] 属性 (C++23)" << std::endl;
#else
    std::cout << "[信息] 编译器不支持标准 [[assume]] 属性" << std::endl;
#if defined(__clang__)
    std::cout << "  替代方案: __builtin_assume() (Clang)" << std::endl;
#elif defined(__GNUC__)
    std::cout << "  替代方案: __builtin_unreachable() 技巧 (GCC)" << std::endl;
#elif defined(_MSC_VER)
    std::cout << "  替代方案: __assume() (MSVC)" << std::endl;
#endif
    std::cout << "请使用支持 C++23 的编译器（如 GCC 14+ 或 Clang 18+）获得标准支持。" << std::endl;
#endif
    std::cout << std::endl;

    basic_usage::demonstrate();
    advanced_usage::demonstrate();
    pitfalls::demonstrate();

    return 0;
}
```

## 参考链接

- [cppreference: [[assume]] 属性（编译器优化假设）](https://en.cppreference.com/w/cpp/language/attributes)
