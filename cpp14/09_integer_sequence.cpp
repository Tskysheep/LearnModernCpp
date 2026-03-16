/**
 * ============================================================
 * 特性名称: integer_sequence与index_sequence
 * 所属标准: C++14
 * ============================================================
 *
 * [是什么]: std::integer_sequence是一个编译期整数序列的模板类，
 *          std::index_sequence是其size_t特化版本。配合
 *          std::make_index_sequence<N>可以生成0到N-1的序列。
 *          这是模板元编程的重要工具。
 *
 * [为什么引入]: C++11中操作tuple和参数包时，经常需要编译期索引序列来
 *              逐个展开元素。这种"索引技巧"需要手动实现递归模板来生成
 *              序列，代码冗长且容易出错。C++14将其标准化，大幅简化了
 *              tuple展开和参数包操作。
 *
 * [关键要点]:
 *   - integer_sequence<T, Is...> 表示类型T的编译期整数序列
 *   - index_sequence<Is...> 等价于 integer_sequence<size_t, Is...>
 *   - make_index_sequence<N> 生成 index_sequence<0, 1, ..., N-1>
 *   - 主要用于tuple展开、数组初始化、编译期循环等场景
 * ============================================================
 */

#include <iostream>
#include <tuple>
#include <array>
#include <utility>
#include <string>
#include <type_traits>
#include <functional>

// ============================================================
// 基础用法
// ============================================================

// 辅助函数：打印index_sequence的内容
template<std::size_t... Is>
void print_sequence(std::index_sequence<Is...>) {
    // 利用初始化列表展开参数包
    using expander = int[];
    std::cout << "  序列: ";
    (void)expander{0, (void(std::cout << Is << " "), 0)...};
    std::cout << "\n";
}

// 打印tuple的所有元素——核心应用
template<typename Tuple, std::size_t... Is>
void print_tuple_impl(const Tuple& t, std::index_sequence<Is...>) {
    using expander = int[];
    std::cout << "(";
    (void)expander{0, (void(
        std::cout << (Is == 0 ? "" : ", ") << std::get<Is>(t)
    ), 0)...};
    std::cout << ")";
}

template<typename... Args>
void print_tuple(const std::tuple<Args...>& t) {
    print_tuple_impl(t, std::make_index_sequence<sizeof...(Args)>{});
}

void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 生成索引序列
    std::cout << "  [make_index_sequence]\n";
    print_sequence(std::make_index_sequence<5>{});    // 0 1 2 3 4
    print_sequence(std::make_index_sequence<10>{});   // 0 1 2 3 ... 9

    // 也可以显式指定
    print_sequence(std::index_sequence<2, 4, 6, 8>{}); // 任意序列

    // 2. 打印tuple
    std::cout << "\n  [打印tuple]\n";
    auto t1 = std::make_tuple(1, 3.14, "hello", 'A');
    std::cout << "  tuple = ";
    print_tuple(t1);
    std::cout << "\n";

    auto t2 = std::make_tuple(42, std::string("C++14"));
    std::cout << "  tuple = ";
    print_tuple(t2);
    std::cout << "\n";

    // 3. integer_sequence也支持其他整数类型
    std::cout << "\n  [不同整数类型的序列]\n";
    auto int_seq = std::integer_sequence<int, 10, 20, 30>{};
    std::cout << "  integer_sequence<int>: size = " << decltype(int_seq)::size() << "\n";

    auto char_seq = std::integer_sequence<char, 'H', 'i', '!'>{};
    std::cout << "  integer_sequence<char>: size = " << decltype(char_seq)::size() << "\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 将tuple展开为函数参数（apply的实现）
template<typename Func, typename Tuple, std::size_t... Is>
auto apply_impl(Func&& f, Tuple&& t, std::index_sequence<Is...>)
    -> decltype(f(std::get<Is>(std::forward<Tuple>(t))...))
{
    return f(std::get<Is>(std::forward<Tuple>(t))...);
}

template<typename Func, typename Tuple>
auto apply(Func&& f, Tuple&& t)
    -> decltype(apply_impl(
        std::forward<Func>(f),
        std::forward<Tuple>(t),
        std::make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>{}))
{
    constexpr auto size = std::tuple_size<typename std::decay<Tuple>::type>::value;
    return apply_impl(
        std::forward<Func>(f),
        std::forward<Tuple>(t),
        std::make_index_sequence<size>{}
    );
}

// 2. 编译期数组变换
template<typename T, std::size_t N, typename Func, std::size_t... Is>
constexpr std::array<T, N> transform_array_impl(
    const std::array<T, N>& arr, Func f, std::index_sequence<Is...>)
{
    return {{f(arr[Is])...}};
}

template<typename T, std::size_t N, typename Func>
constexpr std::array<T, N> transform_array(const std::array<T, N>& arr, Func f) {
    return transform_array_impl(arr, f, std::make_index_sequence<N>{});
}

// 3. tuple转array
template<typename T, typename Tuple, std::size_t... Is>
std::array<T, sizeof...(Is)> tuple_to_array_impl(const Tuple& t, std::index_sequence<Is...>) {
    return {{static_cast<T>(std::get<Is>(t))...}};
}

