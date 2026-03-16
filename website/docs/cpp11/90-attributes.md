---
sidebar_position: 91
title: "属性 ([[noreturn]]、[[carries_dependency]])"
description: "C++11 属性 ([[noreturn]]、[[carries_dependency]]) 教学示例"
tags: [cpp11, attributes]
---

# 属性 ([[noreturn]]、[[carries_dependency]])

C++11 标准引入的 属性 ([[noreturn]]、[[carries_dependency]]) 特性完整教学示例。

## 完整源码

```cpp title="cpp11/90_attributes.cpp"
/**
 * ============================================================
 * 特性名称: 属性 ([[noreturn]]、[[carries_dependency]])
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: C++11引入了标准化的属性语法 [[attribute]]，用于向编译器提供
 *           额外的元信息。C++11定义了两个标准属性：[[noreturn]] 表示函数
 *           不会返回，[[carries_dependency]] 用于优化内存序依赖传递。
 *
 * [为什么引入]: 之前编译器使用各自的扩展（如 __attribute__((noreturn))、
 *               __declspec(noreturn)）来标注函数属性。标准化的属性语法
 *               提供了统一的、可移植的方式。
 *
 * [关键要点]:
 *   - [[noreturn]]: 标记永远不会返回的函数（如 exit、throw、无限循环）
 *   - [[carries_dependency]]: 与内存序优化相关，极少使用
 *   - 属性语法 [[...]] 可以应用于函数、变量、类型等
 *   - 编译器可以利用属性信息进行优化和诊断
 *   - C++14/17/20 后续添加了更多标准属性
 * ============================================================
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

// ============================================================
// 辅助定义
// ============================================================

// [[noreturn]] 标记函数永远不会返回
[[noreturn]] void throw_error(const std::string& msg) {
    throw std::runtime_error(msg);
}

[[noreturn]] void fatal_exit(const std::string& msg) {
    std::cerr << "致命错误: " << msg << std::endl;
    std::abort();
}

[[noreturn]] void infinite_loop() {
    while (true) {
        // 无限循环，永不返回
        // 实际中这里会做有意义的事情（如事件循环）
        break;  // 这里加 break 是为了演示代码能编译，实际不应有
    }
    // 注意：如果编译器发现可能返回，会给出警告
    // 这里为了编译通过加了 abort
    std::abort();
}

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. [[noreturn]] 属性
    std::cout << "[1] [[noreturn]] 属性:\n";
    {
        std::cout << "  [[noreturn]] 告诉编译器函数永远不会正常返回\n";
        std::cout << "  常见场景:\n";
        std::cout << "    - 抛出异常的函数\n";
        std::cout << "    - 调用 exit/abort 的函数\n";
        std::cout << "    - 无限循环的函数（如事件主循环）\n\n";

        // 使用 [[noreturn]] 函数
        try {
            std::cout << "  调用 throw_error...\n";
            throw_error("测试错误");  // 永不返回，抛出异常
        } catch (const std::runtime_error& e) {
            std::cout << "  捕获异常: " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;

    // 2. [[noreturn]] 的声明方式
    std::cout << "[2] [[noreturn]] 的声明语法:\n";
    {
        std::cout << "  // 函数声明:\n";
        std::cout << "  // [[noreturn]] void my_exit();\n\n";
        std::cout << "  // 函数定义:\n";
        std::cout << "  // [[noreturn]] void my_exit() {\n";
        std::cout << "  //     std::exit(1);\n";
        std::cout << "  // }\n\n";
        std::cout << "  // 也可以放在返回类型前:\n";
        std::cout << "  // [[noreturn]] void throw_always() { throw 1; }\n";
    }
    std::cout << std::endl;

    // 3. [[noreturn]] 帮助编译器优化
    std::cout << "[3] [[noreturn]] 帮助编译器优化:\n";
    {
        std::cout << "  编译器知道函数不会返回后，可以:\n";
        std::cout << "    1) 不生成调用后的代码\n";
        std::cout << "    2) 优化控制流分析\n";
        std::cout << "    3) 消除 '控制可能到达非void函数末尾' 的警告\n\n";

        // 示例：在 switch 中使用
        auto classify = [](int value) -> std::string {
            switch (value) {
                case 1: return "一";
                case 2: return "二";
                case 3: return "三";
                default:
                    throw_error("未知值: " + std::to_string(value));
                    // 编译器知道 throw_error 不会返回
                    // 所以不需要在 default 分支后加 return
            }
        };

        try {
            std::cout << "  classify(1) = " << classify(1) << std::endl;
            std::cout << "  classify(2) = " << classify(2) << std::endl;
            classify(99);
        } catch (const std::runtime_error& e) {
            std::cout << "  classify(99) 异常: " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;

    // 4. 属性语法概览
    std::cout << "[4] C++属性语法概览:\n";
    {
        std::cout << "  [[attribute]]          // 基本语法\n";
        std::cout << "  [[attr1, attr2]]       // 多个属性\n";
        std::cout << "  [[namespace::attr]]    // 带命名空间的属性\n";
        std::cout << "  [[attr(args)]]         // 带参数的属性\n\n";

        std::cout << "  属性可以应用于:\n";
        std::cout << "    - 函数声明/定义\n";
        std::cout << "    - 变量声明\n";
        std::cout << "    - 类/结构体声明\n";
        std::cout << "    - 语句\n";
        std::cout << "    - 枚举值\n";
    }
    std::cout << std::endl;

    // 5. [[carries_dependency]] 简介
    std::cout << "[5] [[carries_dependency]] 简介:\n";
    {
        std::cout << "  [[carries_dependency]] 用于原子操作的依赖链优化\n";
        std::cout << "  在 memory_order_consume 语义中使用\n";
        std::cout << "  告诉编译器依赖关系会通过函数参数/返回值传递\n";
        std::cout << "  从而避免不必要的内存栅栏指令\n\n";
        std::cout << "  // 示例:\n";
        std::cout << "  // int* acquire_data() [[carries_dependency]] {\n";
        std::cout << "  //     return ptr.load(std::memory_order_consume);\n";
        std::cout << "  // }\n\n";
        std::cout << "  注意: 实际中极少直接使用，大多数编译器对\n";
        std::cout << "  memory_order_consume 的支持有限\n";
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. [[noreturn]] 用于断言和不可达代码
    std::cout << "[1] [[noreturn]] 用于断言和不可达代码:\n";
    {
        // 自定义断言
        struct Assert {
            [[noreturn]] static void fail(const char* expr,
                                          const char* file, int line) {
                std::cerr << "断言失败: " << expr
                          << " 在 " << file << ":" << line << std::endl;
                std::abort();
            }
        };

        // 宏定义（仅展示概念）
        // #define MY_ASSERT(expr) \
        //     ((expr) ? (void)0 : Assert::fail(#expr, __FILE__, __LINE__))

        std::cout << "  自定义断言函数使用 [[noreturn]]\n";
        std::cout << "  编译器知道断言失败后不会继续执行\n";
    }
    std::cout << std::endl;

    // 2. [[noreturn]] 与异常层次
    std::cout << "[2] [[noreturn]] 与异常处理:\n";
    {
        class ErrorHandler {
        public:
            [[noreturn]] static void throwLogicError(const std::string& msg) {
                // 记录日志
                std::cerr << "[逻辑错误] " << msg << std::endl;
                throw std::logic_error(msg);
            }

            [[noreturn]] static void throwRuntimeError(const std::string& msg) {
                std::cerr << "[运行时错误] " << msg << std::endl;
                throw std::runtime_error(msg);
            }
        };

        try {
            ErrorHandler::throwLogicError("参数不合法");
        } catch (const std::exception& e) {
            std::cout << "  捕获: " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;

    // 3. 标准库中的 [[noreturn]] 函数
    std::cout << "[3] 标准库中的 [[noreturn]] 函数:\n";
    {
        std::cout << "  以下标准库函数被标记为 [[noreturn]]:\n";
        std::cout << "    std::abort()          // 异常终止\n";
        std::cout << "    std::exit(int)        // 正常终止\n";
        std::cout << "    std::_Exit(int)       // 快速终止\n";
        std::cout << "    std::quick_exit(int)  // 快速终止\n";
        std::cout << "    std::terminate()      // 未处理异常终止\n";
        std::cout << "    std::unexpected()     // (C++11, 已弃用)\n";
        std::cout << "    std::rethrow_exception()  // 重新抛出\n";
        std::cout << "    std::throw_with_nested()  // 嵌套异常\n";
    }
    std::cout << std::endl;

    // 4. 各编译器之前的写法对比
    std::cout << "[4] 属性标准化前后对比:\n";
    {
        std::cout << "  C++11之前（不可移植）:\n";
        std::cout << "    GCC:   __attribute__((noreturn)) void fn();\n";
        std::cout << "    MSVC:  __declspec(noreturn) void fn();\n";
        std::cout << "    Clang: __attribute__((noreturn)) void fn();\n\n";
        std::cout << "  C++11之后（标准、可移植）:\n";
        std::cout << "    [[noreturn]] void fn();\n\n";
        std::cout << "  标准属性的优势:\n";
        std::cout << "    - 跨编译器统一语法\n";
        std::cout << "    - 未知属性会被忽略（不报错）\n";
        std::cout << "    - 语义清晰、位置固定\n";
    }
    std::cout << std::endl;

    // 5. 后续标准添加的属性预览
    std::cout << "[5] 后续标准添加的属性预览:\n";
    {
        std::cout << "  C++14:\n";
        std::cout << "    [[deprecated]]              // 标记为已弃用\n";
        std::cout << "    [[deprecated(\"原因\")]]    // 带说明的弃用\n\n";

        std::cout << "  C++17:\n";
        std::cout << "    [[fallthrough]]             // switch贯穿\n";
        std::cout << "    [[nodiscard]]               // 不应忽略返回值\n";
        std::cout << "    [[maybe_unused]]            // 可能未使用\n\n";

        std::cout << "  C++20:\n";
        std::cout << "    [[likely]] / [[unlikely]]   // 分支预测提示\n";
        std::cout << "    [[no_unique_address]]       // 空基类优化\n";
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. [[noreturn]] 函数如果实际返回是未定义行为
    std::cout << "[1] [[noreturn]] 函数如果返回是未定义行为:\n";
    std::cout << "  // [[noreturn]] void bad() {\n";
    std::cout << "  //     // 什么都不做，实际会返回\n";
    std::cout << "  // }  // 未定义行为！\n";
    std::cout << "  编译器可能会给出警告，但不保证\n\n";

    // 2. [[noreturn]] 不能用于有返回值的函数
    std::cout << "[2] [[noreturn]] 的函数返回类型通常是 void:\n";
    std::cout << "  [[noreturn]] 虽然语法上可以用在非void函数上，\n";
    std::cout << "  但这样做没有意义（函数不会返回，返回类型无用）\n";
    std::cout << "  惯例: [[noreturn]] void fn();\n\n";

    // 3. [[noreturn]] 放置位置
    std::cout << "[3] [[noreturn]] 放置在声明最开头:\n";
    std::cout << "  // 正确:\n";
    std::cout << "  // [[noreturn]] void fn();\n\n";
    std::cout << "  // 也正确 (某些编译器支持):\n";
    std::cout << "  // void fn() [[noreturn]];\n\n";
    std::cout << "  // 推荐放在最前面，最清晰\n\n";

    // 4. 声明和定义都要有属性
    std::cout << "[4] 属性应在声明中指定:\n";
    std::cout << "  // 头文件中声明:\n";
    std::cout << "  // [[noreturn]] void error_exit();\n\n";
    std::cout << "  // 源文件中定义:\n";
    std::cout << "  // [[noreturn]] void error_exit() { std::abort(); }\n";
    std::cout << "  // (定义处也建议加上属性以保持一致)\n\n";

    // 5. 未知属性的处理
    std::cout << "[5] 未知属性会被忽略:\n";
    std::cout << "  // [[unknown_attribute]] void fn();  // 编译器忽略\n";
    std::cout << "  // 这是设计特性：允许代码使用未来标准的属性\n";
    std::cout << "  // 同时在当前编译器上也能编译\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 属性 ([[noreturn]]/[[carries_dependency]]) - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 属性 ([[noreturn]]、[[carries_dependency]])](https://en.cppreference.com/w/cpp/language/attributes)
