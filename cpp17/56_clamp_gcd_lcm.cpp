/**
 * ============================================================
 * 特性名称: std::clamp、std::gcd、std::lcm (数学工具)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: C++17 新增的三个实用数学函数：
 *          - std::clamp(value, lo, hi) —— 将值钳制在 [lo, hi] 范围内
 *          - std::gcd(a, b) —— 计算最大公约数 (Greatest Common Divisor)
 *          - std::lcm(a, b) —— 计算最小公倍数 (Least Common Multiple)
 *
 * [为什么引入]: 这些是非常常用的数学操作，之前需要自己实现或使用
 *              非标准的扩展。C++17 将它们纳入标准库，方便使用。
 *
 * [关键要点]:
 *   - clamp 在 <algorithm> 中，gcd/lcm 在 <numeric> 中
 *   - clamp(v, lo, hi): v < lo 返回 lo，v > hi 返回 hi，否则返回 v
 *   - gcd/lcm 只接受整数类型，结果总是非负数
 *   - gcd(0, 0) = 0，gcd(a, 0) = |a|
 *   - lcm(a, 0) = 0
 * ============================================================
 */

#include <iostream>
#include <algorithm>  // std::clamp
#include <numeric>    // std::gcd, std::lcm
#include <vector>
#include <string>
#include <cmath>
#include <cassert>
#include <array>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // ---- std::clamp ----
    std::cout << "== std::clamp ==\n";

    // 1. 基本用法
    int value = 15;
    int lo = 0, hi = 10;
    int clamped = std::clamp(value, lo, hi);
    std::cout << "clamp(15, 0, 10) = " << clamped << "\n";
    std::cout << "clamp(5, 0, 10)  = " << std::clamp(5, 0, 10) << "\n";
    std::cout << "clamp(-3, 0, 10) = " << std::clamp(-3, 0, 10) << "\n";

    // 2. 浮点数 clamp
    std::cout << "clamp(3.7, 0.0, 1.0) = " << std::clamp(3.7, 0.0, 1.0) << "\n";
    std::cout << "clamp(-0.5, 0.0, 1.0) = " << std::clamp(-0.5, 0.0, 1.0) << "\n";
    std::cout << "clamp(0.5, 0.0, 1.0) = " << std::clamp(0.5, 0.0, 1.0) << "\n";

    // ---- std::gcd ----
    std::cout << "\n== std::gcd ==\n";

    std::cout << "gcd(12, 8)  = " << std::gcd(12, 8) << "\n";   // 4
    std::cout << "gcd(100, 75) = " << std::gcd(100, 75) << "\n"; // 25
    std::cout << "gcd(17, 13) = " << std::gcd(17, 13) << "\n";   // 1 (互质)
    std::cout << "gcd(0, 5)   = " << std::gcd(0, 5) << "\n";     // 5
    std::cout << "gcd(0, 0)   = " << std::gcd(0, 0) << "\n";     // 0
    std::cout << "gcd(-12, 8) = " << std::gcd(-12, 8) << "\n";   // 4 (总是非负)

    // ---- std::lcm ----
    std::cout << "\n== std::lcm ==\n";

    std::cout << "lcm(4, 6)   = " << std::lcm(4, 6) << "\n";    // 12
    std::cout << "lcm(12, 8)  = " << std::lcm(12, 8) << "\n";   // 24
    std::cout << "lcm(7, 5)   = " << std::lcm(7, 5) << "\n";    // 35
    std::cout << "lcm(0, 5)   = " << std::lcm(0, 5) << "\n";    // 0
    std::cout << "lcm(3, 3)   = " << std::lcm(3, 3) << "\n";    // 3
}

// ============================================================
// 进阶用法
// ============================================================

// 1. clamp 应用：颜色值限制
struct Color {
    int r, g, b;

    Color clamp_to_valid() const {
        return {
            std::clamp(r, 0, 255),
            std::clamp(g, 0, 255),
            std::clamp(b, 0, 255)
        };
    }

    void print() const {
        std::cout << "RGB(" << r << ", " << g << ", " << b << ")";
    }
};

// 2. clamp 应用：游戏中的值限制
struct Player {
    int hp;
    int max_hp = 100;

    void take_damage(int damage) {
        hp = std::clamp(hp - damage, 0, max_hp);
    }

    void heal(int amount) {
        hp = std::clamp(hp + amount, 0, max_hp);
    }
};

// 3. gcd 应用：分数化简
struct Fraction {
    int numerator;
    int denominator;

    Fraction simplify() const {
        int g = std::gcd(std::abs(numerator), std::abs(denominator));
        if (g == 0) return {0, 1};
        int sign = (denominator < 0) ? -1 : 1;
        return {sign * numerator / g, sign * denominator / g};
    }

    void print() const {
        std::cout << numerator << "/" << denominator;
    }

    // 分数加法（利用 lcm 求通分母）
    Fraction operator+(const Fraction& other) const {
        int common = std::lcm(denominator, other.denominator);
        int new_num = numerator * (common / denominator) +
                      other.numerator * (common / other.denominator);
        return Fraction{new_num, common}.simplify();
    }
};

// 4. 多个数的 gcd/lcm（使用折叠表达式）
template<typename T, typename... Rest>
auto multi_gcd(T first, Rest... rest) {
    if constexpr (sizeof...(rest) == 0) {
        return first;
    } else {
        return std::gcd(first, multi_gcd(rest...));
    }
}

