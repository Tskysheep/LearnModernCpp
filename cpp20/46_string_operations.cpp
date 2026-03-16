/**
 * ============================================================
 * 特性名称: 字符串操作 (starts_with / ends_with)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: C++20 为 std::string 和 std::string_view 添加了
 *          starts_with() 和 ends_with() 成员函数，
 *          用于检查字符串是否以指定前缀或后缀开头/结尾。
 *
 * [为什么引入]: 前缀/后缀检查是极其常见的字符串操作，
 *             之前需要手动比较 substr、find 或 compare，
 *             代码冗长且容易出错。
 *
 * [关键要点]:
 *   - starts_with(str/char/sv) 检查前缀
 *   - ends_with(str/char/sv) 检查后缀
 *   - 同时适用于 string 和 string_view
 *   - 接受 const char*、char、string_view 参数
 * ============================================================
 */

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>

// ===================== basic_usage() =====================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    std::string url = "https://www.example.com/page.html";

    // starts_with 检查前缀
    std::cout << "URL: " << url << "\n\n";
    std::cout << "以 https 开头: " << std::boolalpha << url.starts_with("https") << "\n";
    std::cout << "以 http 开头:  " << url.starts_with("http") << "\n";
    std::cout << "以 ftp 开头:   " << url.starts_with("ftp") << "\n";

    // ends_with 检查后缀
    std::cout << "\n以 .html 结尾: " << url.ends_with(".html") << "\n";
    std::cout << "以 .css 结尾:  " << url.ends_with(".css") << "\n";
    std::cout << "以 .com 结尾:  " << url.ends_with(".com") << "\n";

    // 检查单个字符
    std::string text = "Hello, World!";
    std::cout << "\n\"" << text << "\":\n";
    std::cout << "以 'H' 开头: " << text.starts_with('H') << "\n";
    std::cout << "以 '!' 结尾: " << text.ends_with('!') << "\n";
    std::cout << "以 '?' 结尾: " << text.ends_with('?') << "\n";

    // string_view 也支持
    std::string_view sv = "C++20 is great";
    std::cout << "\nstring_view \"" << sv << "\":\n";
    std::cout << "以 \"C++\" 开头: " << sv.starts_with("C++") << "\n";
    std::cout << "以 \"great\" 结尾: " << sv.ends_with("great") << "\n";
}

// ===================== advanced_usage() =====================

// 与 C++17 及之前的手动方式对比
namespace old_style {
    // C++17 方式1：使用 find
    bool starts_with_find(const std::string& str, const std::string& prefix) {
        return str.find(prefix) == 0;
    }

    // C++17 方式2：使用 compare
    bool starts_with_compare(const std::string& str, const std::string& prefix) {
        if (prefix.size() > str.size()) return false;
        return str.compare(0, prefix.size(), prefix) == 0;
    }

    // C++17 方式3：使用 substr
    bool starts_with_substr(const std::string& str, const std::string& prefix) {
        if (prefix.size() > str.size()) return false;
        return str.substr(0, prefix.size()) == prefix;
    }

    // 后缀检查更麻烦
    bool ends_with_old(const std::string& str, const std::string& suffix) {
        if (suffix.size() > str.size()) return false;
        return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
}

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 文件类型判断
    std::cout << "-- 文件类型判断 --\n";
    std::vector<std::string> files = {
        "main.cpp", "header.h", "style.css", "index.html",
        "data.json", "config.xml", "readme.md", "image.png"
    };

    for (const auto& file : files) {
        std::string type;
        if (file.ends_with(".cpp") || file.ends_with(".h")) {
            type = "C++ 源文件";
        } else if (file.ends_with(".html") || file.ends_with(".css")) {
            type = "Web 文件";
        } else if (file.ends_with(".json") || file.ends_with(".xml")) {
            type = "数据文件";
        } else if (file.ends_with(".md")) {
            type = "文档文件";
        } else if (file.ends_with(".png") || file.ends_with(".jpg")) {
            type = "图片文件";
        } else {
            type = "未知类型";
        }
        std::cout << "  " << file << " → " << type << "\n";
    }

    // URL 协议判断
    std::cout << "\n-- URL 协议判断 --\n";
    std::vector<std::string> urls = {
        "https://secure.example.com",
        "http://old.example.com",
        "ftp://files.example.com",
        "file:///home/user/doc.txt",
        "mailto:user@example.com"
    };

