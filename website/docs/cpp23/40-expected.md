---
sidebar_position: 41
slug: 40-expected
title: "std::expected（期望值类型）"
description: "C++23 std::expected（期望值类型） 教学示例"
tags: [cpp23, expected]
---

# std::expected（期望值类型）

C++23 标准引入的 std::expected（期望值类型） 特性完整教学示例。

## 完整源码

```cpp title="cpp23/40_expected.cpp"
/**
 * ============================================================
 * 特性名称: std::expected（期望值类型）
 * 所属标准: C++23
 * ============================================================
 *
 * [是什么]: std::expected<T, E> 是一个值类型，要么包含一个期望的
 *          成功值（T），要么包含一个错误值（E）。它是函数式错误处理
 *          的核心工具，作为异常和错误码之外的第三种错误处理方式。
 *
 * [为什么引入]: 异常有性能开销且不适合所有场景（如嵌入式），错误码
 *              容易被忽略。std::expected 将错误信息编码到返回类型中，
 *              强制调用者处理错误，同时支持 monadic 操作实现链式调用。
 *
 * [关键要点]:
 *   - expected<T, E>: 要么有值（T），要么有错误（E）
 *   - 支持 value()、error()、has_value() 等访问方法
 *   - 支持 monadic 操作：and_then()、transform()、or_else()、
 *     transform_error()
 *   - std::unexpected<E> 用于构造错误值
 *   - 与 std::optional 类似但多了错误信息
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdint>

// ============================================================
// 特性检测
// ============================================================
#if __has_include(<expected>)
#include <expected>
#ifdef __cpp_lib_expected
#define HAS_EXPECTED 1
#else
#define HAS_EXPECTED 0
#endif
#else
#define HAS_EXPECTED 0
#endif

#if HAS_EXPECTED

// ============================================================
// 基础用法
// ============================================================
namespace basic_usage {

// 定义错误类型
enum class MathError {
    DivisionByZero,
    NegativeSquareRoot,
    Overflow
};

// 错误类型转字符串
std::string to_string(MathError err) {
    switch (err) {
        case MathError::DivisionByZero:      return "除零错误";
        case MathError::NegativeSquareRoot:   return "负数开方";
        case MathError::Overflow:             return "溢出";
        default: return "未知错误";
    }
}

// 返回 expected 的函数：安全除法
std::expected<double, MathError> safe_divide(double a, double b) {
    if (b == 0.0) {
        return std::unexpected(MathError::DivisionByZero);
    }
    return a / b;
}

// 返回 expected 的函数：安全开方
std::expected<double, MathError> safe_sqrt(double x) {
    if (x < 0.0) {
        return std::unexpected(MathError::NegativeSquareRoot);
    }
    return std::sqrt(x);
}

void demonstrate() {
    std::cout << "[基础用法]" << std::endl;

    // 成功情况
    auto result1 = safe_divide(10.0, 3.0);
    if (result1.has_value()) {
        std::cout << "  10.0 / 3.0 = " << result1.value() << std::endl;
    }

    // 也可以用 * 运算符访问值
    auto result2 = safe_sqrt(16.0);
    if (result2) {  // 隐式转换为 bool
        std::cout << "  sqrt(16.0) = " << *result2 << std::endl;
    }

    // 错误情况
    auto result3 = safe_divide(1.0, 0.0);
    if (!result3) {
        std::cout << "  1.0 / 0.0 => 错误: " << to_string(result3.error()) << std::endl;
    }

    auto result4 = safe_sqrt(-4.0);
    if (!result4.has_value()) {
        std::cout << "  sqrt(-4.0) => 错误: " << to_string(result4.error()) << std::endl;
    }

    // value_or：提供默认值
    double val = safe_divide(1.0, 0.0).value_or(0.0);
    std::cout << "  1.0 / 0.0 (默认0.0) = " << val << std::endl;

    // 使用 std::unexpected 显式构造错误
    std::expected<int, std::string> err_result = std::unexpected("自定义错误信息");
    std::cout << "  自定义错误: " << err_result.error() << std::endl;

    std::cout << std::endl;
}

} // namespace basic_usage

// ============================================================
// 进阶用法
// ============================================================
namespace advanced_usage {

// 错误类型用字符串
using Result = std::expected<double, std::string>;

Result parse_number(const std::string& s) {
    try {
        size_t pos;
        double val = std::stod(s, &pos);
        if (pos != s.size()) {
            return std::unexpected("无法完全解析: '" + s + "'");
        }
        return val;
    } catch (...) {
        return std::unexpected("解析失败: '" + s + "'");
    }
}

Result validate_positive(double val) {
    if (val <= 0) {
        return std::unexpected("值必须为正: " + std::to_string(val));
    }
    return val;
}

Result compute_log(double val) {
    if (val <= 0) {
        return std::unexpected("对数要求正数");
    }
    return std::log(val);
}

void demonstrate() {
    std::cout << "[进阶用法]" << std::endl;

    // --- Monadic 操作 ---
    std::cout << "  -- Monadic 操作链 --" << std::endl;

    // and_then: 链式调用返回 expected 的函数
    auto result1 = parse_number("42.5")
        .and_then(validate_positive)
        .and_then(compute_log);

    if (result1) {
        std::cout << "  parse -> validate -> log(\"42.5\") = "
                  << *result1 << std::endl;
    }

    // 链中任何一步失败都会短路
    auto result2 = parse_number("-5")
        .and_then(validate_positive)
        .and_then(compute_log);

    if (!result2) {
        std::cout << "  parse -> validate -> log(\"-5\") => 错误: "
                  << result2.error() << std::endl;
    }

    auto result3 = parse_number("abc")
        .and_then(validate_positive)
        .and_then(compute_log);

    if (!result3) {
        std::cout << "  parse -> validate -> log(\"abc\") => 错误: "
                  << result3.error() << std::endl;
    }

    // transform: 对成功值应用普通函数（不返回 expected）
    std::cout << "  -- transform --" << std::endl;
    auto result4 = parse_number("3.14")
        .transform([](double v) { return v * 2; })
        .transform([](double v) { return static_cast<int>(v); });

    if (result4) {
        std::cout << "  parse(\"3.14\") * 2 -> int = " << *result4 << std::endl;
    }

    // or_else: 处理错误，可以恢复或转换错误
    std::cout << "  -- or_else（错误恢复） --" << std::endl;
    auto result5 = parse_number("invalid")
        .or_else([](const std::string& err) -> Result {
            std::cout << "  捕获错误: " << err << "，使用默认值 0.0" << std::endl;
            return 0.0;  // 恢复为成功值
        });

    std::cout << "  恢复后的值: " << *result5 << std::endl;

    // transform_error: 转换错误类型
    std::cout << "  -- transform_error --" << std::endl;
    auto result6 = parse_number("bad")
        .transform_error([](const std::string& err) {
            return "处理过的错误: [" + err + "]";
        });

    if (!result6) {
        std::cout << "  " << result6.error() << std::endl;
    }

    // --- 与 optional 对比 ---
    std::cout << "  -- 与 std::optional 对比 --" << std::endl;
    std::cout << "  optional<T>:      有值 或 无值（不知道原因）" << std::endl;
    std::cout << "  expected<T, E>:   有值 或 有错误（知道原因）" << std::endl;
    std::cout << "  optional 适合：查找操作（找到/没找到）" << std::endl;
    std::cout << "  expected 适合：计算操作（成功/失败原因）" << std::endl;

    std::cout << std::endl;
}

} // namespace advanced_usage

// ============================================================
// 易错点与陷阱
// ============================================================
namespace pitfalls {

void demonstrate() {
    std::cout << "[易错点与陷阱]" << std::endl;

    // 陷阱1：访问不存在的值
    std::cout << "  陷阱1: 访问不存在的值会导致未定义行为" << std::endl;
    std::cout << "    - 错误状态时调用 value() 会抛 bad_expected_access" << std::endl;
    std::cout << "    - 成功状态时调用 error() 是未定义行为" << std::endl;
    std::cout << "    - 建议: 先检查 has_value()，或用 value_or()" << std::endl;

    std::expected<int, std::string> err = std::unexpected("oops");
    try {
        [[maybe_unused]] int val = err.value();  // 抛异常
    } catch (const std::bad_expected_access<std::string>& e) {
        std::cout << "    捕获异常: bad_expected_access, error = "
                  << e.error() << std::endl;
    }

    // 陷阱2：expected<void, E> 的特殊用法
    std::cout << "  陷阱2: expected<void, E> 用于不需要返回值的场景" << std::endl;
    auto do_something = []() -> std::expected<void, std::string> {
        // 模拟操作成功
        return {};  // 成功（void）
    };
    auto result = do_something();
    std::cout << "    expected<void, E>: 成功 = " << result.has_value() << std::endl;

    // 陷阱3：unexpected 是独立的类型
    std::cout << "  陷阱3: std::unexpected 是构造错误值的辅助类" << std::endl;
    std::cout << "    - 必须用 std::unexpected(error_value) 包装错误" << std::endl;
    std::cout << "    - 不能直接 return error_value（类型不匹配）" << std::endl;

    // 陷阱4：and_then 与 transform 的区别
    std::cout << "  陷阱4: and_then vs transform" << std::endl;
    std::cout << "    - and_then(f): f 返回 expected<U, E>" << std::endl;
    std::cout << "    - transform(f): f 返回 U（自动包装为 expected）" << std::endl;
    std::cout << "    - 类似 flatMap vs map 的关系" << std::endl;

    // 陷阱5：错误类型必须可复制或可移动
    std::cout << "  陷阱5: T 和 E 都需要满足一定的类型要求" << std::endl;
    std::cout << "    - E 不能是 void" << std::endl;
    std::cout << "    - E 不能是数组或引用类型" << std::endl;
    std::cout << "    - T 可以是 void" << std::endl;

    std::cout << std::endl;
}

} // namespace pitfalls

#endif // HAS_EXPECTED

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::expected（期望值类型） - C++23 =====\n" << std::endl;

#if HAS_EXPECTED
    basic_usage::demonstrate();
    advanced_usage::demonstrate();
    pitfalls::demonstrate();
#else
    std::cout << "[提示] 当前编译器不支持 std::expected 特性。" << std::endl;
    std::cout << "请使用支持 C++23 的编译器（如 GCC 14+ 或 Clang 18+）。" << std::endl;
    std::cout << std::endl;
    std::cout << "std::expected 语法示例（供参考）：" << std::endl;
    std::cout << R"(
    #include <expected>

    enum class Error { NotFound, Invalid };

    // 返回 expected：成功返回值，失败返回错误
    std::expected<int, Error> find_value(int key) {
        if (key < 0)
            return std::unexpected(Error::Invalid);
        return key * 10;
    }

    // 使用 monadic 操作链式调用
    auto result = find_value(5)
        .and_then([](int v) -> std::expected<int, Error> {
            return v + 1;
        })
        .transform([](int v) { return v * 2; });

    if (result) {
        std::cout << *result;  // 102
    } else {
        std::cout << "错误";
    }
)" << std::endl;
#endif

    return 0;
}
```

## 参考链接

- [cppreference: std::expected（期望值类型）](https://en.cppreference.com/w/cpp/utility/expected)
