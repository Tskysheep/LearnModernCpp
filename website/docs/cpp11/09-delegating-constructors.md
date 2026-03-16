---
sidebar_position: 10
slug: 09-delegating-constructors
title: "委托构造函数 (Delegating Constructors)"
description: "C++11 委托构造函数 (Delegating Constructors) 教学示例"
tags: [cpp11, delegating_constructors]
---

# 委托构造函数 (Delegating Constructors)

C++11 标准引入的 委托构造函数 (Delegating Constructors) 特性完整教学示例。

## 完整源码

```cpp title="cpp11/09_delegating_constructors.cpp"
/**
 * ============================================================
 * 特性名称: 委托构造函数 (Delegating Constructors)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: 允许一个构造函数调用同类的另一个构造函数，
 *          形成构造函数链，避免初始化代码重复。
 *
 * [为什么引入]: C++03中多个构造函数共享初始化逻辑只能通过
 *   提取私有init()函数来实现，但init()不能初始化const成员、
 *   引用成员或基类。委托构造函数直接在初始化列表层面解决复用问题。
 *
 * [关键要点]:
 *   - 委托构造函数在成员初始化列表中调用目标构造函数
 *   - 不能同时委托和初始化其他成员（二者互斥）
 *   - 目标构造函数先执行完毕，再执行委托构造函数的函数体
 *   - 要避免循环委托（编译器通常会检测）
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// ============================================================
// 基础用法
// ============================================================

// C++03风格：使用私有init函数避免重复
class OldStyle {
public:
    OldStyle() { init(0, "默认", 0.0); }
    OldStyle(int id) { init(id, "默认", 0.0); }
    OldStyle(int id, const std::string& name) { init(id, name, 0.0); }
    OldStyle(int id, const std::string& name, double score) { init(id, name, score); }

    void print() const {
        std::cout << "  OldStyle[" << id_ << "]: " << name_ << ", " << score_ << std::endl;
    }

private:
    void init(int id, const std::string& name, double score) {
        id_ = id;
        name_ = name;
        score_ = score;
        // 注意: init()不能初始化const成员或引用成员!
    }
    int id_;
    std::string name_;
    double score_;
};

// C++11风格：使用委托构造函数
class Student {
public:
    // 主构造函数（最完整的版本）
    Student(int id, const std::string& name, double score)
        : id_(id), name_(name), score_(score) {
        std::cout << "  主构造函数: Student(" << id_ << ", "
                  << name_ << ", " << score_ << ")" << std::endl;
        validate();
    }

    // 委托构造函数们
    Student() : Student(0, "未命名", 0.0) {
        std::cout << "  默认构造委托完成" << std::endl;
    }

    Student(int id) : Student(id, "未命名", 0.0) {
        std::cout << "  单参数构造委托完成" << std::endl;
    }

    Student(int id, const std::string& name) : Student(id, name, 0.0) {
        std::cout << "  双参数构造委托完成" << std::endl;
    }

    void print() const {
        std::cout << "  Student[" << id_ << "]: " << name_
                  << ", 成绩=" << score_ << std::endl;
    }

private:
    void validate() {
        if (id_ < 0) throw std::invalid_argument("学号不能为负");
        if (score_ < 0 || score_ > 100) throw std::invalid_argument("成绩必须在0-100之间");
    }

    int id_;
    std::string name_;
    double score_;
};

void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // C++03风格
    std::cout << "C++03风格 (init函数):" << std::endl;
    OldStyle o1;
    o1.print();
    OldStyle o2(1, "张三", 95.5);
    o2.print();

    // C++11委托构造函数
    std::cout << "\nC++11风格 (委托构造):" << std::endl;
    std::cout << "--- 创建s1 ---" << std::endl;
    Student s1;
    s1.print();

    std::cout << "--- 创建s2 ---" << std::endl;
    Student s2(1001);
    s2.print();

    std::cout << "--- 创建s3 ---" << std::endl;
    Student s3(1002, "李四");
    s3.print();

    std::cout << "--- 创建s4 ---" << std::endl;
    Student s4(1003, "王五", 92.5);
    s4.print();
}

// ============================================================
// 进阶用法
// ============================================================

// 委托构造 + 异常处理
class Connection {
public:
    Connection(const std::string& host, int port, int timeout)
        : host_(host), port_(port), timeout_(timeout), connected_(false) {
        std::cout << "  连接到 " << host_ << ":" << port_
                  << " (超时" << timeout_ << "ms)" << std::endl;
        connected_ = true;
    }

    // 默认超时
    Connection(const std::string& host, int port)
        : Connection(host, port, 5000) {
        std::cout << "  使用默认超时" << std::endl;
    }

    // 默认端口和超时
    Connection(const std::string& host)
        : Connection(host, 80) {
        std::cout << "  使用默认端口" << std::endl;
    }

    // 三层委托链: Connection("host") -> Connection("host", 80) -> Connection("host", 80, 5000)

    void status() const {
        std::cout << "  状态: " << host_ << ":" << port_
                  << " 已连接=" << (connected_ ? "是" : "否") << std::endl;
    }

private:
    std::string host_;
    int port_;
    int timeout_;
    bool connected_;
};

// 委托构造函数与const成员
class Config {
public:
    // 主构造函数 —— 可以初始化const成员!（init函数做不到这点）
    Config(const std::string& name, int version, bool debug)
        : name_(name), version_(version), debug_(debug) {}

    Config(const std::string& name, int version)
        : Config(name, version, false) {}

    Config(const std::string& name)
        : Config(name, 1, false) {}

    Config() : Config("默认配置") {}

    void print() const {
        std::cout << "  配置: " << name_ << " v" << version_
                  << (debug_ ? " [调试]" : " [发布]") << std::endl;
    }

private:
    const std::string name_;   // const成员!
    const int version_;        // const成员!
    bool debug_;
};

void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 委托链
    std::cout << "三层委托链:" << std::endl;
    Connection conn("example.com");
    conn.status();

    // 2. 初始化const成员
    std::cout << "\n初始化const成员:" << std::endl;
    Config c1;
    c1.print();
    Config c2("我的应用", 2, true);
    c2.print();

    // 3. 异常安全 —— 委托构造函数中的异常
    std::cout << "\n异常处理:" << std::endl;
    try {
        Student bad(-1, "错误学生", 50); // id为负数
    } catch (const std::invalid_argument& e) {
        std::cout << "  捕获异常: " << e.what() << std::endl;
    }
}

// ============================================================
// 易错点与陷阱
// ============================================================

class BadExample {
public:
    // 主构造函数
    BadExample(int a, int b) : a_(a), b_(b) {
        std::cout << "  BadExample(" << a_ << ", " << b_ << ")" << std::endl;
    }

    // 错误! 不能同时委托和初始化其他成员
    // BadExample(int a) : BadExample(a, 0), extra_(100) {} // 编译错误!

    // 正确做法：在委托构造函数的函数体中处理额外逻辑
    BadExample(int a) : BadExample(a, 0) {
        extra_ = 100; // 在函数体中赋值
        std::cout << "  额外初始化: extra_=" << extra_ << std::endl;
    }

    void print() const {
        std::cout << "  a=" << a_ << ", b=" << b_ << ", extra=" << extra_ << std::endl;
    }

private:
    int a_;
    int b_;
    int extra_ = 0; // 类内初始化作为后备
};

void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: 不能同时委托和初始化成员
    std::cout << "陷阱1: 委托和成员初始化列表互斥" << std::endl;
    BadExample be(42);
    be.print();

    // 陷阱2: 循环委托
    // class Circular {
    //     Circular(int x) : Circular(x, 0) {}
    //     Circular(int x, int y) : Circular(x) {} // 循环! 未定义行为
    // };
    std::cout << "\n陷阱2: 循环委托会导致无限递归（编译器可能检测不到）" << std::endl;

    // 陷阱3: 委托构造函数的执行顺序
    std::cout << "\n陷阱3: 执行顺序" << std::endl;
    std::cout << "  1. 先执行目标构造函数的完整流程" << std::endl;
    std::cout << "  2. 然后执行委托构造函数的函数体" << std::endl;
    std::cout << "  如果目标构造函数抛出异常，委托构造函数的函数体不会执行" << std::endl;

    // 陷阱4: 委托构造函数中的异常
    std::cout << "\n陷阱4: 异常安全" << std::endl;
    std::cout << "  如果目标构造函数成功，但委托构造函数的函数体抛出异常，" << std::endl;
    std::cout << "  则析构函数会被调用（因为目标构造函数已完成，对象已构造）" << std::endl;

    // 最佳实践
    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 让最完整的构造函数作为主构造函数" << std::endl;
    std::cout << "  2. 其他构造函数委托到主构造函数" << std::endl;
    std::cout << "  3. 把验证逻辑放在主构造函数中（只写一次）" << std::endl;
    std::cout << "  4. 配合类内成员初始化使用效果更好" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: 委托构造函数" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 委托构造函数 (Delegating Constructors)](https://en.cppreference.com/w/cpp/language/constructor)
