---
sidebar_position: 8
title: "[[deprecated]]属性"
description: "C++14 [[deprecated]]属性 教学示例"
tags: [cpp14, deprecated_attribute]
---

# [[deprecated]]属性

C++14 标准引入的 [[deprecated]]属性 特性完整教学示例。

## 完整源码

```cpp title="cpp14/07_deprecated_attribute.cpp"
/**
 * ============================================================
 * 特性名称: [[deprecated]]属性
 * 所属标准: C++14
 * ============================================================
 *
 * [是什么]: [[deprecated]]是C++14引入的标准属性，用于标记实体（函数、类、
 *          变量、枚举、类型别名等）为已弃用。编译器在使用被标记的实体时
 *          会产生警告，可附带说明消息。
 *
 * [为什么引入]: 在C++11之前，标记弃用需要依赖编译器特有的扩展
 *              （如GCC的__attribute__((deprecated))，MSVC的__declspec(deprecated)）。
 *              [[deprecated]]提供了标准化的、跨平台的弃用标记方式，
 *              帮助库作者引导用户迁移到新API。
 *
 * [关键要点]:
 *   - [[deprecated]] 产生编译器弃用警告
 *   - [[deprecated("原因")]] 可附带说明消息
 *   - 可用于函数、类、变量、枚举值、类型别名、命名空间等
 *   - 被标记的代码仍然可以正常编译和运行，只是会产生警告
 * ============================================================
 */

#include <iostream>
#include <string>
#include <cmath>

// ============================================================
// 基础用法
// ============================================================

// 1. 标记弃用的函数（无消息）
[[deprecated]]
int old_function() {
    return 42;
}

// 2. 标记弃用的函数（带消息）
[[deprecated("请使用 new_calculate() 替代")]]
double old_calculate(double x) {
    return x * x;
}

// 新版替代函数
double new_calculate(double x) {
    return std::pow(x, 2);
}

// 3. 标记弃用的类
class [[deprecated("请使用 ModernLogger 替代")]] OldLogger {
public:
    void log(const std::string& msg) {
        std::cout << "    [OldLogger] " << msg << "\n";
    }
};

class ModernLogger {
public:
    void log(const std::string& msg) {
        std::cout << "    [ModernLogger] " << msg << "\n";
    }
};

// 4. 标记弃用的变量
[[deprecated("请使用 NEW_MAX_SIZE")]]
constexpr int OLD_MAX_SIZE = 1024;

constexpr int NEW_MAX_SIZE = 4096;

// 5. 标记弃用的类型别名
using ModernString = std::string;
[[deprecated("请使用 ModernString")]]
typedef std::string LegacyString;

void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 注意：以下使用弃用实体的代码会产生编译器警告
    // 为了演示，我们用注释说明效果，实际调用会触发警告

    // 调用弃用函数
    // int val = old_function();  // 警告: old_function is deprecated
    // double res = old_calculate(3.0);  // 警告: 请使用 new_calculate() 替代

    // 使用新版函数
    double res = new_calculate(3.0);
    std::cout << "  new_calculate(3.0) = " << res << "\n";

    // 使用弃用类
    // OldLogger logger;  // 警告: 请使用 ModernLogger 替代
    ModernLogger logger;
    logger.log("Hello from ModernLogger");

    // 使用弃用变量
    // int size = OLD_MAX_SIZE;  // 警告: 请使用 NEW_MAX_SIZE
    int size = NEW_MAX_SIZE;
    std::cout << "  NEW_MAX_SIZE = " << size << "\n";

    // 使用弃用类型别名
    // LegacyString s = "old";  // 警告: 请使用 ModernString
    ModernString s = "modern";
    std::cout << "  ModernString = \"" << s << "\"\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 弃用枚举值（只标记部分值）
enum class Color {
    Red,
    Green,
    Blue,
    Yellow [[deprecated("Yellow已弃用，请使用Gold")]] = 3,
    Gold = 4
};

// 2. 弃用模板特化（通过标记类实现）
template<typename T>
class DataProcessor {
public:
    void process() {
        std::cout << "    DataProcessor<T>::process()\n";
    }
};

// 弃用特定特化
template<>
class [[deprecated("float精度不足，请使用DataProcessor<double>")]]
DataProcessor<float> {
public:
    void process() {
        std::cout << "    DataProcessor<float>::process() [deprecated]\n";
    }
};

// 3. 弃用重载——引导用户使用更安全的版本
[[deprecated("原始指针不安全，请使用 string 版本")]]
void send_message(const char* msg) {
    std::cout << "    send(char*): " << msg << "\n";
}

void send_message(const std::string& msg) {
    std::cout << "    send(string): " << msg << "\n";
}

// 4. API版本演进示例
namespace api {
    namespace v1 {
        [[deprecated("v1 API 将在2.0版本移除，请迁移到 v2")]]
        void connect(const std::string& host) {
            std::cout << "    [v1] connecting to " << host << "\n";
        }
    }

    namespace v2 {
        struct ConnectionOptions {
            std::string host;
            int port;
            bool use_ssl;
        };

        void connect(const ConnectionOptions& opts) {
            std::cout << "    [v2] connecting to " << opts.host
                      << ":" << opts.port
                      << (opts.use_ssl ? " (SSL)" : "") << "\n";
        }
    }
}

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. 弃用枚举值
    std::cout << "  [弃用枚举值]\n";
    Color c1 = Color::Gold;  // OK，没有警告
    // Color c2 = Color::Yellow;  // 警告: Yellow已弃用，请使用Gold
    std::cout << "  使用 Color::Gold 代替 Color::Yellow\n";
    std::cout << "  Gold = " << static_cast<int>(c1) << "\n";

    // 2. 弃用模板特化
    std::cout << "\n  [弃用模板特化]\n";
    DataProcessor<double> dp_good;
    dp_good.process();  // OK
    // DataProcessor<float> dp_bad;  // 警告: float精度不足
    // dp_bad.process();
    std::cout << "  DataProcessor<double> 正常使用\n";
    std::cout << "  DataProcessor<float> 已弃用\n";

    // 3. 弃用重载
    std::cout << "\n  [弃用重载]\n";
    send_message(std::string("Hello"));  // 使用新版
    // send_message("Hello");  // 警告: 原始指针不安全

    // 4. API版本演进
    std::cout << "\n  [API版本演进]\n";
    // api::v1::connect("example.com");  // 警告: v1 API将被移除
    api::v2::connect({"example.com", 443, true});

    // 5. 编译期检查deprecated——不影响运行
    std::cout << "\n  [重要] deprecated只产生警告，不阻止编译\n";
    std::cout << "  被弃用的代码仍然可以正常编译和运行\n";
}

// ============================================================
// 易错点与陷阱
// ============================================================

// 陷阱演示类
class [[deprecated("使用 NewWidget")]] DeprecatedWidget {
public:
    int value;
    DeprecatedWidget() : value(0) {}
};

// 函数参数中使用弃用类型
void accept_widget(int val) {
    std::cout << "    accept_widget(" << val << ")\n";
}

void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱1：deprecated不阻止编译，只产生警告
    std::cout << "  陷阱1: deprecated只是警告，不是错误\n";
    std::cout << "    如果需要完全禁止使用，应该使用 = delete 或移除声明\n";
    std::cout << "    deprecated适合过渡期间引导用户迁移\n";

    // 陷阱2：属性位置很重要
    std::cout << "\n  陷阱2: [[deprecated]]的放置位置\n";
    std::cout << "    函数:   [[deprecated]] void foo();\n";
    std::cout << "    类:     class [[deprecated]] OldClass {};\n";
    std::cout << "    变量:   [[deprecated]] int old_var = 0;\n";
    std::cout << "    枚举值: enum { A, B [[deprecated]] };\n";
    std::cout << "    typedef: [[deprecated]] typedef int OldInt;\n";

    // 陷阱3：弃用消息字符串是编译器诊断信息的一部分
    std::cout << "\n  陷阱3: 弃用消息应清楚说明替代方案\n";
    std::cout << "    不好: [[deprecated]]\n";
    std::cout << "    好:   [[deprecated(\"请使用 newFunc()，将在v3.0移除\")]]\n";

    // 陷阱4：不同编译器的警告格式不同
    std::cout << "\n  陷阱4: 不同编译器的警告格式\n";
    std::cout << "    GCC:  warning: 'xxx' is deprecated: message\n";
    std::cout << "    MSVC: warning C4996: 'xxx': message\n";
    std::cout << "    Clang: warning: 'xxx' is deprecated: message\n";

    // 陷阱5：-Werror会将deprecated警告变为错误
    std::cout << "\n  陷阱5: 编译选项的影响\n";
    std::cout << "    -Werror 或 /WX 会将弃用警告提升为编译错误\n";
    std::cout << "    可以用 -Wno-deprecated-declarations 临时禁用弃用警告\n";

    // 陷阱6：弃用声明的作用域
    std::cout << "\n  陷阱6: deprecated不能用于语句或表达式\n";
    std::cout << "    只能用于声明（函数、类、变量、枚举值等）\n";
    std::cout << "    不能标记某个if分支或循环为deprecated\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== [[deprecated]]属性 - C++14 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: [[deprecated]]属性](https://en.cppreference.com/w/cpp/language/attributes/deprecated)
