---
sidebar_position: 62
title: "std::stacktrace（栈追踪）"
description: "C++23 std::stacktrace（栈追踪） 教学示例"
tags: [cpp23, stacktrace]
---

# std::stacktrace（栈追踪）

C++23 标准引入的 std::stacktrace（栈追踪） 特性完整教学示例。

## 完整源码

```cpp title="cpp23/61_stacktrace.cpp"
/**
 * ============================================================
 * 特性名称: std::stacktrace（栈追踪）
 * 所属标准: C++23
 * ============================================================
 *
 * [是什么]: std::stacktrace 提供了在运行时获取当前调用栈信息的
 *          标准方式。它可以捕获函数调用链，用于错误诊断、日志
 *          记录等场景。
 *
 * [为什么引入]: 在 C++23 之前，获取栈追踪信息需要依赖平台特定
 *              API（如 Linux 的 backtrace()、Windows 的
 *              CaptureStackBackTrace()）或第三方库（如 Boost.
 *              Stacktrace）。标准化使得跨平台栈追踪成为可能。
 *
 * [关键要点]:
 *   - std::stacktrace::current() 获取当前栈追踪
 *   - std::stacktrace_entry 表示栈中的单个帧
 *   - 可以获取文件名、行号、函数名等信息
 *   - 需要 <stacktrace> 头文件
 *   - 注意：许多编译器对此特性的支持仍不完整
 *   - 可能需要链接额外的库（如 -lstdc++_libbacktrace）
 * ============================================================
 */

#include <iostream>
#include <string>
#include <sstream>

// ============================================================
// 特性检测
// ============================================================
#if __has_include(<stacktrace>)
#include <stacktrace>
#ifdef __cpp_lib_stacktrace
#define HAS_STACKTRACE 1
#else
#define HAS_STACKTRACE 0
#endif
#else
#define HAS_STACKTRACE 0
#endif

#if HAS_STACKTRACE

// ============================================================
// 基础用法
// ============================================================
namespace basic_usage {

void inner_function() {
    std::cout << "  -- 在 inner_function 中获取栈追踪 --" << std::endl;
    auto trace = std::stacktrace::current();

    std::cout << "  栈深度: " << trace.size() << " 帧" << std::endl;
    std::cout << "  栈追踪:" << std::endl;
    for (std::size_t i = 0; i < trace.size() && i < 10; ++i) {
        const auto& entry = trace[i];
        std::cout << "    [" << i << "] " << entry.description();
        if (!entry.source_file().empty()) {
            std::cout << " (" << entry.source_file()
                      << ":" << entry.source_line() << ")";
        }
        std::cout << std::endl;
    }
    if (trace.size() > 10) {
        std::cout << "    ... (省略了 " << trace.size() - 10 << " 帧)" << std::endl;
    }
}

void middle_function() {
    inner_function();
}

void outer_function() {
    middle_function();
}

void demonstrate() {
    std::cout << "[基础用法]" << std::endl;

    // 简单获取栈追踪
    std::cout << "  -- 基本栈追踪 --" << std::endl;
    auto trace = std::stacktrace::current();
    std::cout << "  当前栈追踪:" << std::endl;
    std::cout << trace << std::endl;

    // 通过调用链获取
    std::cout << "  -- 多层调用栈 --" << std::endl;
    outer_function();

    std::cout << std::endl;
}

} // namespace basic_usage

// ============================================================
// 进阶用法
// ============================================================
namespace advanced_usage {

// 带栈追踪的异常类
class TracedException : public std::exception {
public:
    TracedException(const std::string& msg)
        : message_(msg)
        , trace_(std::stacktrace::current()) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

    const std::stacktrace& trace() const noexcept {
        return trace_;
    }

    std::string full_report() const {
        std::ostringstream oss;
        oss << "异常: " << message_ << "\n";
        oss << "栈追踪:\n" << trace_;
        return oss.str();
    }

private:
    std::string message_;
    std::stacktrace trace_;
};

void problematic_function(int x) {
    if (x < 0) {
        throw TracedException("参数不能为负数: " + std::to_string(x));
    }
}

void caller_function(int x) {
    problematic_function(x);
}

// 日志辅助函数
void log_with_trace(const std::string& level, const std::string& msg) {
    auto trace = std::stacktrace::current();
    std::cout << "  [" << level << "] " << msg << std::endl;
    if (trace.size() > 1) {
        // 跳过 log_with_trace 自身
        std::cout << "    调用位置: " << trace[1].description() << std::endl;
    }
}

void demonstrate() {
    std::cout << "[进阶用法]" << std::endl;

    // 场景1：带栈追踪的异常
    std::cout << "  -- 带栈追踪的异常 --" << std::endl;
    try {
        caller_function(-5);
    } catch (const TracedException& e) {
        std::cout << e.full_report() << std::endl;
    }

    // 场景2：日志中附带调用位置
    std::cout << "  -- 日志附带调用位置 --" << std::endl;
    log_with_trace("INFO", "系统启动完成");
    log_with_trace("WARNING", "内存使用率较高");

    // 场景3：栈帧的详细信息
    std::cout << "  -- 栈帧详细信息 --" << std::endl;
    auto trace = std::stacktrace::current();
    if (!trace.empty()) {
        const auto& entry = trace[0];
        std::cout << "  描述: " << entry.description() << std::endl;
        std::cout << "  源文件: " << entry.source_file() << std::endl;
        std::cout << "  行号: " << entry.source_line() << std::endl;
    }

    std::cout << std::endl;
}

} // namespace advanced_usage

// ============================================================
// 易错点与陷阱
// ============================================================
namespace pitfalls {

void demonstrate() {
    std::cout << "[易错点与陷阱]" << std::endl;

    // 陷阱1：性能开销
    std::cout << "  陷阱1: 获取栈追踪有显著性能开销" << std::endl;
    std::cout << "    - 不要在性能关键路径中使用" << std::endl;
    std::cout << "    - 适合用于错误处理、日志等低频场景" << std::endl;

    // 陷阱2：编译器支持不完整
    std::cout << "  陷阱2: 编译器支持状态" << std::endl;
    std::cout << "    - GCC: 需要 -lstdc++_libbacktrace 链接" << std::endl;
    std::cout << "    - MSVC: 支持较好" << std::endl;
    std::cout << "    - Clang: 支持可能不完整" << std::endl;

    // 陷阱3：优化级别影响
    std::cout << "  陷阱3: 优化级别影响栈追踪质量" << std::endl;
    std::cout << "    - -O0: 信息最完整" << std::endl;
    std::cout << "    - -O2/-O3: 内联等优化可能导致帧缺失" << std::endl;
    std::cout << "    - 建议: 带调试信息编译 (-g)" << std::endl;

    // 陷阱4：线程安全
    std::cout << "  陷阱4: stacktrace::current() 捕获调用线程的栈" << std::endl;
    std::cout << "    - 每个线程有自己的调用栈" << std::endl;
    std::cout << "    - 不能获取其他线程的栈追踪" << std::endl;

    std::cout << std::endl;
}

} // namespace pitfalls

#endif // HAS_STACKTRACE

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::stacktrace（栈追踪） - C++23 =====\n" << std::endl;

#if HAS_STACKTRACE
    basic_usage::demonstrate();
    advanced_usage::demonstrate();
    pitfalls::demonstrate();
#else
    std::cout << "[提示] 当前编译器不支持 std::stacktrace 特性。" << std::endl;
    std::cout << "这是 C++23 中支持最不完整的特性之一。" << std::endl;
    std::cout << std::endl;
    std::cout << "编译器支持情况：" << std::endl;
    std::cout << "  - GCC 14+: 需要链接 -lstdc++_libbacktrace" << std::endl;
    std::cout << "  - MSVC 19.34+: 较好支持" << std::endl;
    std::cout << "  - Clang: 支持可能不完整" << std::endl;
    std::cout << std::endl;
    std::cout << "std::stacktrace 语法示例（供参考）：" << std::endl;
    std::cout << R"(
    #include <stacktrace>
    #include <iostream>

    void debug_function() {
        // 获取当前栈追踪
        auto trace = std::stacktrace::current();

        // 打印整个栈追踪
        std::cout << trace << std::endl;

        // 遍历每一帧
        for (const auto& entry : trace) {
            std::cout << entry.description() << std::endl;
            std::cout << "  文件: " << entry.source_file() << std::endl;
            std::cout << "  行号: " << entry.source_line() << std::endl;
        }
    }

    // 带栈追踪的异常
    class MyException : public std::exception {
        std::string msg_;
        std::stacktrace trace_;
    public:
        MyException(const std::string& msg)
            : msg_(msg), trace_(std::stacktrace::current()) {}
        const char* what() const noexcept override { return msg_.c_str(); }
        const std::stacktrace& trace() const { return trace_; }
    };
)" << std::endl;

    // 平台特定的替代方案
    std::cout << "平台特定替代方案：" << std::endl;
#if defined(__GNUC__) || defined(__clang__)
    std::cout << "  当前平台 (GCC/Clang): 可使用 <execinfo.h> 的 backtrace()" << std::endl;
#elif defined(_MSC_VER)
    std::cout << "  当前平台 (MSVC): 可使用 CaptureStackBackTrace()" << std::endl;
#endif
    std::cout << "  跨平台库: Boost.Stacktrace, backward-cpp" << std::endl;
#endif

    return 0;
}
```

## 参考链接

- [cppreference: std::stacktrace（栈追踪）](https://en.cppreference.com/w/cpp/utility/basic_stacktrace)
