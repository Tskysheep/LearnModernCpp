/**
 * ============================================================
 * 特性名称: std::source_location
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: std::source_location 提供了获取源代码位置信息的标准方式，
 *          包括文件名、行号、列号和函数名。
 *
 * [为什么引入]: 替代传统的 __FILE__、__LINE__、__func__ 预处理宏，
 *             提供更安全、更灵活、更 C++ 化的方式。
 *             特别是在日志和断言中使用时，不需要宏展开。
 *
 * [关键要点]:
 *   - source_location::current() 获取调用点的信息
 *   - 提供 file_name()、line()、column()、function_name()
 *   - 可以作为函数默认参数使用（关键用法）
 *   - 替代 __FILE__ 和 __LINE__ 宏
 * ============================================================
 */

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <source_location>

// ===================== basic_usage() =====================

// 基本使用：获取当前位置
void print_location(const std::source_location& loc = std::source_location::current()) {
    std::cout << "  文件: " << loc.file_name() << "\n";
    std::cout << "  行号: " << loc.line() << "\n";
    std::cout << "  列号: " << loc.column() << "\n";
    std::cout << "  函数: " << loc.function_name() << "\n";
}

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 直接获取当前位置
    auto loc = std::source_location::current();
    std::cout << "当前位置:\n";
    std::cout << "  文件: " << loc.file_name() << "\n";
    std::cout << "  行号: " << loc.line() << "\n";
    std::cout << "  函数: " << loc.function_name() << "\n";

    // 作为默认参数使用（关键用法！）
    // print_location 的默认参数会自动捕获调用点的位置
    std::cout << "\n调用 print_location() (自动捕获调用点):\n";
    print_location();  // 这里的行号是调用点，不是 print_location 内部

    std::cout << "\n与传统宏对比:\n";
    std::cout << "  __FILE__: " << __FILE__ << "\n";
    std::cout << "  __LINE__: " << __LINE__ << "\n";
    // __func__ 只在函数内部可用
    std::cout << "  __func__: " << __func__ << "\n";
}

// ===================== advanced_usage() =====================

// 日志系统
enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

std::string_view level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "调试";
        case LogLevel::INFO:    return "信息";
        case LogLevel::WARNING: return "警告";
        case LogLevel::ERROR:   return "错误";
    }
    return "未知";
}

// 日志函数：自动获取调用位置
void log(LogLevel level, std::string_view message,
         const std::source_location& loc = std::source_location::current()) {
    std::cout << "[" << level_to_string(level) << "] "
              << loc.file_name() << ":" << loc.line()
              << " (" << loc.function_name() << "): "
              << message << "\n";
}

// 断言替代
void my_assert(bool condition, std::string_view message = "",
               const std::source_location& loc = std::source_location::current()) {
    if (!condition) {
        std::cout << "断言失败! "
                  << loc.file_name() << ":" << loc.line()
                  << " (" << loc.function_name() << ")";
        if (!message.empty()) {
            std::cout << " - " << message;
        }
        std::cout << "\n";
    }
}

// 函数追踪器
class FunctionTracer {
public:
    explicit FunctionTracer(const std::source_location& loc = std::source_location::current())
        : location_(loc) {
        std::cout << ">>> 进入 " << location_.function_name()
                  << " (" << location_.file_name() << ":" << location_.line() << ")\n";
    }

    ~FunctionTracer() {
        std::cout << "<<< 离开 " << location_.function_name() << "\n";
    }

private:
    std::source_location location_;
};

// 性能检测器的简化版
class ScopeTimer {
public:
    explicit ScopeTimer(const std::source_location& loc = std::source_location::current())
        : location_(loc) {
        std::cout << "[计时开始] " << location_.function_name()
                  << " 行 " << location_.line() << "\n";
    }

    ~ScopeTimer() {
        std::cout << "[计时结束] " << location_.function_name()
                  << " 行 " << location_.line() << "\n";
    }

private:
    std::source_location location_;
};

