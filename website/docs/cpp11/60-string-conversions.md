---
sidebar_position: 61
title: "字符串转换函数 (to_string、stoi、stol、stof等)"
description: "C++11 字符串转换函数 (to_string、stoi、stol、stof等) 教学示例"
tags: [cpp11, string_conversions]
---

# 字符串转换函数 (to_string、stoi、stol、stof等)

C++11 标准引入的 字符串转换函数 (to_string、stoi、stol、stof等) 特性完整教学示例。

## 完整源码

```cpp title="cpp11/60_string_conversions.cpp"
/**
 * ============================================================
 * 特性名称: 字符串转换函数 (to_string、stoi、stol、stof等)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: C++11在<string>头文件中新增了一组数值与字符串之间的转换函数。
 *           std::to_string 将数值转为字符串，std::stoi/stol/stof等将字符串
 *           转为对应的数值类型。
 *
 * [为什么引入]: C++03中数值与字符串转换需要使用stringstream或C语言的atoi/sprintf，
 *               语法繁琐且类型不安全。新函数提供了简洁、类型安全的转换接口。
 *
 * [关键要点]:
 *   - to_string: 支持 int、long、float、double 等数值转字符串
 *   - stoi/stol/stoll: 字符串转 int/long/long long
 *   - stoul/stoull: 字符串转 unsigned long/unsigned long long
 *   - stof/stod/stold: 字符串转 float/double/long double
 *   - 转换失败抛出 invalid_argument 或 out_of_range 异常
 * ============================================================
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. to_string: 数值转字符串
    std::cout << "[1] std::to_string 数值转字符串:\n";
    {
        int i = 42;
        long l = 1234567890L;
        float f = 3.14f;
        double d = 2.718281828;

        std::string si = std::to_string(i);
        std::string sl = std::to_string(l);
        std::string sf = std::to_string(f);
        std::string sd = std::to_string(d);

        std::cout << "  int    42 -> \"" << si << "\"" << std::endl;
        std::cout << "  long   1234567890 -> \"" << sl << "\"" << std::endl;
        std::cout << "  float  3.14f -> \"" << sf << "\"" << std::endl;
        std::cout << "  double 2.718... -> \"" << sd << "\"" << std::endl;

        // 拼接字符串
        std::string msg = "答案是 " + std::to_string(42);
        std::cout << "  字符串拼接: " << msg << std::endl;
    }
    std::cout << std::endl;

    // 2. stoi: 字符串转 int
    std::cout << "[2] std::stoi 字符串转 int:\n";
    {
        std::string s1 = "123";
        std::string s2 = "  -456  ";  // 前导空格会被跳过
        std::string s3 = "789abc";     // 只转换有效部分

        int v1 = std::stoi(s1);
        int v2 = std::stoi(s2);

        std::size_t pos;
        int v3 = std::stoi(s3, &pos);  // pos 记录停止位置

        std::cout << "  \"123\" -> " << v1 << std::endl;
        std::cout << "  \"  -456  \" -> " << v2 << std::endl;
        std::cout << "  \"789abc\" -> " << v3 << ", 停止位置: " << pos << std::endl;
    }
    std::cout << std::endl;

    // 3. 其他整数转换
    std::cout << "[3] stol、stoll、stoul、stoull:\n";
    {
        std::string s = "1234567890123";

        // long
        long lv = std::stol(s);
        std::cout << "  stol(\"" << s << "\") = " << lv << std::endl;

        // long long
        long long llv = std::stoll(s);
        std::cout << "  stoll(\"" << s << "\") = " << llv << std::endl;

        // unsigned long
        unsigned long ulv = std::stoul("4294967295");
        std::cout << "  stoul(\"4294967295\") = " << ulv << std::endl;

        // unsigned long long
        unsigned long long ullv = std::stoull("18446744073709551615");
        std::cout << "  stoull(\"18446744073709551615\") = " << ullv << std::endl;
    }
    std::cout << std::endl;

    // 4. 浮点数转换
    std::cout << "[4] stof、stod、stold:\n";
    {
        std::string s1 = "3.14159";
        std::string s2 = "2.718e2";   // 科学计数法
        std::string s3 = "  -0.5  ";

        float fv = std::stof(s1);
        double dv = std::stod(s2);
        long double ldv = std::stold(s3);

        std::cout << "  stof(\"3.14159\") = " << fv << std::endl;
        std::cout << "  stod(\"2.718e2\") = " << dv << std::endl;
        std::cout << "  stold(\"  -0.5  \") = " << static_cast<double>(ldv) << std::endl;
    }
    std::cout << std::endl;

    // 5. 进制转换
    std::cout << "[5] stoi 支持不同进制:\n";
    {
        // 第三个参数指定进制
        int dec = std::stoi("255", nullptr, 10);     // 十进制
        int hex = std::stoi("FF", nullptr, 16);       // 十六进制
        int oct = std::stoi("377", nullptr, 8);        // 八进制
        int bin = std::stoi("11111111", nullptr, 2);   // 二进制

        std::cout << "  十进制 \"255\" -> " << dec << std::endl;
        std::cout << "  十六进制 \"FF\" -> " << hex << std::endl;
        std::cout << "  八进制 \"377\" -> " << oct << std::endl;
        std::cout << "  二进制 \"11111111\" -> " << bin << std::endl;

        // 自动识别前缀
        int auto_hex = std::stoi("0xFF", nullptr, 0);   // 0x前缀 -> 十六进制
        int auto_oct = std::stoi("0377", nullptr, 0);    // 0前缀 -> 八进制
        std::cout << "  自动识别 \"0xFF\" -> " << auto_hex << std::endl;
        std::cout << "  自动识别 \"0377\" -> " << auto_oct << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. 异常处理
    std::cout << "[1] 异常处理 (invalid_argument / out_of_range):\n";
    {
        // invalid_argument: 无法转换
        try {
            int v = std::stoi("abc");
            (void)v;
        } catch (const std::invalid_argument& e) {
            std::cout << "  stoi(\"abc\"): invalid_argument - " << e.what() << std::endl;
        }

        try {
            int v = std::stoi("");
            (void)v;
        } catch (const std::invalid_argument& e) {
            std::cout << "  stoi(\"\"): invalid_argument - " << e.what() << std::endl;
        }

        // out_of_range: 超出类型范围
        try {
            int v = std::stoi("99999999999999999999");
            (void)v;
        } catch (const std::out_of_range& e) {
            std::cout << "  stoi(超大数): out_of_range - " << e.what() << std::endl;
        }

        try {
            float v = std::stof("1e39");
            (void)v;
        } catch (const std::out_of_range& e) {
            std::cout << "  stof(1e39): out_of_range - " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;

    // 2. 安全转换包装函数
    std::cout << "[2] 安全转换包装函数:\n";
    {
        auto safeStoi = [](const std::string& s, int defaultVal = 0) -> int {
            try {
                return std::stoi(s);
            } catch (const std::invalid_argument&) {
                std::cout << "    [警告] \"" << s << "\" 不是有效整数" << std::endl;
                return defaultVal;
            } catch (const std::out_of_range&) {
                std::cout << "    [警告] \"" << s << "\" 超出范围" << std::endl;
                return defaultVal;
            }
        };

        std::cout << "  安全转换 \"42\" -> " << safeStoi("42") << std::endl;
        std::cout << "  安全转换 \"abc\" -> " << safeStoi("abc", -1) << std::endl;
        std::cout << "  安全转换 \"999999999999\" -> " << safeStoi("999999999999", -1) << std::endl;
    }
    std::cout << std::endl;

    // 3. 利用 pos 参数解析复杂字符串
    std::cout << "[3] 利用 pos 参数解析复杂字符串:\n";
    {
        std::string data = "100x200x300";
        std::size_t pos = 0;

        int width = std::stoi(data, &pos);
        pos++;  // 跳过 'x'
        int height = std::stoi(data.substr(pos), &pos);
        // 注意：这里pos是相对于substr的偏移
        std::string remaining = data.substr(data.find_last_of('x') + 1);
        int depth = std::stoi(remaining);

        std::cout << "  解析 \"" << data << "\":" << std::endl;
        std::cout << "    宽: " << width << std::endl;
        std::cout << "    高: " << height << std::endl;
        std::cout << "    深: " << depth << std::endl;
    }
    std::cout << std::endl;

    // 4. to_string 精度问题与替代方案
    std::cout << "[4] to_string 的精度:\n";
    {
        double pi = 3.141592653589793;
        std::cout << "  to_string(pi) = \"" << std::to_string(pi) << "\"" << std::endl;
        std::cout << "  注意: to_string 对浮点数使用固定6位小数\n";
        std::cout << "  如需控制精度，应使用 ostringstream\n";

        // 特殊值
        double inf = std::numeric_limits<double>::infinity();
        double nan = std::numeric_limits<double>::quiet_NaN();
        std::cout << "  to_string(inf) = \"" << std::to_string(inf) << "\"" << std::endl;
        std::cout << "  to_string(nan) = \"" << std::to_string(nan) << "\"" << std::endl;
    }
    std::cout << std::endl;

    // 5. 与旧方法对比
    std::cout << "[5] 与旧方法 (atoi/sprintf) 对比:\n";
    {
        std::cout << "  旧方法 atoi:\n";
        std::cout << "    - atoi(\"abc\") 返回 0，无法区分错误和合法的0\n";
        std::cout << "    - 溢出时行为未定义\n";
        std::cout << "  新方法 stoi:\n";
        std::cout << "    - stoi(\"abc\") 抛出 invalid_argument 异常\n";
        std::cout << "    - 溢出抛出 out_of_range 异常\n";
        std::cout << "    - 更安全、更可靠\n";
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. stoi 会忽略尾部非数字字符
    std::cout << "[1] stoi 会忽略尾部非数字字符:\n";
    {
        int v = std::stoi("42abc");
        std::cout << "  stoi(\"42abc\") = " << v << " (不会报错！)" << std::endl;
        std::cout << "  如需严格校验，应检查 pos 是否等于字符串长度:\n";

        auto strictStoi = [](const std::string& s) -> int {
            std::size_t pos;
            int result = std::stoi(s, &pos);
            if (pos != s.size()) {
                throw std::invalid_argument("字符串包含非数字字符: " + s);
            }
            return result;
        };

        try {
            strictStoi("42abc");
        } catch (const std::invalid_argument& e) {
            std::cout << "  严格模式: " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;

    // 2. to_string 的浮点精度
    std::cout << "[2] to_string 浮点精度固定为6位:\n";
    {
        std::cout << "  to_string(0.1) = \"" << std::to_string(0.1) << "\"" << std::endl;
        std::cout << "  to_string(1e-7) = \"" << std::to_string(1e-7) << "\"" << std::endl;
        std::cout << "  小数可能显示为 0.000000，大数可能很长\n";
        std::cout << "  建议: 对精度有要求时使用 ostringstream + setprecision\n";
    }
    std::cout << std::endl;

    // 3. stoi 与 stol 的范围差异
    std::cout << "[3] 注意不同函数的值范围:\n";
    {
        std::cout << "  stoi  范围: " << std::numeric_limits<int>::min()
                  << " ~ " << std::numeric_limits<int>::max() << std::endl;
        std::cout << "  stol  范围: " << std::numeric_limits<long>::min()
                  << " ~ " << std::numeric_limits<long>::max() << std::endl;
        std::cout << "  stoll 范围: " << std::numeric_limits<long long>::min()
                  << " ~ " << std::numeric_limits<long long>::max() << std::endl;
    }
    std::cout << std::endl;

    // 4. 空字符串
    std::cout << "[4] 空字符串或纯空白字符串:\n";
    {
        try {
            std::stoi("");
        } catch (const std::invalid_argument&) {
            std::cout << "  stoi(\"\") 抛出 invalid_argument" << std::endl;
        }
        try {
            std::stoi("   ");
        } catch (const std::invalid_argument&) {
            std::cout << "  stoi(\"   \") 抛出 invalid_argument" << std::endl;
        }
    }
    std::cout << std::endl;

    // 5. stof 与 stod 的精度差异
    std::cout << "[5] stof (float) 与 stod (double) 精度差异:\n";
    {
        std::string s = "3.141592653589793";
        float fv = std::stof(s);
        double dv = std::stod(s);

        std::cout << "  stof: " << fv << " (约7位有效数字)" << std::endl;
        std::cout << "  stod: " << dv << " (约15位有效数字)" << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 字符串转换 (to_string/stoi/stol/stof) - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 字符串转换函数 (to_string、stoi、stol、stof等)](https://en.cppreference.com/w/cpp/string/basic_string/stol)
