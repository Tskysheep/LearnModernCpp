---
sidebar_position: 42
title: "std::format (格式化字符串)"
description: "C++20 std::format (格式化字符串) 教学示例"
tags: [cpp20, format]
---

# std::format (格式化字符串)

C++20 标准引入的 std::format (格式化字符串) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/41_format.cpp"
/**
 * ============================================================
 * 特性名称: std::format (格式化字符串)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: std::format 提供了类型安全的字符串格式化功能，
 *          类似 Python 的 f-string 和 C 的 printf，
 *          但更安全、更灵活。
 *
 * [为什么引入]: printf 不类型安全，iostream 语法冗长，
 *             std::format 结合了两者的优点：
 *             类型安全 + 简洁的格式化语法。
 *
 * [关键要点]:
 *   - 使用 {} 作为占位符
 *   - 支持位置参数 {0}, {1}
 *   - 支持格式说明符 {:d}, {:f}, {:.2f}
 *   - 编译期检查格式字符串（C++23）
 *   - 可以自定义类型的格式化
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>

// 条件编译：检查 <format> 是否可用
#if __has_include(<format>)
    #include <format>
    #define HAS_FORMAT 1
#else
    #define HAS_FORMAT 0
#endif

#if HAS_FORMAT && defined(__cpp_lib_format)

// ===================== basic_usage() =====================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. 基本替换
    std::string s1 = std::format("你好, {}!", "C++20");
    std::cout << s1 << "\n";

    // 2. 多个参数
    std::string s2 = std::format("姓名: {}, 年龄: {}, 分数: {}", "张三", 20, 95.5);
    std::cout << s2 << "\n";

    // 3. 位置参数 (可以重复使用)
    std::string s3 = std::format("{0} + {0} = {1}", 3, 6);
    std::cout << s3 << "\n";

    std::string s4 = std::format("{1} 喜欢 {0}, {0} 也喜欢 {1}", "Alice", "Bob");
    std::cout << s4 << "\n";

    // 4. 数字格式化
    std::cout << "\n-- 数字格式化 --\n";
    std::cout << std::format("十进制: {:d}\n", 42);
    std::cout << std::format("十六进制: {:x} / {:X}\n", 255, 255);
    std::cout << std::format("八进制: {:o}\n", 255);
    std::cout << std::format("二进制: {:b}\n", 42);
    std::cout << std::format("带前缀: {:#x}, {:#o}, {:#b}\n", 255, 255, 42);

    // 5. 浮点数格式化
    std::cout << "\n-- 浮点数格式化 --\n";
    double pi = 3.14159265358979;
    std::cout << std::format("默认: {}\n", pi);
    std::cout << std::format("固定2位: {:.2f}\n", pi);
    std::cout << std::format("固定6位: {:.6f}\n", pi);
    std::cout << std::format("科学计数: {:e}\n", pi);
    std::cout << std::format("科学计数2位: {:.2e}\n", 12345.6789);

    // 6. 对齐和填充
    std::cout << "\n-- 对齐和填充 --\n";
    std::cout << std::format("|{:<15}|\n", "左对齐");
    std::cout << std::format("|{:>15}|\n", "右对齐");
    std::cout << std::format("|{:^15}|\n", "居中");
    std::cout << std::format("|{:*^15}|\n", "星号填充");
    std::cout << std::format("|{:-<15}|\n", "横线填充");
}

// ===================== advanced_usage() =====================

// 自定义类型
struct Point {
    double x, y;
};

// 为 Point 自定义格式化器
template<>
struct std::formatter<Point> {
    // 解析格式说明符（这里简单处理）
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    // 格式化输出
    auto format(const Point& p, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({:.2f}, {:.2f})", p.x, p.y);
    }
};

// 带格式选项的自定义类型
struct Color {
    unsigned char r, g, b;
};

template<>
struct std::formatter<Color> {
    char presentation = 'd';  // 'd' = 十进制, 'x' = 十六进制

    constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        if (it != ctx.end() && (*it == 'd' || *it == 'x')) {
            presentation = *it++;
        }
        return it;
    }

    auto format(const Color& c, std::format_context& ctx) const {
        if (presentation == 'x') {
            return std::format_to(ctx.out(), "#{:02x}{:02x}{:02x}",
                                  c.r, c.g, c.b);
        }
        return std::format_to(ctx.out(), "rgb({}, {}, {})",
                              c.r, c.g, c.b);
    }
};

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 自定义类型格式化
    std::cout << "-- 自定义类型 --\n";
    Point p{3.14159, 2.71828};
    std::cout << std::format("点坐标: {}\n", p);

    Color red{255, 0, 0};
    Color green{0, 128, 0};
    std::cout << std::format("红色(十进制): {:d}\n", red);
    std::cout << std::format("红色(十六进制): {:x}\n", red);
    std::cout << std::format("绿色(十六进制): {:x}\n", green);

    // 表格输出
    std::cout << "\n-- 表格格式化 --\n";
    std::cout << std::format("{:^6} {:^8} {:^8}\n", "序号", "名称", "分数");
    std::cout << std::format("{:-^6} {:-^8} {:-^8}\n", "", "", "");

    struct Record { int id; const char* name; double score; };
    Record records[] = {
        {1, "张三", 95.5},
        {2, "李四", 87.3},
        {3, "王五", 92.1}
    };

    for (const auto& r : records) {
        std::cout << std::format("{:^6d} {:^8} {:^8.1f}\n",
                                  r.id, r.name, r.score);
    }

    // format_to 写入迭代器
    std::cout << "\n-- format_to --\n";
    std::string buffer;
    std::format_to(std::back_inserter(buffer),
                   "容量测试: {} + {} = {}", 100, 200, 300);
    std::cout << buffer << "\n";

    // 数字分组（千位分隔符）
    std::cout << "\n-- 特殊格式 --\n";
    std::cout << std::format("正负号: {:+d}, {:+d}\n", 42, -42);
    std::cout << std::format("补零: {:08d}\n", 42);
    std::cout << std::format("补零十六进制: {:#010x}\n", 255);
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：不能混用位置参数和自动编号
    std::cout << "1. 不能混用位置参数和自动编号:\n";
    std::cout << "   format(\"{} {1}\", a, b);  // 错误!\n";
    std::cout << "   要么全用 {} 自动编号，要么全用 {0} {1} 位置\n\n";

    // 陷阱2：花括号转义
    std::cout << "2. 字面花括号需要转义:\n";
    std::cout << std::format("   转义: {{}} 输出为 {{}}\n");
    std::cout << std::format("   JSON: {{\"key\": \"{}\"}}\n", "value");

    // 陷阱3：类型不匹配
    std::cout << "\n3. 格式说明符必须匹配参数类型:\n";
    std::cout << "   format(\"{:d}\", 3.14);  // 编译错误! d 用于整数\n";
    std::cout << "   format(\"{:f}\", 42);    // 编译错误! f 用于浮点数\n\n";

    // 陷阱4：编译器支持
    std::cout << "4. 编译器支持情况:\n";
    std::cout << "   GCC 13+: 完整支持 std::format\n";
    std::cout << "   MSVC 19.29+: 完整支持\n";
    std::cout << "   Clang: 需要较新版本 (17+)\n";
    std::cout << "   建议使用 __cpp_lib_format 宏检测\n\n";

    // 陷阱5：性能
    std::cout << "5. std::format 创建 std::string:\n";
    std::cout << "   如果只需要输出，std::print (C++23) 更高效\n";
    std::cout << "   或使用 format_to 避免临时字符串\n";
}

#else
// ===================== 无 format 支持的降级版本 =====================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";
    std::cout << "[注意] 当前编译器不支持 <format> 或 __cpp_lib_format\n";
    std::cout << "请使用 GCC 13+, MSVC 19.29+ 或 Clang 17+ 编译\n";
    std::cout << "\nstd::format 的基本语法:\n";
    std::cout << "  std::format(\"你好, {}!\", \"C++20\")  → \"你好, C++20!\"\n";
    std::cout << "  std::format(\"{0} + {0} = {1}\", 3, 6)  → \"3 + 3 = 6\"\n";
    std::cout << "  std::format(\"{:.2f}\", 3.14159)  → \"3.14\"\n";
    std::cout << "  std::format(\"{:x}\", 255)  → \"ff\"\n";
}

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";
    std::cout << "[注意] 需要 <format> 头文件支持\n";
    std::cout << "高级功能包括:\n";
    std::cout << "  - 自定义类型的 std::formatter 特化\n";
    std::cout << "  - format_to 写入迭代器\n";
    std::cout << "  - 对齐和填充: {:<10}, {:>10}, {:^10}\n";
}

void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";
    std::cout << "1. 检测方法: #if __has_include(<format>) && defined(__cpp_lib_format)\n";
    std::cout << "2. 替代方案: 使用 {fmt} 库 (std::format 的前身)\n";
}
#endif

// ===================== main() =====================
int main() {
    std::cout << "===== std::format 格式化字符串 - C++20 =====\n\n";

#if HAS_FORMAT && defined(__cpp_lib_format)
    std::cout << "[编译器支持 std::format]\n\n";
#else
    std::cout << "[编译器不支持 std::format，显示说明性内容]\n\n";
#endif

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::format (格式化字符串)](https://en.cppreference.com/w/cpp/utility/format/format)
