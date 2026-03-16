/**
 * ============================================================
 * 特性名称: size_t 字面量后缀 (uz/UZ)
 * 所属标准: C++23
 * ============================================================
 *
 * [是什么]: C++23 引入了 uz（或 UZ）字面量后缀，用于直接创建
 *          std::size_t 类型的整数字面量。例如 42uz 的类型就是
 *          std::size_t，而 42z 的类型是对应的有符号类型。
 *
 * [为什么引入]: 在 C++ 中，整数字面量默认是 int 类型，而容器
 *              的 size() 返回 std::size_t（无符号类型）。两者
 *              比较时会产生有符号/无符号不匹配警告。uz 后缀让
 *              我们可以直接写出 size_t 类型的字面量，消除警告。
 *
 * [关键要点]:
 *   - 42uz 或 42UZ -> std::size_t 类型
 *   - 42z 或 42Z -> std::ptrdiff_t (有符号)类型
 *   - 主要用于消除有符号/无符号比较警告
 *   - 在循环和索引计算中特别有用
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <cstddef>

// ============================================================
// 特性检测
// ============================================================
#ifdef __cpp_size_t_suffix
#define HAS_SIZE_T_LITERAL 1
#else
#define HAS_SIZE_T_LITERAL 0
#endif

// ============================================================
// 基础用法
// ============================================================
namespace basic_usage {

void demonstrate() {
    std::cout << "[基础用法]" << std::endl;

#if HAS_SIZE_T_LITERAL
    // 直接使用 uz 后缀创建 size_t 字面量
    auto a = 42uz;   // std::size_t
    auto b = 42z;    // std::ptrdiff_t（有符号）
    auto c = 42;     // int

    std::cout << "  42uz 的类型是 size_t: "
              << std::is_same_v<decltype(a), std::size_t> << std::endl;
    std::cout << "  42z  的类型是有符号: "
              << std::is_signed_v<decltype(b)> << std::endl;
    std::cout << "  42   的类型是 int:    "
              << std::is_same_v<decltype(c), int> << std::endl;

    // 在容器比较中消除警告
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // 传统写法会产生警告（有符号/无符号比较）：
    // for (int i = 0; i < vec.size(); ++i) { ... }  // 警告！

    // C++23 使用 uz 后缀：无警告
    for (auto i = 0uz; i < vec.size(); ++i) {
        if (i == 0uz) std::cout << "  vec: [";
        std::cout << vec[i];
        if (i + 1uz < vec.size()) std::cout << ", ";
        else std::cout << "]" << std::endl;
    }

    // 大小写变体
    auto x1 = 100uz;   // 小写
    auto x2 = 100UZ;   // 大写
    auto x3 = 100uZ;   // 混合
    auto x4 = 100Uz;   // 混合
    std::cout << "  uz/UZ/uZ/Uz 都是合法的后缀" << std::endl;
    std::cout << "  100uz = " << x1 << ", 100UZ = " << x2
              << ", 100uZ = " << x3 << ", 100Uz = " << x4 << std::endl;

#else
    std::cout << "  [不支持] 编译器未支持 size_t 字面量后缀" << std::endl;
    std::cout << "  替代方案: 使用 static_cast<std::size_t>(42) 或定义辅助函数" << std::endl;

    // 替代方案演示
    auto to_sz = [](auto val) -> std::size_t { return static_cast<std::size_t>(val); };
    std::vector<int> vec = {1, 2, 3, 4, 5};
    for (std::size_t i = 0; i < vec.size(); ++i) {
        if (i == 0) std::cout << "  vec: [";
        std::cout << vec[i];
        if (i + 1 < vec.size()) std::cout << ", ";
        else std::cout << "]" << std::endl;
    }
    (void)to_sz;
#endif

    std::cout << std::endl;
}

} // namespace basic_usage

// ============================================================
// 进阶用法
// ============================================================
namespace advanced_usage {

void demonstrate() {
    std::cout << "[进阶用法]" << std::endl;

#if HAS_SIZE_T_LITERAL
    // 场景1：安全的索引计算
    std::cout << "  -- 安全的索引计算 --" << std::endl;
    std::vector<std::string> names = {"Alice", "Bob", "Charlie", "Diana"};

    // 从末尾开始倒数（避免 size_t 下溢）
    auto last_idx = names.size() - 1uz;
    std::cout << "  最后一个元素: names[" << last_idx << "] = " << names[last_idx] << std::endl;

    // 场景2：与 auto 推导结合
    std::cout << "  -- 与 auto 推导结合 --" << std::endl;
    auto offset = 3uz;
    auto stride = 2uz;
    auto index = offset + stride;  // 结果仍然是 size_t
    std::cout << "  offset(3uz) + stride(2uz) = " << index
              << "，类型是 size_t: "
              << std::is_same_v<decltype(index), std::size_t> << std::endl;

    // 场景3：用 z 后缀做有符号的大小相关计算
    std::cout << "  -- z 后缀（有符号） --" << std::endl;
    auto diff = 10z - 20z;  // 有符号，结果为 -10
    std::cout << "  10z - 20z = " << diff
              << "（有符号，类型 ptrdiff_t）" << std::endl;

    // 场景4：模板参数推导
    std::cout << "  -- 模板参数推导 --" << std::endl;
    auto check_type = []<typename T>(T val) {
        if constexpr (std::is_same_v<T, std::size_t>)
            std::cout << "    " << val << " -> size_t" << std::endl;
        else if constexpr (std::is_same_v<T, int>)
            std::cout << "    " << val << " -> int" << std::endl;
        else
            std::cout << "    " << val << " -> 其他类型" << std::endl;
    };

    check_type(42);     // int
    check_type(42uz);   // size_t
    check_type(42z);    // 有符号（通常 ptrdiff_t / long long）

    // 场景5：在常量表达式中使用
    constexpr auto BUFFER_SIZE = 1024uz;
    constexpr auto HEADER_SIZE = 64uz;
    constexpr auto PAYLOAD_SIZE = BUFFER_SIZE - HEADER_SIZE;
    std::cout << "  constexpr: 缓冲区 " << BUFFER_SIZE
              << " - 头部 " << HEADER_SIZE
              << " = 负载 " << PAYLOAD_SIZE << std::endl;

#else
    std::cout << "  [不支持] 编译器未支持 size_t 字面量后缀" << std::endl;
    std::cout << "  C++23 之前的替代方案:" << std::endl;
    std::cout << "    1. static_cast<std::size_t>(42)" << std::endl;
    std::cout << "    2. std::size_t{42}" << std::endl;
    std::cout << "    3. (std::size_t)42  // 不推荐" << std::endl;

    // 用户定义字面量模拟（仅示意，不完全等价）
    std::cout << "  也可以自定义字面量运算符来模拟:" << std::endl;
    std::cout << R"(    constexpr std::size_t operator""_uz(unsigned long long v) {
        return static_cast<std::size_t>(v);
    }
    auto x = 42_uz;  // 类似效果，但需要下划线)" << std::endl;
#endif

    std::cout << std::endl;
}

} // namespace advanced_usage

// ============================================================
// 易错点与陷阱
// ============================================================
namespace pitfalls {

void demonstrate() {
    std::cout << "[易错点与陷阱]" << std::endl;

    // 陷阱1：size_t 的大小依赖平台
    std::cout << "  陷阱1: size_t 的大小依赖平台" << std::endl;
    std::cout << "    - 32位系统: size_t 通常是 4 字节" << std::endl;
    std::cout << "    - 64位系统: size_t 通常是 8 字节" << std::endl;
    std::cout << "    - 当前系统: sizeof(size_t) = " << sizeof(std::size_t) << " 字节" << std::endl;

    // 陷阱2：下溢风险仍然存在
    std::cout << "  陷阱2: 使用 uz 不能防止下溢" << std::endl;
#if HAS_SIZE_T_LITERAL
    auto danger = 0uz - 1uz;  // 下溢！得到一个巨大的正数
    std::cout << "    0uz - 1uz = " << danger << " (下溢！)" << std::endl;
#else
    std::size_t danger = std::size_t(0) - std::size_t(1);
    std::cout << "    size_t(0) - size_t(1) = " << danger << " (下溢！)" << std::endl;
#endif
    std::cout << "    使用无符号类型时仍需注意减法运算" << std::endl;

    // 陷阱3：z 和 uz 的区别
    std::cout << "  陷阱3: z 和 uz 的区别" << std::endl;
    std::cout << "    - uz: 无符号 (std::size_t)" << std::endl;
    std::cout << "    - z:  有符号 (类似 ptrdiff_t)" << std::endl;
    std::cout << "    - 需根据语境选择正确的后缀" << std::endl;

    // 陷阱4：不要与其他后缀混淆
    std::cout << "  陷阱4: 后缀组合" << std::endl;
    std::cout << "    - 42uz: size_t ✓" << std::endl;
    std::cout << "    - 42u:  unsigned int ✓" << std::endl;
    std::cout << "    - 42ul: unsigned long ✓" << std::endl;
    std::cout << "    - 42ull: unsigned long long ✓" << std::endl;
    std::cout << "    - 42zu: 无效！后缀顺序不能反 ✗" << std::endl;

    std::cout << std::endl;
}

} // namespace pitfalls

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== size_t 字面量后缀 (uz/z) - C++23 =====\n" << std::endl;

    basic_usage::demonstrate();
    advanced_usage::demonstrate();
    pitfalls::demonstrate();

    return 0;
}
