---
sidebar_position: 16
slug: 15-raw-string-literals
title: "原始字符串字面量 (Raw String Literals)"
description: "C++11 原始字符串字面量 (Raw String Literals) 教学示例"
tags: [cpp11, raw_string_literals]
---

# 原始字符串字面量 (Raw String Literals)

C++11 标准引入的 原始字符串字面量 (Raw String Literals) 特性完整教学示例。

## 完整源码

```cpp title="cpp11/15_raw_string_literals.cpp"
/**
 * ============================================================
 * 特性名称: 原始字符串字面量 (Raw String Literals)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: R"delimiter(...)delimiter" 形式的字符串字面量，
 *          其中转义字符不会被处理，内容原样保留。
 *
 * [为什么引入]: 正则表达式、Windows路径、HTML/JSON等内容中包含
 *   大量反斜杠和引号，传统字符串需要双重转义，极难阅读和维护。
 *   原始字符串让这些内容可以直接写入，无需转义。
 *
 * [关键要点]:
 *   - 基本语法: R"(内容)"
 *   - 自定义分隔符: R"delim(内容)delim"  分隔符最多16个字符
 *   - 内容中的换行、引号、反斜杠都原样保留
 *   - 可以与其他字符串前缀组合: LR"()", u8R"()", uR"()", UR"()"
 * ============================================================
 */

#include <iostream>
#include <string>
#include <regex>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. 普通字符串 vs 原始字符串
    std::string normal = "第一行\n第二行\n\t缩进行";
    std::string raw    = R"(第一行\n第二行\n\t缩进行)";

    std::cout << "普通字符串(转义生效):" << std::endl;
    std::cout << normal << std::endl;
    std::cout << "\n原始字符串(原样输出):" << std::endl;
    std::cout << raw << std::endl;

    // 2. Windows路径 —— 不再需要双反斜杠
    std::string path_normal = "C:\\Users\\张三\\Documents\\file.txt";
    std::string path_raw    = R"(C:\Users\张三\Documents\file.txt)";

    std::cout << "\nWindows路径:" << std::endl;
    std::cout << "  普通: " << path_normal << std::endl;
    std::cout << "  原始: " << path_raw << std::endl;
    std::cout << "  相同: " << (path_normal == path_raw ? "是" : "否") << std::endl;

    // 3. 正则表达式 —— 原始字符串的经典用途
    // 匹配邮箱的正则: \w+@\w+\.\w+
    std::string regex_normal = "\\w+@\\w+\\.\\w+";  // 需要双重转义!
    std::string regex_raw    = R"(\w+@\w+\.\w+)";   // 直接写!

    std::cout << "\n正则表达式:" << std::endl;
    std::cout << "  普通: " << regex_normal << std::endl;
    std::cout << "  原始: " << regex_raw << std::endl;

    std::regex pattern(regex_raw);
    std::string email = "test@example.com";
    std::cout << "  \"" << email << "\" 匹配: "
              << (std::regex_match(email, pattern) ? "是" : "否") << std::endl;

    // 4. 多行字符串
    std::string multiline = R"(这是第一行
这是第二行
    这行有缩进
这是最后一行)";
    std::cout << "\n多行字符串:" << std::endl;
    std::cout << multiline << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 自定义分隔符 —— 当内容中包含 )" 时
    // 如果内容中有 )" ，需要自定义分隔符避免歧义
    std::string json = R"json(
{
    "name": "张三",
    "age": 25,
    "address": "北京市(海淀区)",
    "quote": "他说:\"你好\"",
    "path": "C:\temp"
}
)json";
    std::cout << "JSON (自定义分隔符json):" << std::endl;
    std::cout << json << std::endl;

    // 2. 包含 )" 的字符串
    std::string tricky = R"DELIM(函数调用: foo("bar"))DELIM";
    std::cout << "包含特殊字符: " << tricky << std::endl;

    // 3. HTML模板
    std::string html = R"html(
<!DOCTYPE html>
<html>
<head>
    <title>测试页面</title>
    <style>
        body { font-family: "微软雅黑"; }
        .highlight { color: #ff0000; }
    </style>
</head>
<body>
    <h1>欢迎</h1>
    <p class="highlight">这是一个C++生成的HTML</p>
</body>
</html>
)html";
    std::cout << "HTML模板:" << html << std::endl;

    // 4. SQL查询
    std::string sql = R"sql(
SELECT u.name, u.email, o.total
FROM users u
JOIN orders o ON u.id = o.user_id
WHERE o.total > 100.00
    AND u.status = 'active'
ORDER BY o.total DESC
LIMIT 10;
)sql";
    std::cout << "SQL查询:" << sql << std::endl;

    // 5. 复杂正则表达式
    // 匹配 IPv4 地址
    std::string ipv4_regex = R"((\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3}))";
    std::cout << "IPv4正则: " << ipv4_regex << std::endl;

    std::regex ip_pattern(ipv4_regex);
    std::string ip = "192.168.1.100";
    std::cout << "\"" << ip << "\" 匹配: "
              << (std::regex_match(ip, ip_pattern) ? "是" : "否") << std::endl;

    // 6. 与字符串前缀组合
    // wchar_t原始字符串
    // auto wraw = LR"(宽字符原始字符串\n不转义)";

    // UTF-8原始字符串
    auto u8raw = u8R"(UTF-8原始字符串: \n \t \\ 都保留)";
    std::cout << "\nUTF-8原始字符串: " << u8raw << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: 原始字符串中的换行是真实的换行
    std::string s = R"(第一行
第二行)";
    std::cout << "陷阱1: 原始字符串中的换行会包含在字符串中" << std::endl;
    std::cout << "  字符串长度: " << s.size() << " (包含换行符)" << std::endl;
    std::cout << "  内容: " << s << std::endl;

    // 陷阱2: 开头和结尾的换行也会被包含
    std::string with_newlines = R"(
hello
)";
    std::cout << "\n陷阱2: 开头/结尾的换行也被包含" << std::endl;
    std::cout << "  长度: " << with_newlines.size() << std::endl;
    std::cout << "  首字符是换行: "
              << (with_newlines[0] == '\n' ? "是" : "否") << std::endl;

    // 陷阱3: 分隔符不能包含空格、括号和反斜杠
    // R"  (内容)  " // 错误! 分隔符不能有空格
    // R"((内容))"   // 错误! 分隔符不能有括号
    std::cout << "\n陷阱3: 分隔符限制" << std::endl;
    std::cout << "  不能包含: 空格、括号、反斜杠" << std::endl;
    std::cout << "  最长: 16个字符" << std::endl;

    // 陷阱4: 原始字符串不处理任何转义
    std::string no_escape = R"(\0\a\b\f\n\r\t\v\\)";
    std::cout << "\n陷阱4: 所有转义都不处理" << std::endl;
    std::cout << "  内容: " << no_escape << std::endl;
    std::cout << "  这些都是字面字符，不是控制字符" << std::endl;

    // 陷阱5: 调试时注意缩进
    std::cout << "\n陷阱5: 代码缩进会进入字符串" << std::endl;
    // 在函数内部缩进的原始字符串:
    std::string indented = R"(line1
        line2
        line3)";
    // line2和line3前面的空格是字符串的一部分!
    std::cout << "  包含缩进的字符串:" << std::endl;
    std::cout << "  [" << indented << "]" << std::endl;

    // 最佳实践
    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 正则表达式始终使用原始字符串" << std::endl;
    std::cout << "  2. 文件路径（尤其Windows）使用原始字符串" << std::endl;
    std::cout << "  3. 嵌入的代码/模板（HTML, SQL, JSON）使用原始字符串" << std::endl;
    std::cout << "  4. 注意多行原始字符串的缩进问题" << std::endl;
    std::cout << "  5. 内容含 )\" 时使用自定义分隔符" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: 原始字符串字面量" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 原始字符串字面量 (Raw String Literals)](https://en.cppreference.com/w/cpp/language/string_literal)
