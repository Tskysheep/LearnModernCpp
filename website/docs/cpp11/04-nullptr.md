---
sidebar_position: 5
title: "nullptr关键字"
description: "C++11 nullptr关键字 教学示例"
tags: [cpp11, nullptr]
---

# nullptr关键字

C++11 标准引入的 nullptr关键字 特性完整教学示例。

## 完整源码

```cpp title="cpp11/04_nullptr.cpp"
/**
 * ============================================================
 * 特性名称: nullptr关键字
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: nullptr是C++11引入的空指针字面量，类型为std::nullptr_t，
 *          专门用于表示空指针。
 *
 * [为什么引入]: C++03中使用NULL（被定义为0或(void*)0）或字面量0表示
 *   空指针，但它们本质上是整数，在函数重载决议时会产生歧义。nullptr
 *   解决了这一根本性缺陷。
 *
 * [关键要点]:
 *   - nullptr的类型是std::nullptr_t，可隐式转换为任何指针类型
 *   - nullptr不能隐式转换为整数类型
 *   - 在函数重载中，nullptr明确匹配指针参数
 *   - 模板中传递nullptr能正确推导为指针类型相关的参数
 * ============================================================
 */

#include <iostream>
#include <cstddef> // std::nullptr_t
#include <string>
#include <memory>

// ============================================================
// 辅助：用于演示重载决议的函数
// ============================================================
void process(int value) {
    std::cout << "  调用了 process(int): " << value << std::endl;
}

void process(int* ptr) {
    std::cout << "  调用了 process(int*): "
              << (ptr ? "非空指针" : "空指针") << std::endl;
}

// ============================================================
// 辅助：模板函数，用于演示nullptr在模板中的行为
// ============================================================
template <typename T>
void check_null(T ptr) {
    if (ptr == nullptr) {
        std::cout << "  指针为空" << std::endl;
    } else {
        std::cout << "  指针非空" << std::endl;
    }
}

template <typename T>
void show_type(T) {
    std::cout << "  模板参数类型: " << typeid(T).name() << std::endl;
}

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. nullptr初始化指针
    int* p1 = nullptr;
    double* p2 = nullptr;
    std::string* p3 = nullptr;

    std::cout << "p1是否为空: " << (p1 == nullptr ? "是" : "否") << std::endl;
    std::cout << "p2是否为空: " << (p2 == nullptr ? "是" : "否") << std::endl;

    // 2. 与NULL/0的对比
    // NULL在C++中通常被定义为0（整数零）
    // nullptr则是一个明确的空指针字面量
    int* p4 = nullptr;   // 推荐: 意图明确
    int* p5 = NULL;      // C++03风格: 本质是0
    int* p6 = 0;         // C风格: 容易与整数0混淆

    std::cout << "三种方式都是空指针: "
              << (p4 == nullptr && p5 == nullptr && p6 == nullptr ? "是" : "否")
              << std::endl;

    // 3. nullptr在条件判断中
    int value = 42;
    int* ptr = &value;
    if (ptr) {  // 非空指针在布尔上下文中为true
        std::cout << "ptr指向: " << *ptr << std::endl;
    }
    ptr = nullptr;
    if (!ptr) { // nullptr在布尔上下文中为false
        std::cout << "ptr现在为空" << std::endl;
    }
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 重载决议 —— nullptr的核心优势
    std::cout << "使用nullptr调用重载函数:" << std::endl;

    // process(NULL);      // 歧义! NULL是0，可能匹配int或int*
    process(nullptr);      // 明确匹配 process(int*)
    process(42);           // 明确匹配 process(int)

    // 2. 模板中使用nullptr
    std::cout << "\n模板中使用nullptr:" << std::endl;
    int x = 10;
    check_null(&x);       // 非空
    check_null(nullptr);  // 空

    // 查看nullptr的模板参数推导
    show_type(nullptr);   // T推导为std::nullptr_t
    show_type((int*)nullptr); // T推导为int*

    // 3. nullptr_t类型
    std::cout << "\nnullptr_t类型的使用:" << std::endl;
    std::nullptr_t null_val = nullptr; // 可以声明nullptr_t变量
    int* p = null_val;   // nullptr_t可隐式转换为任何指针类型
    std::cout << "nullptr_t变量赋给int*: " << (p == nullptr ? "成功" : "失败") << std::endl;

    // 4. 与智能指针配合
    std::shared_ptr<int> sp1 = nullptr;  // 空的shared_ptr
    std::unique_ptr<int> up1 = nullptr;  // 空的unique_ptr
    std::cout << "空shared_ptr: " << (sp1 == nullptr ? "是" : "否") << std::endl;

    sp1 = std::make_shared<int>(100);
    if (sp1 != nullptr) {
        std::cout << "shared_ptr指向: " << *sp1 << std::endl;
    }
    sp1 = nullptr; // 重置为空（释放资源）
    std::cout << "重置后: " << (sp1 == nullptr ? "已释放" : "仍有效") << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: NULL在重载中可能匹配int而非指针
    std::cout << "使用0调用:" << std::endl;
    process(0);           // 匹配 process(int), 不是指针版本!
    std::cout << "使用nullptr调用:" << std::endl;
    process(nullptr);     // 正确匹配 process(int*)

    // 陷阱2: nullptr不能用于算术运算
    // int n = nullptr;         // 编译错误! nullptr不能转换为int
    // int m = nullptr + 1;     // 编译错误!
    std::cout << "\nnullptr不能参与算术运算（编译期保证安全）" << std::endl;

    // 陷阱3: nullptr与布尔值
    // bool b = nullptr; // 某些编译器允许但会有警告
    // 推荐使用 false 而非 nullptr 来表示布尔假值
    std::cout << "不要用nullptr代替false，虽然在布尔上下文中它等价于false" << std::endl;

    // 陷阱4: 仍需注意空指针解引用
    int* dangerous = nullptr;
    // *dangerous = 42; // 运行时崩溃! nullptr只防止类型歧义，不防止解引用
    std::cout << "nullptr不能防止空指针解引用，仍需检查!" << std::endl;

    // 最佳实践总结
    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 始终用nullptr替代NULL和0" << std::endl;
    std::cout << "  2. 使用前检查指针是否为nullptr" << std::endl;
    std::cout << "  3. 优先使用智能指针而非裸指针" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: nullptr关键字" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: nullptr关键字](https://en.cppreference.com/w/cpp/language/nullptr)
