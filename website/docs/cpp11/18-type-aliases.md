---
sidebar_position: 19
title: "using类型别名 (Type Aliases)"
description: "C++11 using类型别名 (Type Aliases) 教学示例"
tags: [cpp11, type_aliases]
---

# using类型别名 (Type Aliases)

C++11 标准引入的 using类型别名 (Type Aliases) 特性完整教学示例。

## 完整源码

```cpp title="cpp11/18_type_aliases.cpp"
/**
 * ============================================================
 * 特性名称: using类型别名 (Type Aliases)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: using Name = Type; 语法定义类型别名，
 *          是typedef的现代替代方案，尤其支持模板别名。
 *
 * [为什么引入]: typedef语法对于复杂类型（函数指针、模板类型）
 *   可读性差，而且不能直接为模板定义别名。using语法更直观，
 *   并且支持模板别名(alias template)这一全新功能。
 *
 * [关键要点]:
 *   - using Name = Type; 等价于 typedef Type Name;
 *   - 模板别名: template<typename T> using Vec = std::vector<T>;
 *   - using语法更接近"Name = Type"的赋值直觉
 *   - 模板别名是typedef无法实现的功能
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <memory>
#include <cstdint>

// ============================================================
// 基础用法
// ============================================================

// 1. 简单类型别名 —— typedef vs using
typedef int Integer_old;        // C++03 typedef
using Integer = int;            // C++11 using

typedef std::string String_old; // C++03
using String = std::string;     // C++11

// 2. 函数指针别名 —— using更清晰
typedef void (*OldCallback)(int, const std::string&);  // typedef难读
using Callback = void (*)(int, const std::string&);     // using直观

// 3. 复杂类型别名
typedef std::map<std::string, std::vector<int>> OldScoreMap;
using ScoreMap = std::map<std::string, std::vector<int>>;

void sample_callback(int code, const std::string& msg) {
    std::cout << "  回调: code=" << code << ", msg=" << msg << std::endl;
}

void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 简单类型别名
    Integer x = 42;
    String s = "你好";
    std::cout << "Integer x = " << x << std::endl;
    std::cout << "String s = " << s << std::endl;

    // 函数指针别名
    Callback cb = sample_callback;
    cb(200, "成功");

    // 复杂类型别名
    ScoreMap scores;
    scores["语文"] = {90, 85, 92};
    scores["数学"] = {95, 88, 91};

    for (const auto& pair : scores) {
        std::cout << pair.first << ": ";
        for (auto v : pair.second) std::cout << v << " ";
        std::cout << std::endl;
    }

    // using 与 typedef 对比
    std::cout << "\ntypedef vs using 对比:" << std::endl;
    std::cout << "  typedef void (*Func)(int);  // 类型名在中间，不直观" << std::endl;
    std::cout << "  using Func = void (*)(int); // 类型名在左边，清晰" << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 模板别名 —— using独有的能力!（typedef做不到!）
template <typename T>
using Vec = std::vector<T>;

template <typename T>
using StringMap = std::map<std::string, T>;

template <typename T>
using Ptr = std::shared_ptr<T>;

template <typename T>
using UniquePtr = std::unique_ptr<T>;

// 2. 更复杂的模板别名
template <typename Key, typename Value>
using OrderedMap = std::map<Key, Value>;

template <typename T>
using Matrix = std::vector<std::vector<T>>;

// 3. 回调类型的模板别名
template <typename Ret, typename... Args>
using Function = std::function<Ret(Args...)>;

// 4. 在类中使用using定义成员类型
class Database {
public:
    using RecordId = uint64_t;
    using Record = std::map<std::string, std::string>;
    using RecordSet = std::vector<Record>;

    RecordId insert(const Record& record) {
        records_.push_back(record);
        return records_.size();
    }

    const RecordSet& all() const { return records_; }

private:
    RecordSet records_;
};

// 5. 模板别名简化type_traits
template <typename T>
using RemoveConst = typename std::remove_const<T>::type;

template <typename T>
using RemoveRef = typename std::remove_reference<T>::type;

// C++14中标准库提供了这些 _t 别名:
// template <typename T> using remove_const_t = typename remove_const<T>::type;

void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 模板别名
    Vec<int> numbers = {1, 2, 3, 4, 5};
    Vec<std::string> words = {"甲", "乙", "丙"};

    std::cout << "Vec<int>: ";
    for (auto n : numbers) std::cout << n << " ";
    std::cout << std::endl;

    std::cout << "Vec<string>: ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << std::endl;

    // 2. StringMap模板别名
    StringMap<int> ages;
    ages["张三"] = 25;
    ages["李四"] = 30;
    std::cout << "\nStringMap<int>:" << std::endl;
    for (const auto& pair : ages) {
        std::cout << "  " << pair.first << ": " << pair.second << std::endl;
    }

    // 3. Matrix模板别名
    Matrix<double> mat = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0}
    };
    std::cout << "\nMatrix<double>:" << std::endl;
    for (const auto& row : mat) {
        std::cout << "  ";
        for (auto v : row) std::cout << v << " ";
        std::cout << std::endl;
    }

    // 4. 智能指针别名
    auto p = Ptr<Database>(new Database());
    Database::Record rec = {{"name", "王五"}, {"age", "28"}};
    auto id = p->insert(rec);
    std::cout << "\n插入记录, ID = " << id << std::endl;

    // 5. 类中的类型别名
    Database db;
    Database::RecordId rid = db.insert({{"name", "赵六"}, {"city", "北京"}});
    std::cout << "Database记录ID: " << rid << std::endl;

    // 6. type_traits别名
    std::cout << "\ntype_traits别名:" << std::endl;
    std::cout << "  RemoveConst<const int> 是 int: "
              << std::is_same<RemoveConst<const int>, int>::value << std::endl;
    std::cout << "  RemoveRef<int&> 是 int: "
              << std::is_same<RemoveRef<int&>, int>::value << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================

// typedef尝试模板别名（失败）
// template <typename T>
// typedef std::vector<T> Vec_old; // 编译错误! typedef不支持模板!

// C++03的变通方案（丑陋的wrapper struct）
template <typename T>
struct VecTypedef {
    typedef std::vector<T> type;
};
// 使用: VecTypedef<int>::type v; // 繁琐!

void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: typedef不能直接做模板别名
    std::cout << "陷阱1: typedef无法做模板别名" << std::endl;
    // C++03变通方案:
    VecTypedef<int>::type old_vec = {1, 2, 3}; // 需要::type
    // C++11:
    Vec<int> new_vec = {1, 2, 3};              // 直接使用
    std::cout << "  typedef需要wrapper struct + ::type" << std::endl;
    std::cout << "  using直接支持模板别名" << std::endl;

    // 陷阱2: using和typedef在非模板场景中完全等价
    std::cout << "\n陷阱2: 非模板场景中using和typedef等价" << std::endl;
    typedef int MyInt1;
    using MyInt2 = int;
    std::cout << "  is_same<MyInt1, MyInt2>: "
              << std::is_same<MyInt1, MyInt2>::value << std::endl;

    // 陷阱3: 别名不创建新类型
    using Meters = double;
    using Seconds = double;
    Meters distance = 100.0;
    Seconds time = 10.0;
    // 这两个类型实际上都是double，可以混用（没有类型安全!）
    Meters oops = time; // 编译通过! 但语义上是错误的!
    std::cout << "\n陷阱3: 类型别名不创建新类型" << std::endl;
    std::cout << "  Meters和Seconds都是double，可以混用: " << oops << std::endl;
    std::cout << "  要真正的类型安全，需要定义新的类/结构体" << std::endl;

    // 陷阱4: 模板别名不能特化
    // template <> using Vec<bool> = std::deque<bool>; // 编译错误!
    std::cout << "\n陷阱4: 模板别名不能被特化" << std::endl;

    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 新代码优先使用using而非typedef" << std::endl;
    std::cout << "  2. 需要模板别名时只能用using" << std::endl;
    std::cout << "  3. 用有意义的别名提高代码可读性" << std::endl;
    std::cout << "  4. 别名只是可读性工具，不提供类型安全" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: using类型别名" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: using类型别名 (Type Aliases)](https://en.cppreference.com/w/cpp/language/type_alias)
