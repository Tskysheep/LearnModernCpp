---
sidebar_position: 13
slug: 12-perfect-forwarding
title: "完美转发 (Perfect Forwarding)"
description: "C++11 完美转发 (Perfect Forwarding) 教学示例"
tags: [cpp11, perfect_forwarding]
---

# 完美转发 (Perfect Forwarding)

C++11 标准引入的 完美转发 (Perfect Forwarding) 特性完整教学示例。

## 完整源码

```cpp title="cpp11/12_perfect_forwarding.cpp"
/**
 * ============================================================
 * 特性名称: 完美转发 (Perfect Forwarding)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: 通过万能引用(T&&)和std::forward<T>()，将参数的
 *          值类别（左值/右值）原封不动地传递给下一个函数。
 *
 * [为什么引入]: C++03中编写通用包装函数（如工厂函数、emplace等）
 *   时，无法同时高效处理左值和右值参数。要么丢失右值性导致
 *   多余拷贝，要么需要2^N个重载。完美转发一举解决了这个问题。
 *
 * [关键要点]:
 *   - 万能引用(Universal Reference): 模板参数 T&& 中T是推导的
 *   - 引用折叠规则: T& + & = T&, T& + && = T&, T&& + & = T&, T&& + && = T&&
 *   - std::forward<T>(arg): 有条件地转换——左值保持左值，右值保持右值
 *   - std::move是无条件转换，std::forward是有条件转换
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <type_traits>

// ============================================================
// 辅助：展示左值/右值信息
// ============================================================
class Heavy {
public:
    Heavy() : name_("默认") {
        std::cout << "    Heavy() 默认构造" << std::endl;
    }
    Heavy(const std::string& n) : name_(n) {
        std::cout << "    Heavy(const&) 拷贝构造: " << name_ << std::endl;
    }
    Heavy(std::string&& n) : name_(std::move(n)) {
        std::cout << "    Heavy(&&) 移动构造: " << name_ << std::endl;
    }
    Heavy(const Heavy& other) : name_(other.name_) {
        std::cout << "    Heavy 拷贝: " << name_ << std::endl;
    }
    Heavy(Heavy&& other) noexcept : name_(std::move(other.name_)) {
        std::cout << "    Heavy 移动: " << name_ << std::endl;
    }
    const std::string& name() const { return name_; }
private:
    std::string name_;
};

// 目标函数：区分左值和右值
void target(const Heavy& h) {
    std::cout << "  -> target(const Heavy&) 左值版本: " << h.name() << std::endl;
}

void target(Heavy&& h) {
    std::cout << "  -> target(Heavy&&) 右值版本: " << h.name() << std::endl;
}

// ============================================================
// 基础用法
// ============================================================

// 不完美转发的问题
void bad_wrapper(const Heavy& h) {
    std::cout << "  bad_wrapper(const&) -> ";
    target(h); // 总是调用左值版本!
}

void bad_wrapper(Heavy&& h) {
    std::cout << "  bad_wrapper(&&) -> ";
    target(h); // 还是调用左值版本! 因为h有名字，是左值!
}

// 完美转发
template <typename T>
void good_wrapper(T&& arg) {  // T&& 在这里是万能引用
    std::cout << "  good_wrapper -> ";
    target(std::forward<T>(arg)); // 保持值类别
}

void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    Heavy obj("对象A");

    // 1. 不完美转发的问题
    std::cout << "\n--- 不完美转发 ---" << std::endl;
    std::cout << "传左值:" << std::endl;
    bad_wrapper(obj);                // 左值 -> const& -> 调用左值版本 OK
    std::cout << "传右值:" << std::endl;
    bad_wrapper(Heavy("临时对象"));   // 右值 -> && -> 但仍调用左值版本! 错!

    // 2. 完美转发
    std::cout << "\n--- 完美转发 ---" << std::endl;
    std::cout << "传左值:" << std::endl;
    good_wrapper(obj);               // T推导为Heavy&, forward保持左值
    std::cout << "传右值:" << std::endl;
    good_wrapper(Heavy("临时对象"));  // T推导为Heavy, forward转为右值

    // 3. 引用折叠规则说明
    std::cout << "\n--- 引用折叠规则 ---" << std::endl;
    std::cout << "传左值时: T=Heavy&, T&&=Heavy& && = Heavy& (左值引用)" << std::endl;
    std::cout << "传右值时: T=Heavy,  T&&=Heavy&&            (右值引用)" << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 工厂函数模式 —— 完美转发的经典应用
template <typename T, typename... Args>
std::unique_ptr<T> make_unique_impl(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class Product {
public:
    Product(const std::string& name, int quantity, double price)
        : name_(name), qty_(quantity), price_(price) {
        std::cout << "  创建产品: " << name_ << " x" << qty_
                  << " @" << price_ << std::endl;
    }
    void show() const {
        std::cout << "  产品: " << name_ << ", 数量=" << qty_
                  << ", 单价=" << price_ << std::endl;
    }
private:
    std::string name_;
    int qty_;
    double price_;
};

// 2. 通用的日志包装器
template <typename Func, typename... Args>
auto log_call(const std::string& tag, Func&& func, Args&&... args)
    -> decltype(func(std::forward<Args>(args)...)) {
    std::cout << "[" << tag << "] 调用开始" << std::endl;
    auto result = func(std::forward<Args>(args)...);
    std::cout << "[" << tag << "] 调用结束, 结果=" << result << std::endl;
    return result;
}

int add(int a, int b) { return a + b; }

// 3. emplace_back风格的接口
template <typename T>
class SimpleVector {
public:
    template <typename... Args>
    void emplace_back(Args&&... args) {
        // 直接在存储位置构造对象，避免额外的拷贝/移动
        data_.emplace_back(std::forward<Args>(args)...);
        std::cout << "  emplace_back完成, 大小=" << data_.size() << std::endl;
    }

    void show() const {
        for (const auto& item : data_) {
            std::cout << "  -> " << item.name() << std::endl;
        }
    }

private:
    std::vector<T> data_;
};

void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 工厂函数
    std::cout << "--- 工厂函数 ---" << std::endl;
    auto product = make_unique_impl<Product>("笔记本电脑", 5, 6999.0);
    product->show();

    // 2. 日志包装器
    std::cout << "\n--- 日志包装器 ---" << std::endl;
    log_call("加法", add, 3, 5);

    // 3. emplace风格
    std::cout << "\n--- emplace风格 ---" << std::endl;
    SimpleVector<Heavy> sv;
    std::string name = "对象X";
    std::cout << "用左值emplace:" << std::endl;
    sv.emplace_back(name);      // 完美转发左值 -> 拷贝构造
    std::cout << "用右值emplace:" << std::endl;
    sv.emplace_back(std::string("对象Y")); // 完美转发右值 -> 移动构造
    sv.show();
}

// ============================================================
// 易错点与陷阱
// ============================================================

// 陷阱1: 不是所有T&&都是万能引用
class Specific {
public:
    // 这不是万能引用! 因为T不是在此处推导的
    // 它就是一个普通的右值引用
    void process(std::string&& s) {
        std::cout << "  Specific::process(string&&): " << s << std::endl;
    }
};

// 陷阱2: auto&&是万能引用
void demo_auto_universal_ref() {
    Heavy obj("auto测试");
    auto&& ref1 = obj;          // ref1是Heavy& (左值绑定)
    auto&& ref2 = Heavy("临时"); // ref2是Heavy&& (右值绑定)
    std::cout << "  auto&&绑定左值: " << ref1.name() << std::endl;
    std::cout << "  auto&&绑定右值: " << ref2.name() << std::endl;
}

// 陷阱3: forward只用一次
template <typename T>
void multiple_forward_bad(T&& arg) {
    // target(std::forward<T>(arg)); // 第一次forward
    // target(std::forward<T>(arg)); // 第二次forward: 如果arg是右值，第一次可能已移走!
    // 正确做法: 只forward一次，或者在最后一次使用时才forward
    target(arg);                      // 第一次: 当作左值使用
    target(std::forward<T>(arg));     // 最后一次: forward
}

void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: 区分万能引用和右值引用
    std::cout << "陷阱1: 万能引用 vs 右值引用" << std::endl;
    std::cout << "  template<typename T> void f(T&& x) -> 万能引用(T是推导的)" << std::endl;
    std::cout << "  void f(string&& x)                  -> 右值引用(类型确定)" << std::endl;
    std::cout << "  auto&& x = expr                     -> 万能引用" << std::endl;

    // 陷阱2: auto&&
    std::cout << "\n陷阱2: auto&&也是万能引用" << std::endl;
    demo_auto_universal_ref();

    // 陷阱3: 不要多次forward同一参数
    std::cout << "\n陷阱3: 不要多次forward同一个参数" << std::endl;
    std::cout << "  forward后参数可能已被移走，再次使用是未定义行为" << std::endl;
    Heavy h("测试");
    multiple_forward_bad(std::move(h));

    // 陷阱4: std::move vs std::forward
    std::cout << "\n陷阱4: std::move vs std::forward" << std::endl;
    std::cout << "  std::move: 无条件转为右值引用" << std::endl;
    std::cout << "  std::forward: 有条件转发（保持原始值类别）" << std::endl;
    std::cout << "  用在确定要移动的地方: std::move" << std::endl;
    std::cout << "  用在模板转发的地方: std::forward" << std::endl;

    // 最佳实践
    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 万能引用参数只配合std::forward使用" << std::endl;
    std::cout << "  2. 确定要移动时使用std::move" << std::endl;
    std::cout << "  3. 每个参数最多forward/move一次" << std::endl;
    std::cout << "  4. 完美转发是编写通用库代码的核心技术" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: 完美转发" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 完美转发 (Perfect Forwarding)](https://en.cppreference.com/w/cpp/utility/forward)
