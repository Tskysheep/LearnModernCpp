/**
 * ============================================================
 * 特性名称: Unicode字面量与字符类型
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: C++11引入了新的字符类型 char16_t 和 char32_t，
 *          以及对应的字符串字面量前缀 u8"", u"", U""，
 *          为Unicode支持提供了标准化的方案。
 *
 * [为什么引入]: C++03的wchar_t大小和编码在不同平台不一致
 *   (Windows上2字节UTF-16，Linux上4字节UTF-32)，导致跨平台
 *   Unicode处理困难。新类型提供了明确的、可移植的Unicode支持。
 *
 * [关键要点]:
 *   - u8"..." : UTF-8编码, 类型为 const char[] (C++11)
 *   - u"..."  : UTF-16编码, 类型为 const char16_t[]
 *   - U"..."  : UTF-32编码, 类型为 const char32_t[]
 *   - char16_t 至少16位, char32_t 至少32位
 *   - \uXXXX 和 \UXXXXXXXX 用于Unicode转义
 * ============================================================
 */

#include <iostream>
#include <string>
#include <cstdint>
#include <cstring>

// ============================================================
// 辅助函数: 打印字节内容
// ============================================================
void print_bytes(const char* label, const void* data, size_t size) {
    std::cout << label << " [" << size << "字节]: ";
    const unsigned char* bytes = static_cast<const unsigned char*>(data);
    for (size_t i = 0; i < size; ++i) {
        if (bytes[i] >= 0x20 && bytes[i] < 0x7F) {
            std::cout << static_cast<char>(bytes[i]);
        } else {
            // 以16进制打印
            std::cout << "\\x";
            const char hex[] = "0123456789ABCDEF";
            std::cout << hex[bytes[i] >> 4] << hex[bytes[i] & 0xF];
        }
    }
    std::cout << std::endl;
}

