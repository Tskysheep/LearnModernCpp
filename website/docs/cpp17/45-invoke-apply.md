---
sidebar_position: 46
slug: 45-invoke-apply
title: "std::invoke 与 std::apply (统一调用与元组展开)"
description: "C++17 std::invoke 与 std::apply (统一调用与元组展开) 教学示例"
tags: [cpp17, invoke_apply]
---

# std::invoke 与 std::apply (统一调用与元组展开)

C++17 标准引入的 std::invoke 与 std::apply (统一调用与元组展开) 特性完整教学示例。

## 完整源码

```cpp title="cpp17/45_invoke_apply.cpp"
/**
 * ============================================================
 * 特性名称: std::invoke 与 std::apply (统一调用与元组展开)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: std::invoke 提供统一的方式调用任何可调用对象（函数、
 *          成员函数、函数对象、lambda）。std::apply 将 tuple
 *          的元素展开为函数参数并调用。
 *
 * [为什么引入]: C++ 中调用函数、成员函数、函数对象的语法各不相同。
 *              std::invoke 统一了调用语法，让泛型代码不用区分
 *              可调用对象的类型。std::apply 解决了将 tuple 展开
 *              为参数列表的需求。
 *
 * [关键要点]:
 *   - invoke(f, args...) —— f 可以是函数/成员函数/函数对象
 *   - 成员函数: invoke(&Class::method, object, args...)
 *   - 成员变量: invoke(&Class::member, object)
 *   - apply(f, tuple) —— 将 tuple 展开后调用 f
 *   - apply 配合 make_from_tuple 可以用 tuple 构造对象
 * ============================================================
 */

#include <iostream>
#include <functional>   // std::invoke
#include <tuple>        // std::apply, std::make_from_tuple
#include <string>
#include <vector>
#include <utility>

// ============================================================
// 测试用的类和函数
// ============================================================

// 普通函数
int add(int a, int b) {
    return a + b;
}

// 类
struct Calculator {
    std::string name;
    int base;

    int multiply(int a, int b) const {
        return a * b + base;
    }

    static int static_add(int a, int b) {
        return a + b;
    }

    int operator()(int x) const {
        return x * x + base;
    }
};

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // ---- std::invoke ----

    // 1. 调用普通函数
    auto r1 = std::invoke(add, 3, 4);
    std::cout << "普通函数: add(3,4) = " << r1 << "\n";

    // 2. 调用 lambda
    auto lambda = [](int x, int y) { return x - y; };
    auto r2 = std::invoke(lambda, 10, 3);
    std::cout << "lambda: (10,3) = " << r2 << "\n";

    // 3. 调用成员函数
    Calculator calc{"计算器", 0};
    auto r3 = std::invoke(&Calculator::multiply, calc, 5, 6);
    std::cout << "成员函数: multiply(5,6) = " << r3 << "\n";

    // 4. 调用成员函数（通过指针）
    Calculator* pcalc = &calc;
    auto r4 = std::invoke(&Calculator::multiply, pcalc, 3, 4);
    std::cout << "成员函数(指针): multiply(3,4) = " << r4 << "\n";

    // 5. 访问成员变量
    auto name = std::invoke(&Calculator::name, calc);
    std::cout << "成员变量: name = " << name << "\n";

    // 6. 调用函数对象（operator()）
    Calculator square_calc{"平方器", 10};
    auto r5 = std::invoke(square_calc, 5);
    std::cout << "函数对象: (5) = " << r5 << " (5*5+10)\n";

    // 7. 调用静态成员函数
    auto r6 = std::invoke(&Calculator::static_add, 7, 8);
    std::cout << "静态成员函数: static_add(7,8) = " << r6 << "\n";

    // ---- std::apply ----

    std::cout << "\n";

    // 8. 将 tuple 展开为函数参数
    auto args = std::make_tuple(10, 20);
    auto r7 = std::apply(add, args);
    std::cout << "apply(add, {10,20}) = " << r7 << "\n";

    // 9. 用 tuple 调用 lambda
    auto concat = [](const std::string& a, const std::string& b) {
        return a + " " + b;
    };
    auto str_args = std::make_tuple(std::string("Hello"), std::string("World"));
    auto r8 = std::apply(concat, str_args);
    std::cout << "apply(concat, {\"Hello\",\"World\"}) = " << r8 << "\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 泛型调用包装器 —— invoke 让泛型代码简洁
template<typename Callable, typename... Args>
auto timed_call(Callable&& func, Args&&... args) {
    std::cout << "  [调用开始]\n";
    auto result = std::invoke(std::forward<Callable>(func),
                              std::forward<Args>(args)...);
    std::cout << "  [调用结束]\n";
    return result;
}

// 2. 用 apply 实现 tuple 的 for_each
template<typename Tuple, typename Func>
void tuple_for_each(const Tuple& t, Func&& func) {
    std::apply([&func](const auto&... args) {
        (func(args), ...);  // 折叠表达式
    }, t);
}

// 3. 用 apply 将 tuple 转为字符串
template<typename... Args>
std::string tuple_to_string(const std::tuple<Args...>& t) {
    std::string result = "(";
    bool first = true;
    std::apply([&result, &first](const auto&... args) {
        auto append = [&](const auto& arg) {
            if (!first) result += ", ";
            first = false;
            if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::string>) {
                result += "\"" + arg + "\"";
            } else {
                result += std::to_string(arg);
            }
        };
        (append(args), ...);
    }, t);
    result += ")";
    return result;
}

// 4. make_from_tuple —— 用 tuple 构造对象
struct Point3D {
    double x, y, z;
    Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
    void print() const {
        std::cout << "Point3D(" << x << ", " << y << ", " << z << ")\n";
    }
};

// 5. 延迟调用 —— 存储函数和参数，稍后调用
template<typename Func, typename... Args>
auto make_deferred(Func&& func, Args&&... args) {
    // 将函数和参数打包到 tuple 中
    auto package = std::make_tuple(std::forward<Func>(func),
                                    std::forward<Args>(args)...);
    return [package = std::move(package)]() mutable {
        return std::apply([](auto&& func, auto&&... args) {
            return std::invoke(std::forward<decltype(func)>(func),
                              std::forward<decltype(args)>(args)...);
        }, std::move(package));
    };
}

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 泛型调用包装器
    std::cout << "计时调用普通函数:\n";
    auto r1 = timed_call(add, 3, 4);
    std::cout << "  结果: " << r1 << "\n";

    std::cout << "计时调用 lambda:\n";
    auto r2 = timed_call([](int x) { return x * x; }, 7);
    std::cout << "  结果: " << r2 << "\n";

    // tuple_for_each
    std::cout << "\ntuple 遍历: ";
    auto t = std::make_tuple(1, 2.5, std::string("hello"));
    tuple_for_each(t, [](const auto& v) {
        std::cout << v << " ";
    });
    std::cout << "\n";

    // tuple_to_string
    auto t2 = std::make_tuple(42, 3.14, std::string("world"));
    std::cout << "tuple 转字符串: " << tuple_to_string(t2) << "\n";

    // make_from_tuple
    auto point_args = std::make_tuple(1.0, 2.0, 3.0);
    auto point = std::make_from_tuple<Point3D>(point_args);
    std::cout << "make_from_tuple: ";
    point.print();

    // 延迟调用
    auto deferred = make_deferred(add, 100, 200);
    std::cout << "延迟调用结果: " << deferred() << "\n";
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: invoke 的成员函数调用语法
    //   第一个参数必须是对象（值/引用/指针/智能指针）
    Calculator calc{"测试", 0};
    // std::invoke(&Calculator::multiply, 5, 6);  // 错误！缺少对象
    auto r = std::invoke(&Calculator::multiply, calc, 5, 6);
    std::cout << "陷阱1 - 成员函数第一个参数必须是对象: " << r << "\n";

    // 陷阱 2: invoke 与 reference_wrapper
    Calculator calc2{"引用测试", 100};
    auto ref = std::ref(calc2);
    auto r2 = std::invoke(&Calculator::multiply, ref, 3, 4);
    std::cout << "陷阱2 - reference_wrapper 也可以: " << r2 << "\n";

    // 陷阱 3: apply 的 tuple 参数数量必须匹配函数参数数量
    // auto bad_args = std::make_tuple(1, 2, 3);
    // std::apply(add, bad_args);  // 编译错误！add 只要 2 个参数
    std::cout << "陷阱3 - apply 的 tuple 元素数必须匹配函数参数数\n";

    // 陷阱 4: invoke 返回类型
    //   如果调用 void 函数，invoke 也返回 void
    auto void_func = [](int x) { (void)x; };
    std::invoke(void_func, 42);  // 返回 void
    std::cout << "陷阱4 - invoke 的返回类型跟随被调用函数\n";

    // 陷阱 5: apply 不能直接用于成员函数
    //   需要把对象放在 tuple 的第一个元素中
    auto member_args = std::make_tuple(calc, 3, 4);
    auto r3 = std::apply(&Calculator::multiply, member_args);
    std::cout << "陷阱5 - apply 成员函数：对象放 tuple 首位: " << r3 << "\n";

    // 陷阱 6: std::invoke 在编译期的检查
    //   如果可调用对象和参数不匹配，会得到清晰的编译错误
    std::cout << "陷阱6 - invoke 提供清晰的编译期类型检查\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::invoke 与 std::apply - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::invoke 与 std::apply (统一调用与元组展开)](https://en.cppreference.com/w/cpp/utility/functional/invoke)