    for (const auto& u : urls) {
        std::string protocol;
        if (u.starts_with("https://")) protocol = "HTTPS (安全)";
        else if (u.starts_with("http://")) protocol = "HTTP";
        else if (u.starts_with("ftp://")) protocol = "FTP";
        else if (u.starts_with("file://")) protocol = "本地文件";
        else if (u.starts_with("mailto:")) protocol = "邮件";
        else protocol = "未知协议";

        std::cout << "  " << u << " → " << protocol << "\n";
    }

    // 过滤操作
    std::cout << "\n-- 过滤操作 --\n";
    std::vector<std::string> words = {
        "apple", "banana", "ant", "avocado", "blueberry",
        "apricot", "cherry", "almond"
    };

    std::cout << "以 'a' 开头的词: ";
    for (const auto& w : words) {
        if (w.starts_with('a')) {
            std::cout << w << " ";
        }
    }
    std::cout << "\n";

    std::cout << "以 'ry' 结尾的词: ";
    for (const auto& w : words) {
        if (w.ends_with("ry")) {
            std::cout << w << " ";
        }
    }
    std::cout << "\n";

    // 与传统方式对比
    std::cout << "\n-- 与传统方式对比 --\n";
    std::string test = "Hello, World!";
    std::string prefix = "Hello";

    std::cout << "C++20 starts_with: " << test.starts_with(prefix) << "\n";
    std::cout << "find 方式:         " << old_style::starts_with_find(test, prefix) << "\n";
    std::cout << "compare 方式:      " << old_style::starts_with_compare(test, prefix) << "\n";
    std::cout << "substr 方式:       " << old_style::starts_with_substr(test, prefix) << "\n";
    std::cout << "(C++20 方式最简洁，且不会创建临时字符串)\n";

    // 链式判断
    std::cout << "\n-- 实际应用：命令解析 --\n";
    std::vector<std::string> commands = {
        "/help", "/quit", "/save game.sav", "/load game.sav", "hello"
    };

    for (const auto& cmd : commands) {
        if (cmd.starts_with("/help")) {
            std::cout << "  帮助命令\n";
        } else if (cmd.starts_with("/quit")) {
            std::cout << "  退出命令\n";
        } else if (cmd.starts_with("/save")) {
            std::cout << "  保存命令: " << cmd.substr(6) << "\n";
        } else if (cmd.starts_with("/load")) {
            std::cout << "  加载命令: " << cmd.substr(6) << "\n";
        } else if (cmd.starts_with("/")) {
            std::cout << "  未知命令: " << cmd << "\n";
        } else {
            std::cout << "  消息: " << cmd << "\n";
        }
    }
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：空字符串
    std::cout << "1. 空字符串前缀/后缀检查:\n";
    std::string s = "Hello";
    std::cout << "   \"Hello\".starts_with(\"\") = " << std::boolalpha << s.starts_with("") << "\n";
    std::cout << "   \"Hello\".ends_with(\"\") = " << s.ends_with("") << "\n";
    std::cout << "   (空字符串是任何字符串的前缀和后缀)\n\n";

    // 陷阱2：大小写敏感
    std::cout << "2. starts_with/ends_with 是大小写敏感的:\n";
    std::cout << "   \"Hello\".starts_with(\"hello\") = " << s.starts_with("hello") << "\n";
    std::cout << "   如果需要忽略大小写，需要先转换\n\n";

    // 陷阱3：不是 contains
    std::cout << "3. starts_with/ends_with 只检查头尾，不是 contains:\n";
    std::cout << "   \"Hello World\".starts_with(\"World\") = "
              << std::string("Hello World").starts_with("World") << "\n";
    std::cout << "   (C++23 才有 string::contains)\n\n";

    // 陷阱4：与 string_view 的配合
    std::cout << "4. starts_with 接受 string_view，不会创建临时字符串:\n";
    std::cout << "   但 ends_with(string_view) 内部需要比较末尾字符\n";
    std::cout << "   性能上比 find+compare 更好\n\n";

    // 陷阱5：没有 contains（C++20）
    std::cout << "5. C++20 没有 string::contains():\n";
    std::cout << "   C++20: 只有 starts_with 和 ends_with\n";
    std::cout << "   C++23: 添加了 string::contains()\n";
    std::cout << "   C++20 替代: str.find(\"xyz\") != string::npos\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== 字符串 starts_with / ends_with - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
