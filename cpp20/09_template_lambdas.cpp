/**
 * ============================================================
 * 特性名称: 模板 Lambda (Template Lambdas)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: C++20 允许 Lambda 表达式使用显式模板参数列表，
 *          语法为 []<typename T>(T arg) {}。
 *
 * [为什么引入]: C++14 的泛型 Lambda（auto 参数）无法直接访问类型参数，
 *             有些场景需要显式指定或约束类型参数，
 *             如完美转发、SFINAE、concept 约束等。
 *
 * [关键要点]:
 *   - 语法: []<typename T>(T arg) { ... }
 *   - 可以使用多个模板参数、非类型模板参数
 *   - 支持 concept 约束
 *   - 与 auto 参数 Lambda 对比，提供了更多类型控制能力
 * ============================================================
 */

#include <concepts>
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <utility>
#include <tuple>
#include <array>

// ===================== basic_usage() =====================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // C++14 泛型 Lambda（auto 参数）
    auto generic_print_14 = [](const auto& value) {
        std::cout << "[C++14 auto] 值: " << value << "\n";
    };

    // C++20 模板 Lambda（显式模板参数）
    auto generic_print_20 = []<typename T>(const T& value) {
        std::cout << "[C++20 模板] 值: " << value
                  << " (类型大小: " << sizeof(T) << ")\n";
    };

    generic_print_14(42);
    generic_print_14(3.14);
    generic_print_20(42);
    generic_print_20(3.14);

    // 模板 Lambda 可以直接使用类型 T
    auto type_info = []<typename T>(T value) {
        std::cout << "值: " << value;
        if constexpr (std::is_integral_v<T>) {
            std::cout << " (整数类型)";
        } else if constexpr (std::is_floating_point_v<T>) {
            std::cout << " (浮点类型)";
        } else {
            std::cout << " (其他类型)";
        }
        std::cout << "\n";
    };

    type_info(42);
    type_info(3.14);
    type_info(std::string("hello"));

    // 多个模板参数
    auto pair_maker = []<typename T1, typename T2>(T1 a, T2 b) {
        return std::pair<T1, T2>{a, b};
    };

    auto p = pair_maker(1, std::string("hello"));
    std::cout << "\npair: (" << p.first << ", " << p.second << ")\n";
}

// ===================== advanced_usage() =====================
void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 1. concept 约束的模板 Lambda
    auto add = []<std::integral T>(T a, T b) -> T {
        return a + b;
    };

    std::cout << "-- concept 约束 --\n";
    std::cout << "add(3, 5) = " << add(3, 5) << "\n";
    // add(3.14, 2.71);  // 编译错误！不满足 std::integral

    // 2. 非类型模板参数
    auto make_array = []<typename T, std::size_t N>(T init_val) {
        std::array<T, N> arr;
        arr.fill(init_val);
        return arr;
    };

    auto arr = make_array.template operator()<int, 5>(42);
    std::cout << "\n-- 非类型模板参数 --\n";
    std::cout << "array<int, 5> 填充 42: ";
    for (auto v : arr) std::cout << v << " ";
    std::cout << "\n";

    // 3. 完美转发
    auto forwarder = []<typename... Args>(Args&&... args) {
        // 在 C++14 Lambda 中难以完美转发，因为不知道 auto&& 的原始类型
        // 模板 Lambda 可以直接使用 Args 进行转发
        return std::make_tuple(std::forward<Args>(args)...);
    };

    std::cout << "\n-- 完美转发 --\n";
    std::string s = "lvalue";
    auto t = forwarder(42, 3.14, s, std::string("rvalue"));
    std::cout << "tuple: (" << std::get<0>(t) << ", " << std::get<1>(t)
              << ", " << std::get<2>(t) << ", " << std::get<3>(t) << ")\n";

    // 4. 与 auto 参数对比：访问容器元素类型
    auto process_container = []<typename T>(const std::vector<T>& vec) {
        // 直接使用 T，不需要 decltype(vec)::value_type
        T sum{};
        for (const auto& v : vec) {
            sum += v;
        }
        std::cout << "容器元素类型大小: " << sizeof(T)
                  << ", 元素和: " << sum << "\n";
    };

    // 对比 C++14 写法：
    auto process_container_14 = [](const auto& vec) {
        // 需要用 decltype 或 value_type 获取元素类型
        using T = typename std::remove_reference_t<decltype(vec)>::value_type;
        T sum{};
        for (const auto& v : vec) {
            sum += v;
        }
        std::cout << "容器元素类型大小: " << sizeof(T)
                  << ", 元素和: " << sum << "\n";
    };

    std::cout << "\n-- 访问容器元素类型 --\n";
    std::vector<int> vi = {1, 2, 3, 4, 5};
    std::vector<double> vd = {1.1, 2.2, 3.3};

    std::cout << "[C++20 模板Lambda] ";
    process_container(vi);
    std::cout << "[C++14 auto Lambda] ";
    process_container_14(vd);

    // 5. 递归模板 Lambda
    std::cout << "\n-- 递归模板 Lambda --\n";
    auto print_all = []<typename T, typename... Rest>(
        auto& self, const T& first, const Rest&... rest
    ) -> void {
        std::cout << first;
        if constexpr (sizeof...(rest) > 0) {
            std::cout << ", ";
            self(self, rest...);
        }
    };

    std::cout << "打印所有参数: ";
    print_all(print_all, 1, 2.5, "hello", 'A');
    std::cout << "\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：显式指定模板参数的语法
    std::cout << "1. 显式指定 Lambda 模板参数需要 .template operator()<>:\n";
    auto convert = []<typename To, typename From>(From val) -> To {
        return static_cast<To>(val);
    };
    // 调用方式（略显笨重）：
    double result = convert.template operator()<double, int>(42);
    std::cout << "   convert<double>(42) = " << result << "\n";
    std::cout << "   语法: lambda.template operator()<To, From>(arg)\n\n";

    // 陷阱2：auto 参数和模板参数可以混用
    std::cout << "2. auto 参数和显式模板参数可以混用:\n";
    auto mixed = []<typename T>(T a, auto b) {
        std::cout << "   T = " << sizeof(T) << " 字节, auto = " << sizeof(b) << " 字节\n";
    };
    mixed(42, 3.14);
    std::cout << "   但要注意它们是独立的模板参数\n\n";

    // 陷阱3：模板 Lambda 的捕获
    std::cout << "3. 模板 Lambda 仍然可以使用捕获:\n";
    int x = 10;
    auto capture_and_template = [x]<typename T>(T val) {
        std::cout << "   捕获的 x = " << x << ", 参数 = " << val << "\n";
    };
    capture_and_template(42);
    capture_and_template(std::string("hello"));

    // 陷阱4：C++14 auto Lambda 中完美转发的困难
    std::cout << "\n4. C++14 Lambda 完美转发问题:\n";
    std::cout << "   auto f = [](auto&& arg) {\n";
    std::cout << "       // decltype(arg) 包含引用信息，不直观\n";
    std::cout << "       foo(std::forward<decltype(arg)>(arg));\n";
    std::cout << "   };\n";
    std::cout << "   C++20 模板 Lambda 更直观:\n";
    std::cout << "   auto f = []<typename T>(T&& arg) {\n";
    std::cout << "       foo(std::forward<T>(arg));\n";
    std::cout << "   };\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== 模板 Lambda - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
