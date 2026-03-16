/**
 * ============================================================
 * 特性名称: std::quoted
 * 所属标准: C++14
 * ============================================================
 *
 * [是什么]: std::quoted是一个I/O操纵符，用于在流操作中自动为字符串添加
 *          引号和处理转义字符。输出时自动加引号和转义，输入时自动去引号
 *          和还原转义。定义在<iomanip>头文件中。
 *
 * [为什么引入]: 在序列化/反序列化含空格的字符串时，>>操作符会在空格处截断。
 *              手动添加引号和处理转义字符容易出错。std::quoted提供了一个
 *              标准化的方式来实现带引号字符串的安全往返（round-trip）。
 *
 * [关键要点]:
 *   - quoted(s) 输出时在s两端加双引号，转义内部的引号和反斜杠
 *   - quoted(s) 输入时去掉外层引号，还原转义字符
 *   - 可自定义定界符和转义字符: quoted(s, delim, escape)
 *   - 保证"输出再输入"后得到相同的字符串（round-trip保证）
 * ============================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 基本的quoted输出
    std::string greeting = "Hello World";
    std::cout << "  普通输出: " << greeting << "\n";
    std::cout << "  quoted输出: " << std::quoted(greeting) << "\n";

    // 2. 含空格的字符串——演示问题
    std::cout << "\n  [含空格字符串的I/O问题]\n";
    std::string original = "Hello Beautiful World";

    // 不使用quoted：>> 在空格处截断
    {
        std::stringstream ss;
        ss << original;       // 写入: Hello Beautiful World
        std::string readback;
        ss >> readback;       // 读回: 只得到 "Hello"
        std::cout << "  不用quoted: 写入 \"" << original << "\"\n";
        std::cout << "              读回 \"" << readback << "\" (截断了!)\n";
    }

    // 使用quoted：完美往返
    {
        std::stringstream ss;
        ss << std::quoted(original);   // 写入: "Hello Beautiful World"
        std::string readback;
        ss >> std::quoted(readback);   // 读回: Hello Beautiful World
        std::cout << "  用quoted:   写入 " << std::quoted(original) << "\n";
        std::cout << "              读回 \"" << readback << "\" (完整!)\n";
    }

    // 3. 含引号的字符串
    std::cout << "\n  [含引号的字符串]\n";
    std::string with_quotes = "She said \"hello\"";
    std::cout << "  原始: " << with_quotes << "\n";
    std::cout << "  quoted: " << std::quoted(with_quotes) << "\n";

    // 往返测试
    {
        std::stringstream ss;
        ss << std::quoted(with_quotes);
        std::string readback;
        ss >> std::quoted(readback);
        std::cout << "  读回: " << readback << "\n";
        std::cout << "  相等? " << (with_quotes == readback ? "是" : "否") << "\n";
    }

    // 4. 含反斜杠的字符串
    std::cout << "\n  [含反斜杠的字符串]\n";
    std::string with_backslash = "path\\to\\file";
    std::cout << "  原始: " << with_backslash << "\n";
    std::cout << "  quoted: " << std::quoted(with_backslash) << "\n";

    {
        std::stringstream ss;
        ss << std::quoted(with_backslash);
        std::string readback;
        ss >> std::quoted(readback);
        std::cout << "  读回: " << readback << "\n";
        std::cout << "  相等? " << (with_backslash == readback ? "是" : "否") << "\n";
    }
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. 自定义定界符
    std::cout << "  [自定义定界符]\n";
    std::string data = "Hello World";

    // 使用单引号作为定界符
    std::cout << "  单引号: " << std::quoted(data, '\'') << "\n";

    // 使用#作为定界符，$作为转义字符
    std::cout << "  #定界符: " << std::quoted(data, '#', '$') << "\n";

    // 含定界符的字符串
    std::string with_hash = "color #FF0000";
    std::cout << "  含#: " << std::quoted(with_hash, '#', '$') << "\n";

    // 2. 序列化多个字符串
    std::cout << "\n  [序列化多个字符串]\n";
    std::vector<std::string> strings = {
        "Alice Smith",
        "Bob \"The Builder\" Jones",
        "Charlie's Angels",
        "path\\to\\file.txt",
        ""  // 空字符串
    };

    // 序列化
    std::stringstream ss;
    for (const auto& s : strings) {
        ss << std::quoted(s) << " ";
    }
    std::cout << "  序列化: " << ss.str() << "\n";

    // 反序列化
    std::vector<std::string> restored;
    std::string temp;
    while (ss >> std::quoted(temp)) {
        restored.push_back(temp);
    }

    std::cout << "  反序列化:\n";
    for (std::size_t i = 0; i < restored.size(); ++i) {
        std::cout << "    [" << i << "] \"" << restored[i] << "\"";
        if (i < strings.size()) {
            std::cout << (strings[i] == restored[i] ? " (匹配)" : " (不匹配!)");
        }
        std::cout << "\n";
    }

    // 3. 自定义转义字符
    std::cout << "\n  [自定义转义字符]\n";
    std::string csv_data = "field with \"quotes\" inside";

    // 使用双引号定界、双引号转义（类似CSV格式）
    // 默认转义字符是反斜杠
    std::cout << "  默认转义(\\\\): " << std::quoted(csv_data) << "\n";

    // 4. 空字符串和特殊情况
    std::cout << "\n  [特殊情况]\n";

    // 空字符串
    std::string empty;
    std::cout << "  空字符串 quoted: " << std::quoted(empty) << "\n";
    {
        std::stringstream ss2;
        ss2 << std::quoted(empty);
        std::string readback;
        ss2 >> std::quoted(readback);
        std::cout << "  空字符串往返: \"" << readback << "\" (长度=" << readback.size() << ")\n";
    }

    // 只有空格的字符串
    std::string spaces = "   ";
    std::cout << "  纯空格 quoted: " << std::quoted(spaces) << "\n";
    {
        std::stringstream ss3;
        ss3 << std::quoted(spaces);
        std::string readback;
        ss3 >> std::quoted(readback);
        std::cout << "  纯空格往返: \"" << readback << "\" (长度=" << readback.size() << ")\n";
    }

    // 5. 实际应用：简单的键值对序列化
    std::cout << "\n  [键值对序列化]\n";
    std::stringstream kvss;
    // 写入
    kvss << std::quoted("user name") << "=" << std::quoted("John \"Johnny\" Doe") << " ";
    kvss << std::quoted("home dir") << "=" << std::quoted("C:\\Users\\John") << " ";

    std::cout << "  序列化: " << kvss.str() << "\n";

    // 读回
    std::string key, value;
    char eq;
    kvss.seekg(0);
    while (kvss >> std::quoted(key) >> eq >> std::quoted(value)) {
        std::cout << "  读回: " << key << " = " << value << "\n";
    }
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱1：quoted只用于I/O流，不是字符串变换函数
    std::cout << "  陷阱1: quoted不返回string，而是I/O操纵符\n";
    std::cout << "    // std::string s = std::quoted(\"hello\"); // 编译错误!\n";
    std::cout << "    // 必须通过流使用:\n";
    std::cout << "    std::cout << std::quoted(\"hello\");       // OK\n";
    // 如果需要字符串，要通过stringstream中转
    std::stringstream ss;
    ss << std::quoted("hello");
    std::string result = ss.str();
    std::cout << "    通过stringstream获取: \"" << result << "\"\n";

    // 陷阱2：输入时如果没有引号，quoted当作普通字符串读取
    std::cout << "\n  陷阱2: 输入无引号时的行为\n";
    {
        std::stringstream ss2("no_quotes_here");
        std::string val;
        ss2 >> std::quoted(val);
        std::cout << "    无引号输入: \"" << val << "\" (正常读取到空格或EOF)\n";
    }
    {
        std::stringstream ss3("no quotes here");
        std::string val;
        ss3 >> std::quoted(val);
        std::cout << "    无引号含空格: \"" << val << "\" (截断在空格处!)\n";
    }

    // 陷阱3：引号不匹配
    std::cout << "\n  陷阱3: 引号不匹配的行为\n";
    {
        std::stringstream ss4("\"unclosed");
        std::string val;
        ss4 >> std::quoted(val);
        std::cout << "    未闭合引号: \"" << val << "\"\n";
        std::cout << "    流状态: " << (ss4.good() ? "good" : (ss4.eof() ? "eof" : "error")) << "\n";
    }

    // 陷阱4：quoted(const char*)只用于输出，输入必须用string
    std::cout << "\n  陷阱4: quoted输入必须用std::string\n";
    std::cout << "    // const char* buf;\n";
    std::cout << "    // ss >> std::quoted(buf);  // 编译错误!\n";
    std::cout << "    std::string s;\n";
    std::cout << "    ss >> std::quoted(s);        // OK\n";

    // 陷阱5：默认转义字符是反斜杠，可能与你的数据冲突
    std::cout << "\n  陷阱5: 默认转义字符是'\\\\'\n";
    std::string path = "C:\\new\\test";
    std::cout << "    原始路径: " << path << "\n";
    std::cout << "    quoted:   " << std::quoted(path) << "\n";
    std::cout << "    路径中的\\\\会被转义，往返是安全的\n";

    // 验证往返
    {
        std::stringstream ss5;
        ss5 << std::quoted(path);
        std::string readback;
        ss5 >> std::quoted(readback);
        std::cout << "    往返结果: " << readback << "\n";
        std::cout << "    相等? " << (path == readback ? "是" : "否") << "\n";
    }

    // 陷阱6：不同编译器的实现细节
    std::cout << "\n  陷阱6: quoted是C++14特性，确保编译器支持\n";
    std::cout << "    编译时需要 -std=c++14 或更高标准\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::quoted - C++14 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