template<typename T, typename... Args>
std::array<T, sizeof...(Args)> tuple_to_array(const std::tuple<Args...>& t) {
    return tuple_to_array_impl<T>(t, std::make_index_sequence<sizeof...(Args)>{});
}

// 4. 遍历tuple中的每个元素并对其执行操作
template<typename Tuple, typename Func, std::size_t... Is>
void for_each_tuple_impl(Tuple&& t, Func&& f, std::index_sequence<Is...>) {
    using expander = int[];
    (void)expander{0, (void(f(std::get<Is>(std::forward<Tuple>(t)))), 0)...};
}

template<typename Tuple, typename Func>
void for_each_tuple(Tuple&& t, Func&& f) {
    constexpr auto size = std::tuple_size<typename std::decay<Tuple>::type>::value;
    for_each_tuple_impl(std::forward<Tuple>(t), std::forward<Func>(f),
                        std::make_index_sequence<size>{});
}

// 5. 编译期生成数列
template<std::size_t... Is>
constexpr std::array<std::size_t, sizeof...(Is)>
make_squares(std::index_sequence<Is...>) {
    return {{(Is * Is)...}};
}

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. tuple展开为函数参数
    std::cout << "  [apply: tuple展开为函数参数]\n";
    auto add3 = [](int a, double b, int c) {
        return a + b + c;
    };
    auto args = std::make_tuple(1, 2.5, 3);
    auto result = apply(add3, args);
    std::cout << "    add3(1, 2.5, 3) = " << result << "\n";

    // 用tuple调用带字符串参数的函数
    auto greet = [](const std::string& name, int age) {
        return name + " is " + std::to_string(age) + " years old";
    };
    auto greeting = apply(greet, std::make_tuple(std::string("Alice"), 30));
    std::cout << "    greet = " << greeting << "\n";

    // 2. 数组变换
    std::cout << "\n  [编译期数组变换]\n";
    std::array<int, 5> arr = {{1, 2, 3, 4, 5}};
    auto doubled = transform_array(arr, [](int x) { return x * 2; });
    std::cout << "    原数组:   ";
    for (auto x : arr) std::cout << x << " ";
    std::cout << "\n    变换后(*2): ";
    for (auto x : doubled) std::cout << x << " ";
    std::cout << "\n";

    // 3. tuple转array
    std::cout << "\n  [tuple转array]\n";
    auto t = std::make_tuple(10, 20, 30, 40);
    auto a = tuple_to_array<int>(t);
    std::cout << "    tuple(10,20,30,40) -> array: ";
    for (auto x : a) std::cout << x << " ";
    std::cout << "\n";

    // 4. for_each遍历tuple
    std::cout << "\n  [for_each遍历tuple]\n";
    auto mixed = std::make_tuple(42, 3.14, std::string("hello"), 'X');
    std::cout << "    元素: ";
    for_each_tuple(mixed, [](const auto& elem) {
        std::cout << elem << " ";
    });
    std::cout << "\n";

    // 5. 编译期平方数列
    std::cout << "\n  [编译期生成平方数列]\n";
    constexpr auto squares = make_squares(std::make_index_sequence<8>{});
    std::cout << "    平方数列: ";
    for (auto s : squares) std::cout << s << " ";
    std::cout << "\n";
}

// ============================================================
// 易错点与陷阱
// ============================================================

void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱1：make_index_sequence<N>生成的是0到N-1，不是1到N
    std::cout << "  陷阱1: make_index_sequence<5> = {0,1,2,3,4}，不是{1,2,3,4,5}\n";
    std::cout << "    ";
    print_sequence(std::make_index_sequence<5>{});

    // 陷阱2：integer_sequence本身没有存储数据，只有类型信息
    std::cout << "\n  陷阱2: integer_sequence是类型，不是值\n";
    std::cout << "    它的整数序列存在于模板参数中，不是运行时数据\n";
    std::cout << "    sizeof(index_sequence<0,1,2>) = "
              << sizeof(std::index_sequence<0, 1, 2>) << " (空类)\n";

    // 陷阱3：编译时间
    std::cout << "\n  陷阱3: 过大的N可能显著增加编译时间\n";
    std::cout << "    make_index_sequence<10000> 可能导致编译缓慢\n";
    std::cout << "    实际中很少需要超过几百的序列\n";

    // 陷阱4：参数包展开的求值顺序
    std::cout << "\n  陷阱4: 参数包展开中的求值顺序\n";
    std::cout << "    函数参数的求值顺序未定义！\n";
    std::cout << "    func(get<0>(t), get<1>(t), ...) 中各get的执行顺序不确定\n";
    std::cout << "    如果有副作用，需要使用初始化列表来保证顺序\n";

    // 演示：初始化列表保证顺序
    auto t = std::make_tuple(1, 2, 3);
    std::cout << "    初始化列表保证顺序: ";
    for_each_tuple(t, [](auto elem) {
        std::cout << elem << " ";
    });
    std::cout << "\n";

    // 陷阱5：不能用于运行时确定的大小
    std::cout << "\n  陷阱5: 序列长度必须是编译期常量\n";
    std::cout << "    int n = 5;\n";
    std::cout << "    make_index_sequence<n>{}; // 错误! n不是constexpr\n";
    std::cout << "    constexpr int n = 5;\n";
    std::cout << "    make_index_sequence<n>{}; // OK\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== integer_sequence与index_sequence - C++14 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
