---
sidebar_position: 9
title: "__has_include 预处理检查"
description: "C++17 __has_include 预处理检查 教学示例"
tags: [cpp17, has_include]
---

# __has_include 预处理检查

C++17 标准引入的 __has_include 预处理检查 特性完整教学示例。

## 完整源码

```cpp title="cpp17/08_has_include.cpp"
/**
 * ============================================================
 * 特性名称: __has_include 预处理检查
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: 预处理阶段的条件检查宏，用于检测某个头文件是否可用。
 *          语法为 __has_include(<header>) 或 __has_include("header")。
 *
 * [为什么引入]: 不同编译器和平台对某些库的支持不同（如 <optional>、
 *              <filesystem>）。之前需要靠编译器特定宏来检测，
 *              __has_include 提供了标准化的、可移植的检测方式。
 *
 * [关键要点]:
 *   - 只能在 #if / #elif 预处理指令中使用
 *   - 检查头文件是否「存在」，不保证头文件内容完整可用
 *   - 使用 <> 和 "" 遵循各自的搜索路径规则
 *   - 非常适合编写跨平台/跨编译器的兼容性代码
 * ============================================================
 */

#include <iostream>
#include <string>

// ============================================================
// 1. 检测 C++17 标准库头文件
// ============================================================

// 检测 <optional>
#if __has_include(<optional>)
    #include <optional>
    #define HAS_OPTIONAL 1
#elif __has_include(<experimental/optional>)
    #include <experimental/optional>
    #define HAS_OPTIONAL 1
    namespace std { using experimental::optional; }
#else
    #define HAS_OPTIONAL 0
#endif

// 检测 <filesystem>
#if __has_include(<filesystem>)
    #include <filesystem>
    #define HAS_FILESYSTEM 1
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
    #define HAS_FILESYSTEM 1
#else
    #define HAS_FILESYSTEM 0
#endif

// 检测 <string_view>
#if __has_include(<string_view>)
    #include <string_view>
    #define HAS_STRING_VIEW 1
#else
    #define HAS_STRING_VIEW 0
#endif

// 检测 <any>
#if __has_include(<any>)
    #include <any>
    #define HAS_ANY 1
#else
    #define HAS_ANY 0
#endif

// 检测 <variant>
#if __has_include(<variant>)
    #include <variant>
    #define HAS_VARIANT 1
#else
    #define HAS_VARIANT 0
#endif

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 报告各种头文件的可用性
    std::cout << "<optional>    可用: " << (HAS_OPTIONAL ? "是" : "否") << "\n";
    std::cout << "<filesystem>  可用: " << (HAS_FILESYSTEM ? "是" : "否") << "\n";
    std::cout << "<string_view> 可用: " << (HAS_STRING_VIEW ? "是" : "否") << "\n";
    std::cout << "<any>         可用: " << (HAS_ANY ? "是" : "否") << "\n";
    std::cout << "<variant>     可用: " << (HAS_VARIANT ? "是" : "否") << "\n";

    // 条件使用 optional
#if HAS_OPTIONAL
    std::optional<int> opt = 42;
    std::cout << "optional 值: " << opt.value() << "\n";
#else
    std::cout << "optional 不可用，使用替代方案\n";
#endif

    // 条件使用 string_view
#if HAS_STRING_VIEW
    std::string_view sv = "Hello, string_view!";
    std::cout << "string_view: " << sv << "\n";
#else
    std::cout << "string_view 不可用\n";
#endif
}

// ============================================================
// 进阶用法
// ============================================================

// 2. 检测系统特定头文件
#if __has_include(<unistd.h>)
    #define IS_POSIX 1
#else
    #define IS_POSIX 0
#endif

#if __has_include(<windows.h>)
    #define IS_WINDOWS 1
#else
    #define IS_WINDOWS 0
#endif

// 3. 检测第三方库
#if __has_include(<boost/version.hpp>)
    #define HAS_BOOST 1
#else
    #define HAS_BOOST 0
#endif

#if __has_include(<fmt/format.h>)
    #define HAS_FMT 1
#else
    #define HAS_FMT 0
#endif

// 4. 根据平台选择不同实现
std::string get_platform_name() {
#if IS_WINDOWS
    return "Windows";
#elif IS_POSIX
    return "POSIX (Linux/macOS)";
#else
    return "未知平台";
#endif
}

// 5. 提供降级方案的工具函数
std::string safe_format(int value) {
#if HAS_FMT
    return fmt::format("值={}", value);
#else
    return "值=" + std::to_string(value);
#endif
}

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 平台检测
    std::cout << "运行平台: " << get_platform_name() << "\n";
    std::cout << "POSIX 头文件: " << (IS_POSIX ? "可用" : "不可用") << "\n";
    std::cout << "Windows 头文件: " << (IS_WINDOWS ? "可用" : "不可用") << "\n";

    // 第三方库检测
    std::cout << "Boost 库: " << (HAS_BOOST ? "可用" : "不可用") << "\n";
    std::cout << "fmt 库:   " << (HAS_FMT ? "可用" : "不可用") << "\n";

    // 降级方案
    std::cout << safe_format(42) << "\n";

    // 使用特性测试宏配合 __has_include
#if __has_include(<version>)
    std::cout << "<version> 头文件可用 (C++20 特性测试)\n";
#else
    std::cout << "<version> 头文件不可用\n";
#endif
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: __has_include 只检查文件是否存在，不检查内容
    //   头文件存在不代表里面的功能都可用
    //   例如 <filesystem> 可能存在但编译器实现不完整
    std::cout << "陷阱1 - __has_include 只检查文件存在，不保证内容完整\n";
    std::cout << "        建议配合功能测试宏(如 __cpp_lib_filesystem)使用\n";

    // 陷阱 2: 只能在 #if / #elif 中使用
    //   不能在 if constexpr 或运行时代码中使用
    //   if (__has_include(<xxx>))  // 错误！不是运行时表达式
    std::cout << "陷阱2 - 只能在 #if / #elif 预处理指令中使用\n";

    // 陷阱 3: <> 和 "" 搜索路径不同
    //   <header>  —— 系统/编译器搜索路径
    //   "header"  —— 先搜索当前目录，再搜索系统路径
    std::cout << "陷阱3 - <header> 和 \"header\" 的搜索路径不同\n";

    // 陷阱 4: 某些旧编译器不支持 __has_include
    //   需要先检查 __has_include 是否可用：
    //   #ifdef __has_include
    //     #if __has_include(<optional>)
    //       ...
    //     #endif
    //   #endif
    std::cout << "陷阱4 - 旧编译器可能不支持，需要 #ifdef __has_include 保护\n";

    // 陷阱 5: 不要用 __has_include 替代所有的功能测试
    //   C++20 引入了 <version> 和 __cpp_lib_xxx 宏，更精确
    std::cout << "陷阱5 - 优先使用 __cpp_lib_xxx 功能测试宏进行精确检测\n";

    // 展示功能测试宏的使用
#ifdef __cpp_lib_optional
    std::cout << "  __cpp_lib_optional = " << __cpp_lib_optional << "\n";
#endif
#ifdef __cpp_lib_string_view
    std::cout << "  __cpp_lib_string_view = " << __cpp_lib_string_view << "\n";
#endif
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== __has_include 预处理检查 - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: __has_include 预处理检查](https://en.cppreference.com/w/cpp/preprocessor/include)
