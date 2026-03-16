/**
 * ============================================================
 * 特性名称: 统一初始化 (Uniform Initialization / Brace Initialization)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: 使用花括号{}进行初始化的统一语法，适用于所有类型：
 *          基本类型、数组、聚合体、类、容器等。
 *
 * [为什么引入]: C++03中有多种初始化语法（=, (), {}），规则复杂
 *   且不一致。统一初始化提供一种适用于所有场景的语法，并且
 *   能防止窄化转换(narrowing conversion)，还能解决"最令人困惑
 *   的解析"(most vexing parse)问题。
 *
 * [关键要点]:
 *   - 花括号初始化可用于几乎所有类型
 *   - 防止窄化转换（如double到int、int到char等会丢失精度的转换）
 *   - 解决most vexing parse问题
 *   - 当类有接受initializer_list的构造函数时，花括号初始化优先匹配它
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <initializer_list>

// ============================================================
// 辅助类
// ============================================================
struct Point {
    double x;
    double y;

    void print() const {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};

class Widget {
public:
    Widget() : id_(0), name_("默认") {
        std::cout << "  Widget默认构造: " << name_ << std::endl;
    }
    Widget(int id, const std::string& name) : id_(id), name_(name) {
        std::cout << "  Widget(int, string)构造: " << name_ << std::endl;
    }
    Widget(std::initializer_list<int> values) : id_(0), name_("列表构造") {
        std::cout << "  Widget(initializer_list)构造, 元素: ";
        for (auto v : values) std::cout << v << " ";
        std::cout << std::endl;
    }

    void print() const {
        std::cout << "  Widget[" << id_ << "]: " << name_ << std::endl;
    }

private:
    int id_;
    std::string name_;
};

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. 基本类型的花括号初始化
    int a{42};
    double b{3.14};
    char c{'A'};
    bool d{true};
    std::cout << "a=" << a << ", b=" << b << ", c=" << c << ", d=" << d << std::endl;

    // 2. 数组初始化
    int arr[]{1, 2, 3, 4, 5};
    std::cout << "数组: ";
    for (auto v : arr) std::cout << v << " ";
    std::cout << std::endl;

    // 3. 聚合体(struct)初始化
    Point p1{1.0, 2.0};
    std::cout << "点1: ";
    p1.print();

    // 4. STL容器初始化
    std::vector<int> vec{10, 20, 30, 40, 50};
    std::cout << "向量: ";
    for (auto v : vec) std::cout << v << " ";
    std::cout << std::endl;

    std::map<std::string, int> scores{
        {"语文", 90},
        {"数学", 95},
        {"英语", 88}
    };
    std::cout << "成绩:" << std::endl;
    for (const auto& pair : scores) {
        std::cout << "  " << pair.first << ": " << pair.second << std::endl;
    }

    // 5. 零初始化
    int x{};       // x = 0
    double y{};    // y = 0.0
    int* p{};      // p = nullptr
    std::cout << "零初始化: x=" << x << ", y=" << y
              << ", p=" << (p == nullptr ? "nullptr" : "非空") << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 防止窄化转换 —— 花括号初始化的重要安全特性
    // int narrow1{3.14};   // 编译错误! double到int是窄化转换
    // int narrow2{1000000}; // 如果int是16位，则错误（值超出范围）
    // char narrow3{256};    // 编译错误! 256超出char范围
    int safe1 = 3.14;       // 传统初始化: 编译器只是警告，值被截断为3
    std::cout << "传统初始化允许窄化: int = 3.14 -> " << safe1 << std::endl;
    std::cout << "花括号初始化会阻止窄化转换（编译错误）" << std::endl;

    // 2. 解决 Most Vexing Parse（最令人困惑的解析）
    // C++03的问题：
    // Widget w();  // 这不是创建Widget对象! 这是声明一个函数!
    // C++11解决方案：
    Widget w1{};   // 明确: 创建Widget对象，调用默认构造函数
    Widget w2{1, "测试"}; // 明确: 调用两参数构造函数

    // 3. 返回值使用花括号初始化
    struct Result {
        int code;
        std::string message;
    };

    auto get_result = []() -> Result {
        return {0, "成功"}; // 直接用花括号返回，无需写类型名
    };

    auto r = get_result();
    std::cout << "结果: code=" << r.code << ", msg=" << r.message << std::endl;

    // 4. 函数参数直接使用花括号
    auto print_point = [](Point p) {
        std::cout << "点: ";
        p.print();
    };
    print_point({3.0, 4.0}); // 直接传花括号初始化的临时对象

    // 5. 嵌套花括号初始化
    std::vector<Point> points{{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}};
    std::cout << "点集合:" << std::endl;
    for (const auto& pt : points) {
        std::cout << "  ";
        pt.print();
    }
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: initializer_list构造函数的优先级
    std::cout << "Widget构造函数重载决议:" << std::endl;
    Widget w1(1, "正常");   // 调用 Widget(int, string)
    Widget w2{1, 2};        // 调用 Widget(initializer_list<int>)! 不是Widget(int, string)!
    // 花括号会优先匹配initializer_list版本

    // 陷阱2: vector的经典陷阱
    std::vector<int> v1(5, 10);   // 5个元素，每个值为10: [10,10,10,10,10]
    std::vector<int> v2{5, 10};   // 2个元素，值为5和10: [5, 10]

    std::cout << "\nvector(5,10): ";
    for (auto v : v1) std::cout << v << " ";
    std::cout << std::endl;

    std::cout << "vector{5,10}: ";
    for (auto v : v2) std::cout << v << " ";
    std::cout << std::endl;

    // 陷阱3: auto与花括号初始化
    auto x1 = {1, 2, 3}; // x1是std::initializer_list<int>，不是vector!
    // auto x2{1, 2, 3};  // C++11中是initializer_list, C++17中可能不同
    auto x3{42};           // 在某些编译器中是int，在某些中是initializer_list<int>
    std::cout << "\nauto = {1,2,3} 的类型: " << typeid(x1).name() << std::endl;
    std::cout << "auto{42} 的类型: " << typeid(x3).name() << std::endl;

    // 陷阱4: 空花括号意味着默认初始化而非initializer_list
    Widget w3{};  // 调用默认构造函数，不是initializer_list版本

    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 花括号初始化能防止窄化转换，增加安全性" << std::endl;
    std::cout << "  2. 注意initializer_list构造函数的优先级" << std::endl;
    std::cout << "  3. vector等容器的()和{}行为不同，需特别注意" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: 统一初始化" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
