---
sidebar_position: 42
title: "optional 的 monadic 操作"
description: "C++23 optional 的 monadic 操作 教学示例"
tags: [cpp23, optional_monadic]
---

# optional 的 monadic 操作

C++23 标准引入的 optional 的 monadic 操作 特性完整教学示例。

## 完整源码

```cpp title="cpp23/41_optional_monadic.cpp"
/**
 * ============================================================
 * 特性名称: optional 的 monadic 操作
 * 所属标准: C++23
 * ============================================================
 *
 * [是什么]: C++23 为 std::optional 新增了三个 monadic（单子）操作：
 *          and_then()、transform()、or_else()。这些操作允许对
 *          optional 进行链式调用，避免了繁琐的 if-has_value 嵌套。
 *
 * [为什么引入]: 之前使用 optional 时，每次操作都需要检查是否有值，
 *              导致代码嵌套层级深且重复。monadic 操作让代码更加
 *              简洁、线性、函数式，提高可读性。
 *
 * [关键要点]:
 *   - and_then(f): f 接受值并返回 optional<U>，用于可能失败的链式操作
 *   - transform(f): f 接受值并返回 U，自动包装为 optional<U>
 *   - or_else(f): 值为空时调用 f，f 返回 optional<T> 作为替代
 *   - 这三个操作与 std::expected 的同名操作语义一致
 * ============================================================
 */

#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <charconv>
#include <cmath>

// ============================================================
// 特性检测
// ============================================================
// optional 的 monadic 操作需要 __cpp_lib_optional >= 202110L
#ifdef __cpp_lib_optional
#if __cpp_lib_optional >= 202110L
#define HAS_OPTIONAL_MONADIC 1
#else
#define HAS_OPTIONAL_MONADIC 0
#endif
#else
#define HAS_OPTIONAL_MONADIC 0
#endif

#if HAS_OPTIONAL_MONADIC

// ============================================================
// 基础用法
// ============================================================
namespace basic_usage {

// 模拟一个用户查找函数
std::optional<std::string> find_user(int id) {
    if (id == 1) return "Alice";
    if (id == 2) return "Bob";
    return std::nullopt;
}

// 获取用户的邮箱（可能失败）
std::optional<std::string> get_email(const std::string& name) {
    if (name == "Alice") return "alice@example.com";
    return std::nullopt;
}

// 获取邮箱域名（不会失败，但返回转换后的值）
std::string extract_domain(const std::string& email) {
    auto pos = email.find('@');
    if (pos != std::string::npos) return email.substr(pos + 1);
    return "";
}

void demonstrate() {
    std::cout << "[基础用法]" << std::endl;

    // --- and_then: 链式可能失败的操作 ---
    std::cout << "  -- and_then --" << std::endl;

    // 成功链：find_user(1) -> get_email("Alice") -> 有值
    auto email1 = find_user(1).and_then(get_email);
    std::cout << "  用户1的邮箱: " << email1.value_or("(未找到)") << std::endl;

    // 第一步失败：find_user(999) -> nullopt，后续不执行
    auto email2 = find_user(999).and_then(get_email);
    std::cout << "  用户999的邮箱: " << email2.value_or("(未找到)") << std::endl;

    // 第二步失败：find_user(2) -> "Bob" -> get_email("Bob") -> nullopt
    auto email3 = find_user(2).and_then(get_email);
    std::cout << "  用户2的邮箱: " << email3.value_or("(未找到)") << std::endl;

    // --- transform: 对有值的 optional 应用普通函数 ---
    std::cout << "  -- transform --" << std::endl;

    auto domain = find_user(1)
        .and_then(get_email)
        .transform(extract_domain);

    std::cout << "  用户1邮箱的域名: " << domain.value_or("(未找到)") << std::endl;

    // transform 可以改变类型
    auto name_len = find_user(1)
        .transform([](const std::string& name) { return name.length(); });
    std::cout << "  用户1名字长度: " << name_len.value_or(0) << std::endl;

    // --- or_else: 空值时的替代方案 ---
    std::cout << "  -- or_else --" << std::endl;

    auto user = find_user(999)
        .or_else([]() -> std::optional<std::string> {
            std::cout << "  (未找到用户，使用默认值)" << std::endl;
            return "Guest";
        });
    std::cout << "  用户: " << user.value_or("(无)") << std::endl;

    std::cout << std::endl;
}

} // namespace basic_usage

// ============================================================
// 进阶用法
// ============================================================
namespace advanced_usage {

// 安全的字符串转整数
std::optional<int> safe_stoi(const std::string& s) {
    int result{};
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), result);
    if (ec == std::errc{} && ptr == s.data() + s.size()) {
        return result;
    }
    return std::nullopt;
}

// 安全的整数开方
std::optional<double> safe_sqrt(int x) {
    if (x < 0) return std::nullopt;
    return std::sqrt(static_cast<double>(x));
}

// 配置查找模拟
struct Config {
    std::optional<std::string> get(const std::string& key) const {
        if (key == "port") return "8080";
        if (key == "host") return "localhost";
        if (key == "timeout") return "30";
        return std::nullopt;
    }
};

void demonstrate() {
    std::cout << "[进阶用法]" << std::endl;

    // 场景1：多步数据转换管道
    std::cout << "  -- 数据转换管道 --" << std::endl;

    auto process = [](const std::string& input) {
        return safe_stoi(input)
            .and_then(safe_sqrt)
            .transform([](double v) { return v * 100; })
            .transform([](double v) { return static_cast<int>(v); });
    };

    std::cout << "  \"16\" -> stoi -> sqrt -> *100 -> int = "
              << process("16").value_or(-1) << std::endl;
    std::cout << "  \"-4\" -> stoi -> sqrt -> *100 -> int = "
              << process("-4").value_or(-1) << " (负数开方失败)" << std::endl;
    std::cout << "  \"abc\" -> stoi -> sqrt -> *100 -> int = "
              << process("abc").value_or(-1) << " (解析失败)" << std::endl;

    // 场景2：配置读取与默认值
    std::cout << "  -- 配置读取链 --" << std::endl;
    Config config;

    auto port = config.get("port")
        .and_then(safe_stoi)
        .or_else([]() -> std::optional<int> {
            std::cout << "  (使用默认端口)" << std::endl;
            return 3000;
        });
    std::cout << "  端口: " << port.value_or(0) << std::endl;

    auto unknown = config.get("unknown_key")
        .and_then(safe_stoi)
        .or_else([]() -> std::optional<int> {
            std::cout << "  (未知配置，使用默认值)" << std::endl;
            return 42;
        });
    std::cout << "  未知配置: " << unknown.value_or(0) << std::endl;

    // 场景3：对比传统写法和 monadic 写法
    std::cout << "  -- 传统写法 vs monadic 写法 --" << std::endl;
    std::cout << R"(
    // 传统写法（层层嵌套）:
    auto user = find_user(id);
    if (user) {
        auto email = get_email(*user);
        if (email) {
            auto domain = extract_domain(*email);
            // 使用 domain...
        }
    }

    // Monadic 写法（线性链式）:
    auto domain = find_user(id)
        .and_then(get_email)
        .transform(extract_domain);
    )" << std::endl;

    // 场景4：复杂的错误恢复链
    std::cout << "  -- 多重 or_else 恢复链 --" << std::endl;
    auto get_value = [&config](const std::string& key) {
        return config.get(key)
            .or_else([&]() { return config.get(key + "_default"); })
            .or_else([]() -> std::optional<std::string> { return "fallback"; });
    };
    std::cout << "  get_value(\"port\") = " << get_value("port").value() << std::endl;
    std::cout << "  get_value(\"missing\") = " << get_value("missing").value() << std::endl;

    std::cout << std::endl;
}

} // namespace advanced_usage

// ============================================================
// 易错点与陷阱
// ============================================================
namespace pitfalls {

void demonstrate() {
    std::cout << "[易错点与陷阱]" << std::endl;

    // 陷阱1：and_then 和 transform 的返回类型区别
    std::cout << "  陷阱1: and_then vs transform 的返回类型" << std::endl;
    std::cout << "    - and_then(f): f 必须返回 optional<U>" << std::endl;
    std::cout << "    - transform(f): f 返回 U，自动包装为 optional<U>" << std::endl;
    std::cout << "    - 用错会导致编译错误" << std::endl;

    // 示例
    std::optional<int> opt = 42;

    // transform: 返回普通值
    auto r1 = opt.transform([](int x) { return x * 2; });
    std::cout << "    transform(x*2): " << r1.value_or(0) << std::endl;

    // and_then: 返回 optional
    auto r2 = opt.and_then([](int x) -> std::optional<int> {
        if (x > 0) return x * 2;
        return std::nullopt;
    });
    std::cout << "    and_then(x*2 if >0): " << r2.value_or(0) << std::endl;

    // 陷阱2：or_else 的回调必须返回相同的 optional 类型
    std::cout << "  陷阱2: or_else 的返回类型必须与原 optional 一致" << std::endl;
    std::cout << "    - optional<int>::or_else(f) -> f 必须返回 optional<int>" << std::endl;

    // 陷阱3：链式操作不修改原始 optional
    std::cout << "  陷阱3: monadic 操作返回新的 optional，不修改原始值" << std::endl;
    std::optional<int> original = 10;
    auto transformed = original.transform([](int x) { return x * 3; });
    std::cout << "    原始值: " << original.value() << std::endl;
    std::cout << "    变换后: " << transformed.value() << std::endl;

    // 陷阱4：空 optional 上 transform 不执行回调
    std::cout << "  陷阱4: 空 optional 上 transform/and_then 不执行回调" << std::endl;
    std::optional<int> empty;
    bool called = false;
    auto result = empty.transform([&called](int x) {
        called = true;
        return x * 2;
    });
    std::cout << "    空 optional.transform() 是否调用了回调: "
              << (called ? "是" : "否") << std::endl;
    std::cout << "    结果 has_value: " << result.has_value() << std::endl;

    // 陷阱5：transform 返回 void 时得到 optional<void>
    // （注意：这在某些实现中可能不被支持）
    std::cout << "  陷阱5: 注意 transform 中不要忘记返回值" << std::endl;
    std::cout << "    - 如果 f 返回 void，行为可能因实现而异" << std::endl;

    std::cout << std::endl;
}

} // namespace pitfalls

#endif // HAS_OPTIONAL_MONADIC

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== optional 的 monadic 操作 - C++23 =====\n" << std::endl;

#if HAS_OPTIONAL_MONADIC
    basic_usage::demonstrate();
    advanced_usage::demonstrate();
    pitfalls::demonstrate();
#else
    std::cout << "[提示] 当前编译器不支持 optional 的 monadic 操作。" << std::endl;
    std::cout << "需要 __cpp_lib_optional >= 202110L" << std::endl;
#ifdef __cpp_lib_optional
    std::cout << "当前 __cpp_lib_optional = " << __cpp_lib_optional << std::endl;
#else
    std::cout << "__cpp_lib_optional 未定义" << std::endl;
#endif
    std::cout << "请使用支持 C++23 的编译器（如 GCC 14+ 或 Clang 18+）。" << std::endl;
    std::cout << std::endl;
    std::cout << "optional monadic 操作示例（供参考）：" << std::endl;
    std::cout << R"(
    #include <optional>

    std::optional<int> safe_stoi(const std::string& s);
    std::optional<double> safe_sqrt(int x);

    // and_then: 链式可能失败的操作
    auto result = safe_stoi("16")
        .and_then(safe_sqrt);           // optional<double>

    // transform: 对值应用普通函数
    auto doubled = safe_stoi("42")
        .transform([](int x) { return x * 2; });  // optional<int>

    // or_else: 空值时提供替代
    auto with_default = safe_stoi("bad")
        .or_else([]() -> std::optional<int> { return 0; });
)" << std::endl;
#endif

    return 0;
}
```

## 参考链接

- [cppreference: optional 的 monadic 操作](https://en.cppreference.com/w/cpp/utility/optional)
