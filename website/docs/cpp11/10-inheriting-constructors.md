---
sidebar_position: 11
slug: 10-inheriting-constructors
title: "继承构造函数 (Inheriting Constructors)"
description: "C++11 继承构造函数 (Inheriting Constructors) 教学示例"
tags: [cpp11, inheriting_constructors]
---

# 继承构造函数 (Inheriting Constructors)

C++11 标准引入的 继承构造函数 (Inheriting Constructors) 特性完整教学示例。

## 完整源码

```cpp title="cpp11/10_inheriting_constructors.cpp"
/**
 * ============================================================
 * 特性名称: 继承构造函数 (Inheriting Constructors)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: 使用 using Base::Base; 声明，派生类可以自动继承
 *          基类的所有构造函数，无需逐一转发。
 *
 * [为什么引入]: C++03中，如果基类有多个构造函数，派生类需要为
 *   每个都写一个转发构造函数，这是纯粹的样板代码。继承构造函数
 *   消除了这种繁琐的重复。
 *
 * [关键要点]:
 *   - 语法: using Base::Base;
 *   - 继承的构造函数不会初始化派生类新增的成员（需用类内初始化）
 *   - 如果派生类定义了与基类签名相同的构造函数，则派生类版本优先
 *   - 默认构造、拷贝构造、移动构造不会被继承
 * ============================================================
 */

#include <iostream>
#include <string>

// ============================================================
// 基类：有多个构造函数
// ============================================================
class Base {
public:
    Base() : id_(0), name_("默认") {
        std::cout << "  Base() 默认构造" << std::endl;
    }

    explicit Base(int id) : id_(id), name_("无名") {
        std::cout << "  Base(int): id=" << id_ << std::endl;
    }

    Base(int id, const std::string& name) : id_(id), name_(name) {
        std::cout << "  Base(int, string): id=" << id_ << ", name=" << name_ << std::endl;
    }

    Base(int id, const std::string& name, double score)
        : id_(id), name_(name) {
        std::cout << "  Base(int, string, double): id=" << id_
                  << ", name=" << name_ << ", score=" << score << std::endl;
    }

    virtual void print() const {
        std::cout << "  Base[" << id_ << "]: " << name_ << std::endl;
    }

    int id() const { return id_; }
    const std::string& name() const { return name_; }

protected:
    int id_;
    std::string name_;
};

// ============================================================
// C++03风格：手动转发每个构造函数
// ============================================================
class OldDerived : public Base {
public:
    // 必须为每个基类构造函数写一个转发版本
    OldDerived() : Base(), extra_("旧默认") {}
    OldDerived(int id) : Base(id), extra_("旧默认") {}
    OldDerived(int id, const std::string& name) : Base(id, name), extra_("旧默认") {}
    OldDerived(int id, const std::string& name, double score)
        : Base(id, name, score), extra_("旧默认") {}

    void print() const override {
        std::cout << "  OldDerived[" << id_ << "]: " << name_
                  << " (extra=" << extra_ << ")" << std::endl;
    }

private:
    std::string extra_;
};

// ============================================================
// C++11风格：继承构造函数
// ============================================================
class NewDerived : public Base {
public:
    using Base::Base;  // 一行搞定！继承Base的所有构造函数

    void print() const override {
        std::cout << "  NewDerived[" << id_ << "]: " << name_
                  << " (extra=" << extra_ << ")" << std::endl;
    }

private:
    std::string extra_ = "新默认"; // 类内初始化，因为继承的构造函数不会初始化它
};

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // C++03风格
    std::cout << "--- C++03风格（手动转发）---" << std::endl;
    OldDerived o1;
    OldDerived o2(100);
    OldDerived o3(101, "张三");
    o1.print();
    o2.print();
    o3.print();

    // C++11风格
    std::cout << "\n--- C++11风格（继承构造函数）---" << std::endl;
    NewDerived n1;            // 继承的默认构造
    NewDerived n2(200);       // 继承的Base(int)
    NewDerived n3(201, "李四"); // 继承的Base(int, string)
    NewDerived n4(202, "王五", 95.0); // 继承的Base(int, string, double)
    n1.print();
    n2.print();
    n3.print();
    n4.print();
}

// ============================================================
// 进阶用法
// ============================================================

// 派生类可以覆盖特定的构造函数
class OverrideDerived : public Base {
public:
    using Base::Base;  // 继承所有

    // 覆盖Base(int, string)版本，添加派生类特有的逻辑
    OverrideDerived(int id, const std::string& name)
        : Base(id, name), tag_("自定义") {
        std::cout << "  OverrideDerived自己的(int, string)构造函数" << std::endl;
    }

    void print() const override {
        std::cout << "  OverrideDerived[" << id_ << "]: " << name_
                  << " (tag=" << tag_ << ")" << std::endl;
    }

private:
    std::string tag_ = "默认标签";
};

// 多层继承中的继承构造函数
class GrandChild : public NewDerived {
public:
    using NewDerived::NewDerived; // 继承NewDerived的构造函数（间接继承Base的）

    void print() const override {
        std::cout << "  GrandChild[" << id_ << "]: " << name_ << std::endl;
    }
};

// 多重继承中的继承构造函数
class Base1 {
public:
    Base1(int x) : x_(x) {
        std::cout << "  Base1(int): " << x_ << std::endl;
    }
    int x_;
};

class Base2 {
public:
    Base2(double y) : y_(y) {
        std::cout << "  Base2(double): " << y_ << std::endl;
    }
    double y_;
};

class Multi : public Base1, public Base2 {
public:
    using Base1::Base1; // 继承Base1(int)
    using Base2::Base2; // 继承Base2(double)

    // 需要自己的构造函数来同时初始化两个基类
    Multi(int x, double y) : Base1(x), Base2(y) {
        std::cout << "  Multi(int, double)" << std::endl;
    }
};

void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 覆盖特定构造函数
    std::cout << "覆盖特定构造函数:" << std::endl;
    OverrideDerived od1(300);          // 使用继承的Base(int)
    OverrideDerived od2(301, "赵六");   // 使用自己覆盖的版本
    od1.print();
    od2.print();

    // 2. 多层继承
    std::cout << "\n多层继承:" << std::endl;
    GrandChild gc(400, "孙子类");
    gc.print();

    // 3. 多重继承
    std::cout << "\n多重继承:" << std::endl;
    Multi m(10, 3.14);
    std::cout << "  Multi: x=" << m.x_ << ", y=" << m.y_ << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: 继承的构造函数不会初始化派生类的成员
    std::cout << "陷阱1: 派生类新增成员不会被继承的构造函数初始化" << std::endl;
    std::cout << "  解决方案: 使用类内初始化(= 默认值)为新增成员提供默认值" << std::endl;

    // 陷阱2: 如果基类构造函数是explicit的，继承的也是explicit
    std::cout << "\n陷阱2: explicit属性也会被继承" << std::endl;
    // Base b = 42;           // 错误! Base(int)是explicit的
    // NewDerived nd = 42;    // 同样错误! 继承的也是explicit
    NewDerived nd(42);        // OK，直接初始化
    nd.print();

    // 陷阱3: 默认参数在继承中的处理
    // 如果基类构造函数有默认参数，会被拆分为多个构造函数
    // 例如 Base(int a, int b = 0) 会继承为 Derived(int a, int b) 和 Derived(int a)
    std::cout << "\n陷阱3: 带默认参数的构造函数会被拆分继承" << std::endl;

    // 陷阱4: 多重继承中构造函数冲突
    // 如果两个基类有相同签名的构造函数，using都继承会产生歧义
    // 需要在派生类中显式定义一个来解决
    std::cout << "\n陷阱4: 多重继承中可能出现构造函数签名冲突" << std::endl;

    // 最佳实践
    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 使用using Base::Base简化不需要额外初始化的派生类" << std::endl;
    std::cout << "  2. 新增成员一定要使用类内初始化" << std::endl;
    std::cout << "  3. 需要特殊逻辑的构造函数单独覆盖" << std::endl;
    std::cout << "  4. 搭配委托构造函数使用效果更佳" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: 继承构造函数" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 继承构造函数 (Inheriting Constructors)](https://en.cppreference.com/w/cpp/language/using_declaration)
