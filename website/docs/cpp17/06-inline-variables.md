---
sidebar_position: 7
title: "inline 变量 (Inline Variables)"
description: "C++17 inline 变量 (Inline Variables) 教学示例"
tags: [cpp17, inline_variables]
---

# inline 变量 (Inline Variables)

C++17 标准引入的 inline 变量 (Inline Variables) 特性完整教学示例。

## 完整源码

```cpp title="cpp17/06_inline_variables.cpp"
/**
 * ============================================================
 * 特性名称: inline 变量 (Inline Variables)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: 允许在头文件中使用 inline 关键字定义全局变量和静态
 *          成员变量，保证在多个翻译单元中只有一个实体（ODR 安全）。
 *
 * [为什么引入]: C++14 中，头文件里的全局变量如果被多个 .cpp 包含，
 *              会导致链接器报「重复定义」错误（违反 ODR，
 *              One Definition Rule）。inline 变量解决了这个问题，
 *              让头文件中的常量定义变得简单。
 *
 * [关键要点]:
 *   - inline 变量可以在头文件中定义，被多个翻译单元包含不会重复
 *   - constexpr 静态成员在 C++17 中隐式 inline
 *   - 解决了以前 static const 成员需要在 .cpp 中定义的麻烦
 *   - 与 static 变量的区别：static 每个翻译单元有自己的副本，
 *     inline 所有翻译单元共享同一个实体
 * ============================================================
 */

#include <iostream>
#include <string>

// ============================================================
// 模拟头文件中的 inline 变量
// ============================================================

// C++14 方式：在头文件中声明，需要在某个 .cpp 中定义
// extern const int MAX_SIZE;  // 头文件中声明
// const int MAX_SIZE = 100;   // 某个 .cpp 中定义

// C++17 方式：直接在头文件中定义
inline constexpr int MAX_SIZE = 100;
inline const std::string APP_NAME = "MyApp";
inline int global_counter = 0;  // 可变的 inline 变量

// 类中的 static 成员
struct Config {
    // C++14: 需要在类外某个 .cpp 中定义
    // static const int version;  // 声明
    // const int Config::version = 1;  // .cpp 中定义

    // C++17: 直接在类内定义
    static inline int version = 1;
    static inline const std::string name = "Config";

    // constexpr 静态成员在 C++17 中隐式 inline
    static constexpr double PI = 3.14159265358979;
    static constexpr int MAX_RETRY = 3;
};

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 使用 inline 常量
    std::cout << "MAX_SIZE = " << MAX_SIZE << "\n";
    std::cout << "APP_NAME = " << APP_NAME << "\n";

    // 2. 使用可变的 inline 变量
    global_counter++;
    std::cout << "global_counter = " << global_counter << "\n";

    // 3. 使用类的 static inline 成员
    std::cout << "Config::version = " << Config::version << "\n";
    std::cout << "Config::name = " << Config::name << "\n";
    std::cout << "Config::PI = " << Config::PI << "\n";
    std::cout << "Config::MAX_RETRY = " << Config::MAX_RETRY << "\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 单例模式：inline 变量让头文件中的单例更简洁
class Logger {
public:
    static Logger& instance() {
        static Logger logger;  // C++11 保证线程安全
        return logger;
    }

    void log(const std::string& msg) {
        std::cout << "[LOG] " << msg << "\n";
    }

private:
    Logger() = default;
};

// C++17: 也可以用 inline 变量提供全局访问点
inline Logger& logger = Logger::instance();

// 模板类的 static 成员 —— C++17 前需要在头文件中单独定义
template<typename T>
struct TypeInfo {
    static inline int instance_count = 0;  // C++17: 直接初始化

    TypeInfo() { ++instance_count; }
    ~TypeInfo() { --instance_count; }
};

// 用 inline 变量做编译期查找表
inline constexpr int fibonacci[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. 单例通过 inline 变量访问
    logger.log("通过 inline 变量访问单例");

    // 2. 模板类的 static inline 成员
    {
        TypeInfo<int> a, b, c;
        std::cout << "TypeInfo<int> 实例数: "
                  << TypeInfo<int>::instance_count << "\n";

        TypeInfo<double> d;
        std::cout << "TypeInfo<double> 实例数: "
                  << TypeInfo<double>::instance_count << "\n";
    }
    std::cout << "析构后 TypeInfo<int> 实例数: "
              << TypeInfo<int>::instance_count << "\n";

    // 3. constexpr 查找表
    std::cout << "fibonacci[7] = " << fibonacci[7] << "\n";

    // 4. 修改类的 static inline 成员
    Config::version = 2;
    std::cout << "修改后 Config::version = " << Config::version << "\n";
}

// ============================================================
// 易错点 / 陷阱
// ============================================================

// static vs inline 的区别演示
// 注意：在实际项目中这两个变量会在头文件中
static int static_var = 0;           // 每个翻译单元有自己的副本
inline int inline_var = 0;           // 所有翻译单元共享同一个

void modify_in_func1() {
    static_var++;
    inline_var++;
}

void modify_in_func2() {
    static_var++;
    inline_var++;
}

void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: static vs inline
    //   在单个翻译单元中看不出区别，
    //   但在多个 .cpp 中，static 各有副本，inline 共享
    modify_in_func1();
    modify_in_func2();
    std::cout << "陷阱1 - 单文件中 static_var=" << static_var
              << ", inline_var=" << inline_var
              << " (单文件看不出区别)\n";
    std::cout << "  多文件时: static 各有副本, inline 全局共享\n";

    // 陷阱 2: inline 变量的初始化顺序
    //   不同翻译单元中 inline 变量的初始化顺序未定义（static init order fiasco）
    //   与普通全局变量一样的问题
    std::cout << "陷阱2 - inline 变量跨翻译单元的初始化顺序仍然未定义\n";

    // 陷阱 3: 可变 inline 变量的线程安全
    //   inline int counter = 0; 在多线程中不安全
    //   需要用 atomic 或加锁
    std::cout << "陷阱3 - 可变 inline 变量不是线程安全的\n";

    // 陷阱 4: constexpr 在 C++17 中隐式 inline
    //   static constexpr int x = 5; 在 C++14 中可能需要类外定义
    //   C++17 中不再需要
    std::cout << "陷阱4 - C++17 中 static constexpr 隐式 inline，无需类外定义\n";

    // 陷阱 5: inline 变量必须在每个翻译单元中有相同的定义
    //   不同头文件中定义不同值的同名 inline 变量是 UB
    std::cout << "陷阱5 - 不同翻译单元的 inline 变量定义必须完全相同\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== inline 变量 - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: inline 变量 (Inline Variables)](https://en.cppreference.com/w/cpp/language/inline)