template <typename CharT>
void print_code_units(const char* label, const CharT* str, size_t count) {
    std::cout << label << " [" << count << "个码元]: ";
    for (size_t i = 0; i < count; ++i) {
        std::cout << "U+" << std::hex << static_cast<uint32_t>(str[i]) << " ";
    }
    std::cout << std::dec << std::endl;
}

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. 各种字符串前缀
    const char*     s1 = "Hello";      // 窄字符串, 取决于源文件编码
    const char*     s2 = u8"Hello";    // UTF-8 编码
    const char16_t* s3 = u"Hello";     // UTF-16 编码
    const char32_t* s4 = U"Hello";     // UTF-32 编码
    const wchar_t*  s5 = L"Hello";     // 宽字符, 平台相关

    std::cout << "各种字符串编码的大小:" << std::endl;
    std::cout << "  char:     sizeof = " << sizeof(char) << " 字节" << std::endl;
    std::cout << "  char16_t: sizeof = " << sizeof(char16_t) << " 字节" << std::endl;
    std::cout << "  char32_t: sizeof = " << sizeof(char32_t) << " 字节" << std::endl;
    std::cout << "  wchar_t:  sizeof = " << sizeof(wchar_t) << " 字节" << std::endl;

    (void)s1; (void)s2; (void)s3; (void)s4; (void)s5;

    // 2. Unicode转义字符
    // \uXXXX  : 4位十六进制, BMP范围
    // \UXXXXXXXX : 8位十六进制, 完整Unicode范围
    const char* smiley_u8 = u8"\u7B11";    // "笑" 的UTF-8
    const char16_t smile16[] = u"\u7B11";   // "笑" 的UTF-16
    const char32_t smile32[] = U"\U00007B11"; // "笑" 的UTF-32

    std::cout << "\nUnicode转义:" << std::endl;
    std::cout << "  u8\"\\u7B11\" = " << smiley_u8 << std::endl;
    std::cout << "  char16_t码值: 0x" << std::hex
              << static_cast<uint32_t>(smile16[0]) << std::dec << std::endl;
    std::cout << "  char32_t码值: 0x" << std::hex
              << static_cast<uint32_t>(smile32[0]) << std::dec << std::endl;

    // 3. UTF-8中文字符串
    const char* chinese_u8 = u8"你好世界";
    std::cout << "\nUTF-8中文: " << chinese_u8 << std::endl;
    std::cout << "  strlen = " << std::strlen(chinese_u8) << " (字节数，非字符数)" << std::endl;

    // 4. 字符字面量
    char     c1 = 'A';       // 普通字符
    char16_t c2 = u'A';      // UTF-16字符
    char32_t c3 = U'A';      // UTF-32字符
    char16_t c4 = u'\u4F60';  // '你' 的UTF-16
    char32_t c5 = U'\U0001F600'; // emoji的UTF-32 (笑脸)

    std::cout << "\n字符字面量:" << std::endl;
    std::cout << "  char 'A' = " << static_cast<int>(c1) << std::endl;
    std::cout << "  char16_t u'A' = " << static_cast<int>(c2) << std::endl;
    std::cout << "  char32_t U'A' = " << static_cast<int>(c3) << std::endl;
    std::cout << "  char16_t u'\\u4F60'(你) = 0x" << std::hex
              << static_cast<int>(c4) << std::dec << std::endl;
    std::cout << "  char32_t U'\\U0001F600' = 0x" << std::hex
              << static_cast<int>(c5) << std::dec << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 查看UTF-8编码的字节表示
    const char* text_u8 = u8"ABC你好";
    std::cout << "UTF-8 \"ABC你好\" 的字节表示:" << std::endl;
    print_bytes("  ", text_u8, std::strlen(text_u8));
    // A=0x41, B=0x42, C=0x43, 你=0xE4BDA0, 好=0xE5A5BD

    // 2. UTF-16编码
    const char16_t text_u16[] = u"ABC\u4F60\u597D"; // ABC你好
    size_t u16_count = sizeof(text_u16) / sizeof(char16_t) - 1; // 减去\0
    std::cout << "\nUTF-16 \"ABC你好\":" << std::endl;
    print_code_units("  ", text_u16, u16_count);

    // 3. UTF-32编码（每个字符一个码元，简单直接）
    const char32_t text_u32[] = U"ABC\u4F60\u597D"; // ABC你好
    size_t u32_count = sizeof(text_u32) / sizeof(char32_t) - 1;
    std::cout << "\nUTF-32 \"ABC你好\":" << std::endl;
    print_code_units("  ", text_u32, u32_count);

    // 4. 补充平面字符（如emoji）在UTF-16中需要代理对
    const char16_t emoji_u16[] = u"\U0001F600"; // 笑脸emoji
    size_t emoji_count = sizeof(emoji_u16) / sizeof(char16_t) - 1;
    std::cout << "\nUTF-16 emoji (U+1F600):" << std::endl;
    print_code_units("  ", emoji_u16, emoji_count);
    std::cout << "  需要" << emoji_count << "个char16_t (代理对)" << std::endl;

    const char32_t emoji_u32[] = U"\U0001F600";
    std::cout << "UTF-32 emoji (U+1F600):" << std::endl;
    print_code_units("  ", emoji_u32, 1);
    std::cout << "  只需1个char32_t" << std::endl;

    // 5. std::u16string 和 std::u32string
    std::u16string u16str = u"UTF-16\u5B57\u7B26\u4E32";  // "UTF-16字符串"
    std::u32string u32str = U"UTF-32\u5B57\u7B26\u4E32";  // "UTF-32字符串"
    std::cout << "\nstd::u16string 长度: " << u16str.size() << " 个char16_t" << std::endl;
    std::cout << "std::u32string 长度: " << u32str.size() << " 个char32_t" << std::endl;

    // 6. 与原始字符串组合
    const char* raw_u8 = u8R"(UTF-8原始字符串: 路径C:\Users\test)";
    std::cout << "\nu8R\"(...)\" = " << raw_u8 << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: strlen对UTF-8返回字节数，不是字符数
    const char* chinese = u8"你好";
    std::cout << "陷阱1: strlen(u8\"你好\") = " << std::strlen(chinese)
              << " (字节数, 不是字符数2)" << std::endl;

    // 陷阱2: char16_t不能表示所有Unicode字符（需要代理对）
    std::cout << "\n陷阱2: char16_t不能用一个码元表示所有Unicode字符" << std::endl;
    std::cout << "  BMP以外的字符(如emoji)需要两个char16_t(代理对)" << std::endl;
    std::cout << "  只有char32_t能保证一个码元=一个码点" << std::endl;

    // 陷阱3: 源文件编码问题
    std::cout << "\n陷阱3: 源文件自身的编码很重要" << std::endl;
    std::cout << "  u8\"...\" 会将源码中的字符转换为UTF-8" << std::endl;
    std::cout << "  但前提是编译器能正确读取源文件" << std::endl;
    std::cout << "  建议: 源文件保存为UTF-8(带或不带BOM)" << std::endl;

    // 陷阱4: C++11中u8字符串的类型
    std::cout << "\n陷阱4: C++11中u8字符串的类型是const char[]" << std::endl;
    std::cout << "  C++20引入了char8_t和std::u8string" << std::endl;
    std::cout << "  C++11/14/17中u8字面量可以赋给const char*" << std::endl;

    // 陷阱5: 标准库对Unicode的支持有限
    std::cout << "\n陷阱5: C++标准库对Unicode操作支持有限" << std::endl;
    std::cout << "  没有标准的UTF-8/16/32互转函数(codecvt已弃用)" << std::endl;
    std::cout << "  复杂Unicode处理建议使用ICU等第三方库" << std::endl;

    // 陷阱6: wchar_t的可移植性问题
    std::cout << "\n陷阱6: wchar_t大小不固定" << std::endl;
    std::cout << "  Windows: " << sizeof(wchar_t) << " 字节 (UTF-16)" << std::endl;
    std::cout << "  如需可移植的固定大小Unicode类型，用char16_t/char32_t" << std::endl;

    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 内部处理统一使用UTF-8 (u8\"...\")" << std::endl;
    std::cout << "  2. 需要逐字符处理时考虑char32_t" << std::endl;
    std::cout << "  3. 与系统API交互时按平台要求转换" << std::endl;
    std::cout << "  4. 保持源文件为UTF-8编码" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: Unicode字面量与字符类型" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
