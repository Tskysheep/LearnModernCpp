/**
 * ============================================================
 * 特性名称: 折叠表达式 (Fold Expressions)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: 在可变参数模板 (variadic templates) 中，用简洁的语法
 *          对参数包进行折叠（归约）运算。
 *          四种形式：
 *            一元右折叠: (args op ...)
 *            一元左折叠: (... op args)
 *            二元右折叠: (args op ... op init)
 *            二元左折叠: (init op ... op args)
 *
 * [为什么引入]: C++11/14 中展开参数包需要递归模板或初始化列表技巧，
 *              代码冗长且难以理解。折叠表达式一行搞定。
 *
 * [关键要点]:
 *   - 支持大部分二元运算符：+  -  *  /  %  &  |  ^  <<  >>  &&  ||  ,
 *   - 一元折叠对空参数包：&& 返回 true，|| 返回 false，逗号返回 void()
 *   - 其他运算符对空参数包是编译错误 → 用二元折叠提供初始值
 *   - 折叠表达式必须用圆括号包裹
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

// ============================================================
// 基础用法
// ============================================================

// 1. 求和：一元左折叠 (... + args)
template<typename... Args>
auto sum(Args... args) {
    return (... + args);  // ((a1 + a2) + a3) + ...
}

// 2. 全部打印：一元左折叠 + 逗号运算符
template<typename... Args>
void print_all(Args&&... args) {
    ((std::cout << args << " "), ...);
    std::cout << "\n";
}

// 3. 逻辑与：一元折叠
template<typename... Args>
bool all_true(Args... args) {
    return (... && args);  // 空参数包返回 true
}

// 4. 逻辑或：一元折叠
template<typename... Args>
bool any_true(Args... args) {
    return (... || args);  // 空参数包返回 false
}

void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 求和
    std::cout << "sum(1,2,3,4,5) = " << sum(1, 2, 3, 4, 5) << "\n";
    std::cout << "sum(1.1, 2.2)  = " << sum(1.1, 2.2) << "\n";

    // 打印所有参数
    std::cout << "print_all: ";
    print_all(1, "hello", 3.14, 'A');

    // 逻辑运算
    std::cout << "all_true(true, true, true)  = " << all_true(true, true, true) << "\n";
    std::cout << "all_true(true, false, true) = " << all_true(true, false, true) << "\n";
    std::cout << "any_true(false, false, true) = " << any_true(false, false, true) << "\n";

    // 空参数包
    std::cout << "all_true() [空] = " << all_true() << " (默认 true)\n";
    std::cout << "any_true() [空] = " << any_true() << " (默认 false)\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 二元折叠 —— 提供初始值，处理空参数包
template<typename... Args>
auto safe_sum(Args... args) {
    return (0 + ... + args);  // 二元左折叠，初始值 0
}

// 2. 字符串拼接
template<typename... Args>
std::string concat(Args&&... args) {
    return (std::string{} + ... + std::string(args));
}

// 3. 用折叠表达式实现 push_back 到 vector
template<typename T, typename... Args>
void push_many(std::vector<T>& vec, Args&&... args) {
    (vec.push_back(std::forward<Args>(args)), ...);
}

// 4. 链式比较：检查所有值是否相等
template<typename T, typename... Args>
bool all_equal(T first, Args... rest) {
    return ((first == rest) && ...);
}

// 5. 打印带分隔符
template<typename First, typename... Rest>
void print_with_separator(const std::string& sep, First&& first, Rest&&... rest) {
    std::cout << first;
    ((std::cout << sep << rest), ...);
    std::cout << "\n";
}

// 6. 计算参数个数（展示 sizeof...）
template<typename... Args>
constexpr std::size_t count_args(Args...) {
    return sizeof...(Args);
}

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 安全求和（空参数包返回 0）
    std::cout << "safe_sum()      = " << safe_sum() << "\n";
    std::cout << "safe_sum(1,2,3) = " << safe_sum(1, 2, 3) << "\n";

    // 字符串拼接
    std::cout << "concat: " << concat("Hello", " ", "World", "!") << "\n";

    // push_many
    std::vector<int> vec;
    push_many(vec, 10, 20, 30, 40);
    std::cout << "push_many: ";
    for (auto v : vec) std::cout << v << " ";
    std::cout << "\n";

    // all_equal
    std::cout << "all_equal(5,5,5) = " << all_equal(5, 5, 5) << "\n";
    std::cout << "all_equal(5,5,3) = " << all_equal(5, 5, 3) << "\n";

    // 带分隔符打印
    std::cout << "分隔符打印: ";
    print_with_separator(" | ", "Alice", "Bob", "Charlie");

    // 计算参数个数
    std::cout << "参数个数: " << count_args(1, "a", 3.14) << "\n";
}

// ============================================================
// 四种折叠形式对比
// ============================================================

// 一元右折叠: (args op ...)  => a1 op (a2 op (a3 op a4))
template<typename... Args>
auto right_fold_sub(Args... args) {
    return (args - ...);
}

// 一元左折叠: (... op args)  => ((a1 op a2) op a3) op a4
template<typename... Args>
auto left_fold_sub(Args... args) {
    return (... - args);
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: 左折叠 vs 右折叠结果不同（非交换运算符）
    // 左折叠: ((1-2)-3)-4 = -8
    // 右折叠: 1-(2-(3-4)) = -2
    std::cout << "陷阱1 - 左折叠 (1-2-3-4): " << left_fold_sub(1, 2, 3, 4) << "\n";
    std::cout << "陷阱1 - 右折叠 (1-2-3-4): " << right_fold_sub(1, 2, 3, 4) << "\n";

    // 陷阱 2: 一元折叠对空参数包 —— 只有 &&、||、逗号 有默认值
    //         sum() 会编译错误！需要用二元折叠
    std::cout << "陷阱2 - 空参数包用二元折叠: safe_sum()=" << safe_sum() << "\n";

    // 陷阱 3: 折叠表达式必须用圆括号包裹
    // return args + ...;    // 错误！
    // return (args + ...);  // 正确
    std::cout << "陷阱3 - 折叠表达式必须用圆括号 () 包裹\n";

    // 陷阱 4: 运算符优先级 —— 折叠表达式内不要混合运算符
    // (args + ... * init) 是非法的 —— 折叠只能有一个运算符
    std::cout << "陷阱4 - 折叠表达式内只能有一个运算符\n";

    // 陷阱 5: 逗号折叠的副作用顺序是确定的（从左到右）
    std::cout << "陷阱5 - 逗号折叠顺序: ";
    auto print_indexed = [](auto... args) {
        int i = 0;
        ((std::cout << "[" << i++ << "]=" << args << " "), ...);
        std::cout << "\n";
    };
    print_indexed("a", "b", "c");
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 折叠表达式 (Fold Expressions) - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
