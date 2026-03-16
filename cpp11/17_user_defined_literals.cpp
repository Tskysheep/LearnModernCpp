/**
 * ============================================================
 * 特性名称: 用户自定义字面量 (User-Defined Literals)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: 允许通过定义 operator"" 后缀函数，为字面量提供
 *          自定义的含义和类型转换。
 *
 * [为什么引入]: C++03中的字面量只有内置类型，无法直接表达
 *   业务含义（如 100_km, 30_min, "hello"_s）。自定义字面量
 *   让代码更具表达力和类型安全性。
 *
 * [关键要点]:
 *   - 后缀名必须以下划线开头（不带下划线的保留给标准库）
 *   - 可用于整数、浮点数、字符和字符串字面量
 *   - 参数类型有固定的可选形式
 *   - C++14标准库增加了 ""s (string), ""h/min/s (chrono) 等
 * ============================================================
 */

#include <iostream>
#include <string>
#include <cstddef>   // size_t

// ============================================================
// 自定义字面量: 距离单位
// ============================================================
struct Distance {
    double meters;

    explicit Distance(double m) : meters(m) {}

    void print() const {
        if (meters >= 1000.0) {
            std::cout << meters / 1000.0 << " km";
        } else if (meters < 1.0) {
            std::cout << meters * 100.0 << " cm";
        } else {
            std::cout << meters << " m";
        }
    }
};

// 运算符重载
Distance operator+(const Distance& a, const Distance& b) {
    return Distance(a.meters + b.meters);
}

bool operator>(const Distance& a, const Distance& b) {
    return a.meters > b.meters;
}

// 自定义字面量
Distance operator"" _km(long double val) {
    return Distance(static_cast<double>(val) * 1000.0);
}

Distance operator"" _km(unsigned long long val) {
    return Distance(static_cast<double>(val) * 1000.0);
}

Distance operator"" _m(long double val) {
    return Distance(static_cast<double>(val));
}

Distance operator"" _m(unsigned long long val) {
    return Distance(static_cast<double>(val));
}

Distance operator"" _cm(long double val) {
    return Distance(static_cast<double>(val) / 100.0);
}

Distance operator"" _cm(unsigned long long val) {
    return Distance(static_cast<double>(val) / 100.0);
}

// ============================================================
// 自定义字面量: 时间单位
// ============================================================
struct Duration {
    double seconds;

    explicit Duration(double s) : seconds(s) {}

    void print() const {
        if (seconds >= 3600.0) {
            std::cout << seconds / 3600.0 << " 小时";
        } else if (seconds >= 60.0) {
            std::cout << seconds / 60.0 << " 分钟";
        } else {
            std::cout << seconds << " 秒";
        }
    }
};

Duration operator"" _h(unsigned long long val) {
    return Duration(static_cast<double>(val) * 3600.0);
}

Duration operator"" _min(unsigned long long val) {
    return Duration(static_cast<double>(val) * 60.0);
}

Duration operator"" _s(unsigned long long val) {
    return Duration(static_cast<double>(val));
}

// ============================================================
// 自定义字面量: 温度
// ============================================================
struct Temperature {
    double celsius;
    explicit Temperature(double c) : celsius(c) {}
    void print() const {
        std::cout << celsius << "\xC2\xB0" << "C"
                  << " (" << (celsius * 9.0 / 5.0 + 32.0) << "\xC2\xB0" << "F)";
    }
};

Temperature operator"" _degC(long double val) {
    return Temperature(static_cast<double>(val));
}

Temperature operator"" _degF(long double val) {
    return Temperature(static_cast<double>((val - 32.0) * 5.0 / 9.0));
}

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. 距离字面量
    auto dist1 = 5_km;
    auto dist2 = 500_m;
    auto dist3 = 30_cm;

    std::cout << "距离:" << std::endl;
    std::cout << "  5_km   = "; dist1.print(); std::cout << std::endl;
    std::cout << "  500_m  = "; dist2.print(); std::cout << std::endl;
    std::cout << "  30_cm  = "; dist3.print(); std::cout << std::endl;

    // 距离运算
    auto total = 1_km + 500_m + 50_cm;
    std::cout << "  1km + 500m + 50cm = "; total.print(); std::cout << std::endl;

    // 2. 时间字面量
    auto t1 = 2_h;
    auto t2 = 30_min;
    auto t3 = 45_s;

    std::cout << "\n时间:" << std::endl;
    std::cout << "  2_h    = "; t1.print(); std::cout << std::endl;
    std::cout << "  30_min = "; t2.print(); std::cout << std::endl;
    std::cout << "  45_s   = "; t3.print(); std::cout << std::endl;

    // 3. 温度字面量
    auto temp1 = 100.0_degC;
    auto temp2 = 212.0_degF;

    std::cout << "\n温度:" << std::endl;
    std::cout << "  100.0_degC = "; temp1.print(); std::cout << std::endl;
    std::cout << "  212.0_degF = "; temp2.print(); std::cout << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================

