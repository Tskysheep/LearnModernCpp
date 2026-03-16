---
sidebar_position: 62
title: "⟨regex⟩ 正则表达式"
description: "C++11 ⟨regex⟩ 正则表达式 教学示例"
tags: [cpp11, regex]
---

# `<regex>` 正则表达式

C++11 标准引入的 `<regex>` 正则表达式 特性完整教学示例。

## 完整源码

```cpp title="cpp11/61_regex.cpp"
/**
 * ============================================================
 * 特性名称: <regex> 正则表达式
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: C++11标准库引入了 <regex> 头文件，提供正则表达式的匹配、搜索、
 *           替换和迭代功能，支持ECMAScript等多种语法。
 *
 * [为什么引入]: C++03没有标准的正则表达式支持，开发者需要依赖第三方库
 *               （如Boost.Regex、PCRE），<regex>将正则表达式纳入标准。
 *
 * [关键要点]:
 *   - std::regex: 正则表达式对象
 *   - std::regex_match: 完全匹配整个字符串
 *   - std::regex_search: 搜索第一个匹配的子串
 *   - std::regex_replace: 替换匹配的内容
 *   - std::smatch / std::cmatch: 匹配结果（分组捕获）
 *   - std::sregex_iterator: 迭代所有匹配
 * ============================================================
 */

#include <iostream>
#include <regex>
#include <string>
#include <vector>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. regex_match: 完全匹配
    std::cout << "[1] regex_match (完全匹配):\n";
    {
        std::regex pattern(R"(\d{3}-\d{4}-\d{4})");  // 手机号格式

        std::string s1 = "138-1234-5678";
        std::string s2 = "abc-1234-5678";
        std::string s3 = "138-1234-5678 多余文字";

        std::cout << "  \"" << s1 << "\" 匹配: "
                  << (std::regex_match(s1, pattern) ? "是" : "否") << std::endl;
        std::cout << "  \"" << s2 << "\" 匹配: "
                  << (std::regex_match(s2, pattern) ? "是" : "否") << std::endl;
        std::cout << "  \"" << s3 << "\" 匹配: "
                  << (std::regex_match(s3, pattern) ? "是" : "否")
                  << " (match要求完全匹配)" << std::endl;
    }
    std::cout << std::endl;

    // 2. regex_search: 搜索子串匹配
    std::cout << "[2] regex_search (搜索子串匹配):\n";
    {
        std::string text = "今天是2024年3月15日，明天是2024年3月16日。";
        std::regex datePattern(R"((\d{4})年(\d{1,2})月(\d{1,2})日)");
        std::smatch match;

        if (std::regex_search(text, match, datePattern)) {
            std::cout << "  找到第一个日期: " << match[0] << std::endl;
            std::cout << "    年: " << match[1] << std::endl;
            std::cout << "    月: " << match[2] << std::endl;
            std::cout << "    日: " << match[3] << std::endl;
            std::cout << "    匹配位置: " << match.position() << std::endl;
            std::cout << "    匹配长度: " << match.length() << std::endl;
        }
    }
    std::cout << std::endl;

    // 3. 分组捕获
    std::cout << "[3] 分组捕获:\n";
    {
        std::string email = "user@example.com";
        std::regex emailPattern(R"((\w+)@(\w+)\.(\w+))");
        std::smatch match;

        if (std::regex_match(email, match, emailPattern)) {
            std::cout << "  完整匹配: " << match[0] << std::endl;
            std::cout << "  用户名: " << match[1] << std::endl;
            std::cout << "  域名: " << match[2] << std::endl;
            std::cout << "  后缀: " << match[3] << std::endl;
            std::cout << "  分组数量: " << match.size() - 1 << std::endl;
        }
    }
    std::cout << std::endl;

    // 4. regex_replace: 替换
    std::cout << "[4] regex_replace (替换):\n";
    {
        std::string text = "手机号: 13812345678, 另一个: 13998765432";
        // 将手机号中间4位替换为****
        std::regex phonePattern(R"((\d{3})\d{4}(\d{4}))");
        std::string result = std::regex_replace(text, phonePattern, "$1****$2");
        std::cout << "  原文: " << text << std::endl;
        std::cout << "  脱敏: " << result << std::endl;

        // 简单替换
        std::string html = "<b>粗体</b> 和 <i>斜体</i>";
        std::regex tagPattern(R"(<[^>]+>)");
        std::string plain = std::regex_replace(html, tagPattern, "");
        std::cout << "  去除HTML标签: \"" << plain << "\"" << std::endl;
    }
    std::cout << std::endl;

    // 5. 基本正则语法
    std::cout << "[5] 常用正则语法回顾:\n";
    {
        std::cout << "  .      任意字符\n";
        std::cout << "  \\d     数字 [0-9]\n";
        std::cout << "  \\w     单词字符 [a-zA-Z0-9_]\n";
        std::cout << "  \\s     空白字符\n";
        std::cout << "  *      零次或多次\n";
        std::cout << "  +      一次或多次\n";
        std::cout << "  ?      零次或一次\n";
        std::cout << "  {n,m}  n到m次\n";
        std::cout << "  [abc]  字符类\n";
        std::cout << "  (...)  分组捕获\n";
        std::cout << "  |      或\n";
        std::cout << "  ^$     行首/行尾\n";
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. sregex_iterator 迭代所有匹配
    std::cout << "[1] sregex_iterator (迭代所有匹配):\n";
    {
        std::string text = "价格: 19.99元, 折扣: 5.50元, 运费: 3.00元";
        std::regex pricePattern(R"(\d+\.\d{2})");

        std::sregex_iterator begin(text.begin(), text.end(), pricePattern);
        std::sregex_iterator end;

        double total = 0;
        std::cout << "  找到的所有价格:\n";
        for (auto it = begin; it != end; ++it) {
            std::cout << "    " << (*it)[0] << " 元" << std::endl;
            total += std::stod((*it)[0].str());
        }
        std::cout << "  总计: " << total << " 元" << std::endl;
    }
    std::cout << std::endl;

    // 2. sregex_token_iterator (分割字符串)
    std::cout << "[2] sregex_token_iterator (正则分割字符串):\n";
    {
        std::string csv = "张三,25,男,北京";
        std::regex comma(",");

        // -1 表示返回不匹配的部分（即分割后的各段）
        std::sregex_token_iterator begin(csv.begin(), csv.end(), comma, -1);
        std::sregex_token_iterator end;

        std::cout << "  分割 CSV:\n";
        int idx = 0;
        for (auto it = begin; it != end; ++it) {
            std::cout << "    [" << idx++ << "] " << *it << std::endl;
        }

        // 也可以用 0 获取匹配的分隔符
        // 或用正数获取指定分组
    }
    std::cout << std::endl;

    // 3. 正则选项
    std::cout << "[3] 正则表达式选项:\n";
    {
        // 忽略大小写
        std::regex pattern("hello", std::regex::icase);
        std::cout << "  icase匹配 \"HELLO\": "
                  << (std::regex_match(std::string("HELLO"), pattern) ? "是" : "否") << std::endl;

        // 使用不同的正则语法
        // std::regex::ECMAScript (默认)
        // std::regex::basic      (POSIX基本)
        // std::regex::extended   (POSIX扩展)
        // std::regex::grep
        // std::regex::egrep

        std::regex extPattern("[[:digit:]]+", std::regex::extended);
        std::cout << "  POSIX扩展语法匹配数字 \"12345\": "
                  << (std::regex_match(std::string("12345"), extPattern) ? "是" : "否") << std::endl;
    }
    std::cout << std::endl;

    // 4. 复杂模式匹配
    std::cout << "[4] 复杂模式匹配 (IP地址验证):\n";
    {
        // 简化的IP地址正则（不验证0-255范围）
        std::regex ipPattern(R"((\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3}))");

        auto validateIP = [&ipPattern](const std::string& ip) {
            std::smatch match;
            if (!std::regex_match(ip, match, ipPattern)) return false;

            for (int i = 1; i <= 4; ++i) {
                int val = std::stoi(match[i].str());
                if (val < 0 || val > 255) return false;
            }
            return true;
        };

        std::vector<std::string> ips = {
            "192.168.1.1", "256.1.1.1", "10.0.0.1", "abc.def.ghi.jkl"
        };

        for (const auto& ip : ips) {
            std::cout << "  " << ip << ": "
                      << (validateIP(ip) ? "有效" : "无效") << std::endl;
        }
    }
    std::cout << std::endl;

    // 5. regex_replace 高级用法
    std::cout << "[5] regex_replace 高级用法:\n";
    {
        // 使用替换标志
        std::string text = "AAA BBB AAA CCC AAA";
        std::regex pattern("AAA");

        // 只替换第一个匹配
        std::string first = std::regex_replace(text, pattern, "XXX",
                                                std::regex_constants::format_first_only);
        std::cout << "  只替换第一个: " << first << std::endl;

        // 替换所有
        std::string all = std::regex_replace(text, pattern, "XXX");
        std::cout << "  替换所有: " << all << std::endl;

        // 使用反向引用交换
        std::string nameList = "张三 李四, 王五 赵六";
        std::regex nameSwap(R"((\S+)\s+(\S+))");
        std::string swapped = std::regex_replace(nameList, nameSwap, "$2 $1");
        std::cout << "  交换姓名顺序: " << swapped << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. regex_match 与 regex_search 的区别
    std::cout << "[1] regex_match 与 regex_search 的区别:\n";
    {
        std::string text = "abc123def";
        std::regex numPattern(R"(\d+)");

        std::cout << "  regex_match(\"abc123def\", \\d+): "
                  << (std::regex_match(text, numPattern) ? "是" : "否")
                  << " (必须完全匹配)" << std::endl;
        std::cout << "  regex_search(\"abc123def\", \\d+): "
                  << (std::regex_search(text, numPattern) ? "是" : "否")
                  << " (搜索子串)" << std::endl;
    }
    std::cout << std::endl;

    // 2. 正则表达式编译开销
    std::cout << "[2] 正则表达式编译开销大，应复用 regex 对象:\n";
    std::cout << "  // 错误：循环中反复编译\n";
    std::cout << "  // for (...) { std::regex r(\"...\"); regex_match(s, r); }\n";
    std::cout << "  // 正确：编译一次，多次使用\n";
    std::cout << "  // std::regex r(\"...\");\n";
    std::cout << "  // for (...) { regex_match(s, r); }\n\n";

    // 3. 异常处理
    std::cout << "[3] 无效正则表达式抛出 regex_error:\n";
    {
        try {
            std::regex bad(R"([)");  // 不匹配的括号
        } catch (const std::regex_error& e) {
            std::cout << "  regex_error: " << e.what() << std::endl;
            std::cout << "  错误代码: " << e.code() << std::endl;
        }
    }
    std::cout << std::endl;

    // 4. 转义字符
    std::cout << "[4] 注意字符串中的转义:\n";
    std::cout << "  // 普通字符串需要双重转义:\n";
    std::cout << "  // std::regex r(\"\\\\d+\");  // 表示 \\d+\n";
    std::cout << "  // 推荐使用 raw string literal:\n";
    std::cout << "  // std::regex r(R\"(\\d+)\");  // 更清晰\n\n";

    // 5. 性能注意事项
    std::cout << "[5] 性能注意事项:\n";
    std::cout << "  - C++标准库的regex实现可能比较慢\n";
    std::cout << "  - 避免对超长字符串使用复杂正则\n";
    std::cout << "  - 注意回溯导致的指数级时间复杂度\n";
    std::cout << "  - 简单的字符串操作优先用 string::find 等函数\n";
    std::cout << "  - 性能敏感场景考虑 RE2 等第三方库\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== <regex> 正则表达式 - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: `<regex>` 正则表达式](https://en.cppreference.com/w/cpp/regex)