void do_some_work() {
    FunctionTracer tracer;  // 自动记录进入和离开
    std::cout << "  正在执行工作...\n";
}

void process_data() {
    FunctionTracer tracer;
    std::cout << "  处理数据...\n";
    do_some_work();
    std::cout << "  数据处理完成\n";
}

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 日志系统演示
    std::cout << "-- 日志系统 --\n";
    log(LogLevel::INFO, "程序启动");
    log(LogLevel::DEBUG, "加载配置文件");
    log(LogLevel::WARNING, "配置项缺失，使用默认值");
    log(LogLevel::ERROR, "连接数据库失败");

    // 断言演示
    std::cout << "\n-- 断言 --\n";
    my_assert(1 + 1 == 2, "基本数学检查");
    my_assert(1 + 1 == 3, "这个会失败");

    // 函数追踪
    std::cout << "\n-- 函数追踪 --\n";
    process_data();

    // 在Lambda中使用
    std::cout << "\n-- Lambda 中使用 --\n";
    auto lambda_log = [](std::string_view msg,
                          const std::source_location& loc = std::source_location::current()) {
        std::cout << "[Lambda] " << loc.function_name() << ":" << loc.line()
                  << " - " << msg << "\n";
    };

    lambda_log("从 Lambda 记录日志");

    // 存储位置信息
    std::cout << "\n-- 存储位置信息 --\n";
    struct LogEntry {
        LogLevel level;
        std::string message;
        std::source_location location;
    };

    std::vector<LogEntry> log_entries;
    auto add_log = [&log_entries](LogLevel level, std::string message,
                                   const std::source_location& loc = std::source_location::current()) {
        log_entries.push_back({level, std::move(message), loc});
    };

    add_log(LogLevel::INFO, "第一条日志");
    add_log(LogLevel::WARNING, "第二条日志");

    std::cout << "已记录 " << log_entries.size() << " 条日志:\n";
    for (const auto& entry : log_entries) {
        std::cout << "  [" << level_to_string(entry.level) << "] "
                  << entry.location.line() << ": " << entry.message << "\n";
    }
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：source_location::current() 在参数默认值中的行为
    std::cout << "1. current() 在不同位置的行为:\n";
    std::cout << "   作为默认参数: 捕获调用点的位置（通常是你想要的）\n";
    std::cout << "   在函数体内: 捕获函数体内该行的位置（通常不是你想要的）\n";
    std::cout << "   关键：必须放在默认参数中，不能放在函数体里!\n\n";

    // 演示：错误用法
    auto bad_log = [](std::string_view msg) {
        auto loc = std::source_location::current();  // 捕获的是这里的位置!
        std::cout << "   [错误方式] " << loc.function_name() << ":" << loc.line()
                  << " - " << msg << "\n";
    };

    auto good_log = [](std::string_view msg,
                        const std::source_location& loc = std::source_location::current()) {
        std::cout << "   [正确方式] " << loc.function_name() << ":" << loc.line()
                  << " - " << msg << "\n";
    };

    bad_log("这里的行号是函数体内部的行号");
    good_log("这里的行号是调用点的行号");

    // 陷阱2：与宏的交互
    std::cout << "\n2. source_location 不受宏展开影响:\n";
    std::cout << "   宏 __FILE__ 和 __LINE__ 在预处理阶段展开\n";
    std::cout << "   source_location 在编译阶段处理\n";
    std::cout << "   两者可能给出不同的结果\n\n";

    // 陷阱3：source_location 大小
    std::cout << "3. source_location 的大小:\n";
    std::cout << "   sizeof(source_location) = " << sizeof(std::source_location) << " 字节\n";
    std::cout << "   它存储的是指针（指向编译器生成的字符串表），通常很小\n\n";

    // 陷阱4：函数名的格式
    std::cout << "4. function_name() 返回的格式因编译器而异:\n";
    std::cout << "   GCC: 可能包含参数类型和返回类型\n";
    std::cout << "   MSVC: 通常只包含函数名\n";
    std::cout << "   不应该解析函数名字符串来提取信息\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== std::source_location - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