// 字符串字面量 operator""
// 参数必须是 (const char*, size_t)
std::string operator"" _upper(const char* str, size_t len) {
    std::string result(str, len);
    for (auto& c : result) {
        if (c >= 'a' && c <= 'z') c -= 32;
    }
    return result;
}

std::string operator"" _repeat3(const char* str, size_t len) {
    std::string s(str, len);
    return s + s + s;
}

// 整数字面量的原始字面量运算符（接收字符串）
// 可以处理任意大的整数字面量
unsigned long long operator"" _b(const char* binary_str) {
    unsigned long long result = 0;
    while (*binary_str) {
        result = (result << 1) | (*binary_str - '0');
        ++binary_str;
    }
    return result;
}

// 编译期整数后缀
constexpr long long operator"" _KB(unsigned long long val) {
    return static_cast<long long>(val) * 1024;
}

constexpr long long operator"" _MB(unsigned long long val) {
    return static_cast<long long>(val) * 1024 * 1024;
}

constexpr long long operator"" _GB(unsigned long long val) {
    return static_cast<long long>(val) * 1024 * 1024 * 1024;
}

void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 字符串字面量
    auto upper_str = "hello world"_upper;
    auto repeated = "ha"_repeat3;
    std::cout << "\"hello world\"_upper = " << upper_str << std::endl;
    std::cout << "\"ha\"_repeat3 = " << repeated << std::endl;

    // 2. 二进制字面量（C++14有0b前缀，C++11可以用自定义字面量）
    auto bin_val = 11010110_b;
    std::cout << "\n11010110_b = " << bin_val << " (十进制)" << std::endl;
    auto bin_val2 = 11111111_b;
    std::cout << "11111111_b = " << bin_val2 << " (十进制)" << std::endl;

    // 3. 数据大小字面量（constexpr，编译期计算）
    auto file_size = 4_KB;
    auto mem_size  = 512_MB;
    auto disk_size = 2_GB;

    std::cout << "\n数据大小:" << std::endl;
    std::cout << "  4_KB   = " << file_size << " 字节" << std::endl;
    std::cout << "  512_MB = " << mem_size << " 字节" << std::endl;
    std::cout << "  2_GB   = " << disk_size << " 字节" << std::endl;

    // 4. 字面量运算符的参数形式总结
    std::cout << "\n字面量运算符的参数形式:" << std::endl;
    std::cout << "  整数:   operator\"\"_x(unsigned long long)" << std::endl;
    std::cout << "  浮点:   operator\"\"_x(long double)" << std::endl;
    std::cout << "  字符串: operator\"\"_x(const char*, size_t)" << std::endl;
    std::cout << "  字符:   operator\"\"_x(char)" << std::endl;
    std::cout << "  原始:   operator\"\"_x(const char*)" << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: 不以下划线开头的后缀是保留的
    std::cout << "陷阱1: 后缀必须以下划线开头" << std::endl;
    std::cout << "  _km, _s 等: 用户自定义(OK)" << std::endl;
    std::cout << "  s, h, min 等: 保留给标准库" << std::endl;
    std::cout << "  不加下划线可能与未来标准冲突" << std::endl;

    // 陷阱2: 注意运算符参数类型
    std::cout << "\n陷阱2: 参数类型是固定的" << std::endl;
    std::cout << "  整数字面量只接受unsigned long long，不能用int" << std::endl;
    std::cout << "  浮点字面量只接受long double，不能用double" << std::endl;

    // 陷阱3: 字面量之间没有隐式转换
    // auto d = 5_km + 30_min; // 编译错误! Distance和Duration不能相加
    std::cout << "\n陷阱3: 不同单位类型之间不能直接运算" << std::endl;
    std::cout << "  5_km + 30_min 无法编译（需要显式定义运算符）" << std::endl;

    // 陷阱4: 字面量和变量的区别
    int x = 5;
    // auto d = x_km; // 错误! x_km是一个标识符，不是字面量后缀
    // auto d = (x)_km; // 也不行! 后缀只能用于字面量
    std::cout << "\n陷阱4: 自定义字面量后缀只能用于字面量，不能用于变量" << std::endl;
    (void)x;

    // 陷阱5: C++11标准库没有内置的字面量后缀
    std::cout << "\n陷阱5: 标准库字面量" << std::endl;
    std::cout << "  C++11: 无标准库字面量" << std::endl;
    std::cout << "  C++14增加: \"hello\"s (std::string), 1h, 30min, 5s (chrono)" << std::endl;

    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 后缀以下划线开头" << std::endl;
    std::cout << "  2. 用于领域特定的单位（距离、时间、角度等）" << std::endl;
    std::cout << "  3. 配合强类型使用，防止单位混用" << std::endl;
    std::cout << "  4. constexpr后缀允许编译期计算" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: 用户自定义字面量" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