int gcd_of_vector(const std::vector<int>& v) {
    int result = v[0];
    for (size_t i = 1; i < v.size(); ++i) {
        result = std::gcd(result, v[i]);
    }
    return result;
}

long long lcm_of_vector(const std::vector<int>& v) {
    long long result = v[0];
    for (size_t i = 1; i < v.size(); ++i) {
        result = std::lcm(result, static_cast<long long>(v[i]));
    }
    return result;
}

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 颜色钳制
    Color raw{300, -20, 128};
    std::cout << "原始颜色: ";
    raw.print();
    Color valid = raw.clamp_to_valid();
    std::cout << " -> 钳制后: ";
    valid.print();
    std::cout << "\n";

    // 游戏HP管理
    Player player{80};
    std::cout << "\n玩家 HP: " << player.hp;
    player.take_damage(30);
    std::cout << " -> 受伤30: " << player.hp;
    player.heal(100);
    std::cout << " -> 治疗100: " << player.hp;
    player.take_damage(999);
    std::cout << " -> 受伤999: " << player.hp << "\n";

    // 分数化简
    std::cout << "\n分数化简:\n";
    Fraction f1{12, 8};
    std::cout << "  ";
    f1.print();
    std::cout << " = ";
    f1.simplify().print();
    std::cout << "\n";

    Fraction f2{-6, 9};
    std::cout << "  ";
    f2.print();
    std::cout << " = ";
    f2.simplify().print();
    std::cout << "\n";

    // 分数加法（利用 lcm 通分）
    Fraction a{1, 3}, b{1, 4};
    auto c = a + b;
    std::cout << "  ";
    a.print();
    std::cout << " + ";
    b.print();
    std::cout << " = ";
    c.print();
    std::cout << "\n";

    // 多个数的 gcd/lcm
    std::vector<int> nums{12, 18, 24, 36};
    std::cout << "\n多个数的 gcd{12,18,24,36} = " << gcd_of_vector(nums) << "\n";
    std::cout << "multi_gcd(12,18,24,36) = " << multi_gcd(12, 18, 24, 36) << "\n";
    std::cout << "多个数的 lcm{12,18,24,36} = " << lcm_of_vector(nums) << "\n";

    // clamp 批量操作
    std::vector<int> values{-5, 3, 7, 15, 20, -1, 8};
    std::cout << "\n批量 clamp [0, 10]: ";
    for (int v : values) {
        std::cout << std::clamp(v, 0, 10) << " ";
    }
    std::cout << "\n";

    // 与 clamp 等价的 min/max 写法对比
    int x = 15;
    // C++14 写法：
    int old_way = std::min(std::max(x, 0), 10);
    // C++17 写法：
    int new_way = std::clamp(x, 0, 10);
    std::cout << "\nmin(max(15,0),10) = " << old_way << "\n";
    std::cout << "clamp(15, 0, 10)  = " << new_way << "\n";
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: clamp 要求 lo <= hi，否则是 UB
    // std::clamp(5, 10, 0);  // UB！lo=10 > hi=0
    std::cout << "陷阱1 - clamp 要求 lo <= hi，否则未定义行为\n";

    // 陷阱 2: clamp 返回的是引用（const T&）
    //   如果传入临时值，注意生命周期
    int a = 5;
    const int& ref = std::clamp(a, 0, 10);  // OK: a 还活着
    std::cout << "陷阱2 - clamp 返回 const 引用: " << ref << "\n";
    // const int& bad = std::clamp(5, 0, 10);  // 危险：绑定到临时值
    // 但实际上编译器通常会延长临时值生命周期

    // 陷阱 3: gcd/lcm 只接受整数类型
    // std::gcd(3.14, 2.0);  // 编译错误！
    std::cout << "陷阱3 - gcd/lcm 只接受整数类型\n";

    // 陷阱 4: lcm 可能溢出
    //   lcm(a, b) = |a| / gcd(a,b) * |b|
    //   大数的 lcm 可能超过 int 范围
    std::cout << "陷阱4 - lcm 可能溢出! lcm(大数, 大数) 需要用 long long\n";
    long long big_lcm = std::lcm(100000LL, 99999LL);
    std::cout << "  lcm(100000, 99999) = " << big_lcm << "\n";

    // 陷阱 5: gcd 结果总是非负的
    std::cout << "陷阱5 - gcd(-12, -8) = " << std::gcd(-12, -8) << " (总是非负)\n";

    // 陷阱 6: clamp 与 NaN
    //   浮点数 NaN 的比较行为特殊
    //   clamp(NaN, 0.0, 1.0) 的结果是 UB
    std::cout << "陷阱6 - clamp(NaN, lo, hi) 是未定义行为\n";

    // 陷阱 7: gcd(0, 0) = 0 是特殊情况
    std::cout << "陷阱7 - gcd(0,0) = " << std::gcd(0, 0) << " (数学上未定义，C++定义为0)\n";

    // 陷阱 8: clamp 的类型要一致
    // std::clamp(5, 0L, 10L);  // 可能编译错误：int 和 long 不匹配
    // 解决：显式转换
    auto result = std::clamp(5L, 0L, 10L);
    std::cout << "陷阱8 - clamp 三个参数类型必须一致: " << result << "\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::clamp / gcd / lcm - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
