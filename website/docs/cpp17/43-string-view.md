---
sidebar_position: 44
title: "std::string_view (非拥有字符串视图)"
description: "C++17 std::string_view (非拥有字符串视图) 教学示例"
tags: [cpp17, string_view]
---

# std::string_view (非拥有字符串视图)

C++17 标准引入的 std::string_view (非拥有字符串视图) 特性完整教学示例。

## 完整源码

```cpp title="cpp17/43_string_view.cpp"
/**
 * ============================================================
 * 特性名称: std::string_view (非拥有字符串视图)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: 一个轻量级的、非拥有的字符串引用。只包含一个指针和
 *          一个长度，不会拷贝或分配字符串数据。可以引用
 *          std::string、C 字符串、字符串字面量等。
 *
 * [为什么引入]: 传递 const std::string& 接收 C 字符串时会创建
 *              临时 string（堆分配）。传递 const char* 又失去了
 *              长度信息。string_view 既不拷贝又保留长度信息，
 *              是只读字符串参数的最佳选择。
 *
 * [关键要点]:
 *   - 只读视图 —— 不能修改底层字符串
 *   - 不拥有数据 —— 不管理底层字符串的生命周期
 *   - substr() 返回新的 string_view，不分配内存（O(1)）
 *   - 可以从 string、const char*、字面量隐式构造
 *   - 最大陷阱：底层字符串销毁后，string_view 变成悬垂引用
 * ============================================================
 */

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>

// ============================================================
// 基础用法
// ============================================================

// 用 string_view 做参数 —— 避免不必要的拷贝
void greet(std::string_view name) {
    std::cout << "你好, " << name << "!\n";
}

// 对比：const string& 版本
void greet_old(const std::string& name) {
    std::cout << "你好, " << name << "! (旧版)\n";
}

void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 从不同来源构造 string_view
    std::string_view sv1 = "字面量";                      // 从字面量
    std::string str = "std::string";
    std::string_view sv2 = str;                            // 从 string
    const char* cstr = "C 字符串";
    std::string_view sv3 = cstr;                           // 从 const char*
    std::string_view sv4{"Buffer++++", 6};                 // 指定长度

    std::cout << "sv1: " << sv1 << "\n";
    std::cout << "sv2: " << sv2 << "\n";
    std::cout << "sv3: " << sv3 << "\n";
    std::cout << "sv4: " << sv4 << "\n";

    // 2. 基本操作
    std::string_view sv = "Hello, World!";
    std::cout << "长度: " << sv.size() << "\n";
    std::cout << "空?: " << sv.empty() << "\n";
    std::cout << "首字符: " << sv.front() << "\n";
    std::cout << "尾字符: " << sv.back() << "\n";
    std::cout << "sv[7]: " << sv[7] << "\n";

    // 3. 传参 —— 无拷贝
    greet("字面量");                // const char* → string_view (无拷贝)
    greet(str);                     // string → string_view (无拷贝)
    greet(sv1);                     // string_view → string_view (无拷贝)

    // 对比：旧方式
    greet_old("字面量");           // const char* → 临时 string (堆分配!)
    greet_old(str);                // string → const string& (无拷贝)

    // 4. substr —— O(1)，不分配内存！
    std::string_view full = "Hello, World!";
    std::string_view sub = full.substr(7, 5);  // "World"
    std::cout << "substr: " << sub << "\n";

    // 对比：string::substr 会分配新内存
    // std::string sub_str = str.substr(7, 5);  // 分配内存
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 高效的字符串分割
std::vector<std::string_view> split(std::string_view str, char delim) {
    std::vector<std::string_view> result;
    size_t start = 0;
    while (start < str.size()) {
        size_t end = str.find(delim, start);
        if (end == std::string_view::npos) {
            result.push_back(str.substr(start));
            break;
        }
        result.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    return result;
}

// 2. 去除首尾空白
std::string_view trim(std::string_view sv) {
    while (!sv.empty() && sv.front() == ' ') sv.remove_prefix(1);
    while (!sv.empty() && sv.back() == ' ')  sv.remove_suffix(1);
    return sv;
}

// 3. 判断前缀/后缀
bool starts_with(std::string_view str, std::string_view prefix) {
    return str.size() >= prefix.size() &&
           str.substr(0, prefix.size()) == prefix;
}

bool ends_with(std::string_view str, std::string_view suffix) {
    return str.size() >= suffix.size() &&
           str.substr(str.size() - suffix.size()) == suffix;
}

// 4. string_view 做 map 的键 —— 避免字符串拷贝查找
#include <map>

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 字符串分割
    std::string data = "Alice,Bob,Charlie,David";
    auto parts = split(data, ',');
    std::cout << "分割结果: ";
    for (auto p : parts) std::cout << "[" << p << "] ";
    std::cout << "\n";

    // 去除空白
    std::string_view padded = "   hello world   ";
    std::cout << "去除空白: [" << trim(padded) << "]\n";

    // 前缀/后缀判断
    std::string_view filename = "photo.jpg";
    std::cout << "以 photo 开头: " << starts_with(filename, "photo") << "\n";
    std::cout << "以 .jpg 结尾: " << ends_with(filename, ".jpg") << "\n";
    std::cout << "以 .png 结尾: " << ends_with(filename, ".png") << "\n";

    // remove_prefix / remove_suffix
    std::string_view sv = "<<<content>>>";
    sv.remove_prefix(3);  // 移除前 3 个字符
    sv.remove_suffix(3);  // 移除后 3 个字符
    std::cout << "去除标记: " << sv << "\n";

    // 查找操作
    std::string_view text = "The quick brown fox jumps over the lazy dog";
    auto pos = text.find("fox");
    if (pos != std::string_view::npos) {
        std::cout << "找到 'fox' 在位置: " << pos << "\n";
    }

    // string_view 和 string 的转换
    std::string_view sv2 = "convert me";
    std::string s(sv2);  // 显式构造 string（会拷贝）
    std::cout << "转换为 string: " << s << "\n";
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: 悬垂引用 —— 最危险的陷阱！
    std::string_view dangling;
    {
        std::string temp = "temporary";
        dangling = temp;
        // temp 在这里被销毁
    }
    // dangling 现在指向已释放的内存！使用它是 UB！
    // std::cout << dangling;  // UB！不要这样做！
    std::cout << "陷阱1 - string_view 引用已销毁的 string 是 UB!\n";

    // 陷阱 2: 函数返回局部 string 的 string_view
    // std::string_view bad_func() {
    //     std::string local = "hello";
    //     return local;  // 危险！返回后 local 被销毁
    // }
    std::cout << "陷阱2 - 不要返回局部 string 的 string_view\n";

    // 陷阱 3: string_view 不保证以 null 结尾
    std::string_view sv = "Hello, World!";
    std::string_view sub = sv.substr(0, 5);  // "Hello"
    // printf("%s", sub.data());  // 危险！可能打印多余字符
    // 正确做法：转为 string 或使用 write/cout
    std::cout << "陷阱3 - string_view 不保证 null 结尾: "
              << std::string(sub) << "\n";

    // 陷阱 4: string_view 是只读的
    // std::string_view sv = "hello";
    // sv[0] = 'H';  // 编译错误！
    std::cout << "陷阱4 - string_view 是只读的，不能修改内容\n";

    // 陷阱 5: 从临时 string 构造 string_view
    // std::string_view bad = std::string("temp");  // 危险！临时对象即刻销毁
    // 但这是安全的：
    std::string_view safe = "literal";  // 字面量有静态生命周期
    std::cout << "陷阱5 - 字面量是安全的: " << safe << "\n";
    std::cout << "        临时 string 构造 string_view 是危险的!\n";

    // 陷阱 6: string_view 的比较
    std::string_view a = "hello";
    std::string b = "hello";
    std::cout << "陷阱6 - string_view 和 string 可以直接比较: "
              << (a == b) << "\n";

    // 性能对比
    std::cout << "\n--- 大小对比 ---\n";
    std::cout << "sizeof(string):      " << sizeof(std::string) << "\n";
    std::cout << "sizeof(string_view): " << sizeof(std::string_view) << "\n";
    std::cout << "sizeof(const char*): " << sizeof(const char*) << "\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::string_view (非拥有字符串视图) - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::string_view (非拥有字符串视图)](https://en.cppreference.com/w/cpp/string/basic_string_view)
