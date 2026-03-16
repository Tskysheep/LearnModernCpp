---
sidebar_position: 12
title: "移动语义 (Move Semantics)"
description: "C++11 移动语义 (Move Semantics) 教学示例"
tags: [cpp11, move_semantics]
---

# 移动语义 (Move Semantics)

C++11 标准引入的 移动语义 (Move Semantics) 特性完整教学示例。

## 完整源码

```cpp title="cpp11/11_move_semantics.cpp"
/**
 * ============================================================
 * 特性名称: 移动语义 (Move Semantics)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: 右值引用(&&)、移动构造/赋值函数和std::move共同构成
 *          移动语义，允许资源从一个对象"转移"到另一个对象，
 *          避免不必要的深拷贝。
 *
 * [为什么引入]: C++03中临时对象和将亡对象的资源无法被"偷走"，
 *   只能拷贝后再销毁原来的，造成巨大的性能浪费。移动语义
 *   让资源可以低成本地转移所有权。
 *
 * [关键要点]:
 *   - 左值(lvalue)有名字可取地址，右值(rvalue)是临时的不可取地址
 *   - 右值引用(T&&)可以绑定右值，延长其生命周期
 *   - std::move将左值无条件转换为右值引用（仅类型转换，不移动）
 *   - 移动后的对象处于"有效但未指定"的状态
 *   - 移动构造/赋值函数应标记为noexcept以获得最佳性能
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <utility>   // std::move
#include <cstring>   // memcpy, strlen
#include <algorithm> // std::swap

// ============================================================
// 自定义String类 —— 展示移动语义的核心
// ============================================================
class MyString {
public:
    // 默认构造
    MyString() : data_(nullptr), size_(0) {
        std::cout << "    [默认构造]" << std::endl;
    }

    // 从C字符串构造
    MyString(const char* str) {
        size_ = std::strlen(str);
        data_ = new char[size_ + 1];
        std::memcpy(data_, str, size_ + 1);
        std::cout << "    [构造] \"" << data_ << "\"" << std::endl;
    }

    // 拷贝构造 —— 深拷贝，需要分配内存
    MyString(const MyString& other) : size_(other.size_) {
        data_ = new char[size_ + 1];
        std::memcpy(data_, other.data_, size_ + 1);
        std::cout << "    [拷贝构造] \"" << data_ << "\" (分配了新内存!)" << std::endl;
    }

    // 移动构造 —— 偷走资源，零拷贝
    MyString(MyString&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr; // 源对象放弃资源所有权
        other.size_ = 0;
        std::cout << "    [移动构造] \"" << (data_ ? data_ : "null")
                  << "\" (零拷贝!)" << std::endl;
    }

    // 拷贝赋值
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            std::memcpy(data_, other.data_, size_ + 1);
            std::cout << "    [拷贝赋值] \"" << data_ << "\"" << std::endl;
        }
        return *this;
    }

    // 移动赋值 —— 偷走资源
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
            std::cout << "    [移动赋值] \"" << (data_ ? data_ : "null")
                      << "\" (零拷贝!)" << std::endl;
        }
        return *this;
    }

    // 析构
    ~MyString() {
        if (data_) {
            std::cout << "    [析构] \"" << data_ << "\"" << std::endl;
        }
        delete[] data_;
    }

    const char* c_str() const { return data_ ? data_ : "(空)"; }
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

private:
    char* data_;
    size_t size_;
};

// ============================================================
// 辅助函数
// ============================================================
MyString createGreeting(const std::string& name) {
    MyString result(("你好, " + name + "!").c_str());
    return result; // 返回局部对象 —— 触发移动或RVO
}

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. 左值 vs 右值
    std::cout << "\n--- 左值 vs 右值 ---" << std::endl;
    int x = 42;       // x是左值，42是右值
    int& lr = x;      // 左值引用绑定左值
    // int& lr2 = 42; // 错误! 左值引用不能绑定右值
    const int& clr = 42; // const左值引用可以绑定右值
    int&& rr = 42;    // 右值引用绑定右值
    std::cout << "左值引用: " << lr << ", const左值引用: " << clr
              << ", 右值引用: " << rr << std::endl;

    // 2. 拷贝 vs 移动
    std::cout << "\n--- 拷贝 vs 移动 ---" << std::endl;
    std::cout << "创建a:" << std::endl;
    MyString a("Hello");

    std::cout << "拷贝a到b:" << std::endl;
    MyString b(a);            // 拷贝构造

    std::cout << "移动a到c:" << std::endl;
    MyString c(std::move(a)); // 移动构造

    std::cout << "a的内容: \"" << a.c_str() << "\" (已被移走!)" << std::endl;
    std::cout << "b的内容: \"" << b.c_str() << "\"" << std::endl;
    std::cout << "c的内容: \"" << c.c_str() << "\"" << std::endl;

    // 3. std::move的本质 —— 仅仅是类型转换
    std::cout << "\n--- std::move是类型转换，不是移动操作 ---" << std::endl;
    MyString d("World");
    MyString&& rref = std::move(d); // 只是把d转换为右值引用，没有移动!
    std::cout << "d仍然有效: \"" << d.c_str() << "\"" << std::endl;
    std::cout << "rref指向同一对象: \"" << rref.c_str() << "\"" << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 函数返回值的移动（RVO/NRVO可能消除移动）
    std::cout << "--- 函数返回值 ---" << std::endl;
    MyString greeting = createGreeting("世界");
    std::cout << "问候: \"" << greeting.c_str() << "\"" << std::endl;

    // 2. 移动赋值
    std::cout << "\n--- 移动赋值 ---" << std::endl;
    MyString e("旧值");
    std::cout << "移动赋值:" << std::endl;
    e = MyString("新值");  // 右边是临时对象（右值），触发移动赋值
    std::cout << "e: \"" << e.c_str() << "\"" << std::endl;

    // 3. 容器中的移动语义
    std::cout << "\n--- 容器中的移动 ---" << std::endl;
    std::vector<MyString> vec;
    vec.reserve(3); // 预分配空间避免重新分配干扰演示

    std::cout << "push_back临时对象:" << std::endl;
    vec.push_back(MyString("甲")); // 移动（临时对象是右值）

    std::cout << "push_back左值:" << std::endl;
    MyString item("乙");
    vec.push_back(item); // 拷贝（左值不能移动）

    std::cout << "push_back + std::move:" << std::endl;
    vec.push_back(std::move(item)); // 移动（std::move转为右值）
    std::cout << "item已被移走: \"" << item.c_str() << "\"" << std::endl;

    // 4. swap利用移动语义
    std::cout << "\n--- 移动实现高效swap ---" << std::endl;
    MyString s1("AAAA");
    MyString s2("BBBB");
    std::cout << "交换前: s1=\"" << s1.c_str() << "\", s2=\"" << s2.c_str() << "\"" << std::endl;

    // std::swap内部使用移动: temp=move(a), a=move(b), b=move(temp)
    MyString temp(std::move(s1));
    s1 = std::move(s2);
    s2 = std::move(temp);
    std::cout << "交换后: s1=\"" << s1.c_str() << "\", s2=\"" << s2.c_str() << "\"" << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: 移动后使用（use after move）
    std::cout << "陷阱1: 移动后使用" << std::endl;
    std::string s = "你好世界";
    std::string t = std::move(s);
    // s现在处于"有效但未指定"的状态
    // 可以安全地: 赋新值、调用clear()、检查empty()、析构
    // 不应该: 假设它的内容是什么
    std::cout << "  移动后s.empty() = " << (s.empty() ? "是" : "否") << std::endl;
    std::cout << "  t = \"" << t << "\"" << std::endl;
    s = "重新赋值"; // OK!
    std::cout << "  重新赋值后 s = \"" << s << "\"" << std::endl;

    // 陷阱2: 不要对const对象使用std::move
    std::cout << "\n陷阱2: const对象不能真正被移动" << std::endl;
    const std::string cs = "常量字符串";
    std::string u = std::move(cs); // std::move(cs)返回const string&&
    // 但移动构造函数接受string&&（非const），所以匹配的是拷贝构造!
    // 实际执行的是拷贝，不是移动!
    std::cout << "  const对象'移动'后仍有值: \"" << cs << "\"（实际是拷贝!）" << std::endl;

    // 陷阱3: 不要在return语句中使用std::move
    // MyString foo() {
    //     MyString local("hello");
    //     return std::move(local); // 反模式! 阻止了RVO/NRVO优化!
    //     return local;            // 正确! 编译器会自动应用移动或RVO
    // }
    std::cout << "\n陷阱3: return语句不要加std::move（会阻止RVO优化）" << std::endl;

    // 陷阱4: 移动构造/赋值必须标记noexcept
    std::cout << "\n陷阱4: 移动操作应标记noexcept" << std::endl;
    std::cout << "  vector等容器只有在移动操作是noexcept时才会使用移动" << std::endl;
    std::cout << "  否则退化为拷贝以保证异常安全" << std::endl;

    // 陷阱5: 注意移动后对象状态的合法操作
    std::cout << "\n陷阱5: 移动后对象的合法操作" << std::endl;
    std::cout << "  可以: 析构、赋新值" << std::endl;
    std::cout << "  取决于类型: 可能可以clear()、empty()等" << std::endl;
    std::cout << "  不应该: 读取其值（结果不确定）" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: 移动语义" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 移动语义 (Move Semantics)](https://en.cppreference.com/w/cpp/language/move_constructor)
