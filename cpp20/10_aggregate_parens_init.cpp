/**
 * ============================================================
 * 特性名称: 圆括号聚合初始化 (Parenthesized Aggregate Initialization)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: C++20 允许使用圆括号 () 对聚合体进行初始化，
 *          之前只能使用花括号 {} 进行聚合初始化。
 *
 * [为什么引入]: 解决 make_shared/emplace_back 等函数
 *             无法直接构造聚合体的问题，因为这些函数
 *             内部使用圆括号进行转发构造。
 *
 * [关键要点]:
 *   - 聚合体可以用 () 初始化，使 make_shared/emplace 等函数可用
 *   - 与花括号初始化的区别：圆括号不检查窄化转换
 *   - 圆括号初始化不支持指定初始化器
 *   - 未提供的成员会被值初始化
 * ============================================================
 */

#include <iostream>
#include <memory>
#include <vector>
#include <array>
#include <string>
#include <utility>

// ===================== basic_usage() =====================

// 简单聚合体
struct Point {
    int x;
    int y;
};

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a = 255;
};

struct Config {
    std::string name;
    int width;
    int height;
};

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // C++17: 只能用花括号初始化聚合体
    Point p1{10, 20};
    std::cout << "花括号初始化: Point{10, 20} = (" << p1.x << ", " << p1.y << ")\n";

    // C++20: 可以用圆括号初始化聚合体
    Point p2(30, 40);
    std::cout << "圆括号初始化: Point(30, 40) = (" << p2.x << ", " << p2.y << ")\n";

    // 关键应用：make_shared 现在可以直接构造聚合体！
    // C++17: std::make_shared<Point>(10, 20);  // 编译错误！
    // C++20: 合法!
    auto ptr = std::make_shared<Point>(10, 20);
    std::cout << "\nmake_shared<Point>(10, 20): (" << ptr->x << ", " << ptr->y << ")\n";

    auto ptr2 = std::make_shared<Config>("窗口", 800, 600);
    std::cout << "make_shared<Config>(\"窗口\", 800, 600): "
              << ptr2->name << " " << ptr2->width << "x" << ptr2->height << "\n";

    // emplace_back 也可以了
    std::vector<Point> points;
    points.emplace_back(1, 2);   // C++17 编译错误, C++20 合法
    points.emplace_back(3, 4);
    points.emplace_back(5, 6);

    std::cout << "\nemplace_back 构造的点: ";
    for (const auto& p : points) {
        std::cout << "(" << p.x << "," << p.y << ") ";
    }
    std::cout << "\n";
}

// ===================== advanced_usage() =====================

// 带数组成员的聚合体
struct Matrix {
    int data[4];
};

// 继承的聚合体
struct Base {
    int id;
};

struct Derived : Base {
    std::string name;
};

// 嵌套聚合体
struct Rect {
    Point origin;
    Point size;
};

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // make_unique 也可以用了
    auto up = std::make_unique<Color>(255, 128, 0);
    std::cout << "make_unique<Color>(255, 128, 0): "
              << "r=" << (int)up->r << " g=" << (int)up->g
              << " b=" << (int)up->b << " a=" << (int)up->a << "\n";

    // 在 optional 中使用
    // std::optional<Point> opt(std::in_place, 10, 20);  // C++20 合法

    // emplace 系列函数
    std::vector<Config> configs;
    configs.emplace_back("主窗口", 1920, 1080);
    configs.emplace_back("副窗口", 800, 600);

    std::cout << "\n配置列表:\n";
    for (const auto& cfg : configs) {
        std::cout << "  " << cfg.name << ": " << cfg.width << "x" << cfg.height << "\n";
    }

    // 花括号 vs 圆括号的区别
    std::cout << "\n-- 花括号 vs 圆括号 --\n";

    // 花括号检查窄化转换
    // Color c1{256, 0, 0};  // 可能的窄化警告/错误 (256 超出 unsigned char 范围)
    // 圆括号不检查窄化转换
    Color c2(256, 0, 0);     // 合法（但值会被截断）
    std::cout << "圆括号 Color(256, 0, 0): r=" << (int)c2.r
              << " (发生截断，256→" << (int)c2.r << ")\n";

    // 数组成员 —— 注意：包含数组的聚合体不能用圆括号初始化
    Matrix m{1, 2, 3, 4};  // 花括号仍可用
    std::cout << "\nMatrix{1,2,3,4}: ";
    for (int v : m.data) std::cout << v << " ";
    std::cout << "\n";

    // unique_ptr 数组
    auto arr_ptr = std::make_unique<Point[]>(3);
    // 注意：make_unique 数组版本不支持参数初始化
    std::cout << "\nmake_unique<Point[]>(3): 数组已创建\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：圆括号不检查窄化转换
    std::cout << "1. 圆括号初始化不检查窄化转换:\n";
    std::cout << "   Point p{3.14, 2.71};  // 花括号: 窄化错误!\n";
    std::cout << "   Point p(3.14, 2.71);  // 圆括号: 合法，但截断为 (3, 2)\n";
    Point p_narrow(3.14, 2.71);
    std::cout << "   实际结果: (" << p_narrow.x << ", " << p_narrow.y << ")\n\n";

    // 陷阱2：圆括号不能使用指定初始化器
    std::cout << "2. 圆括号不支持指定初始化器:\n";
    std::cout << "   Point p{.x=1, .y=2};  // 花括号: 合法\n";
    std::cout << "   Point p(.x=1, .y=2);  // 圆括号: 编译错误!\n\n";

    // 陷阱3：与最令人困扰的解析（most vexing parse）的关系
    std::cout << "3. 注意 most vexing parse:\n";
    std::cout << "   Point p();   // 这是函数声明，不是变量定义!\n";
    std::cout << "   Point p{};   // 这是变量定义（零初始化）\n";
    std::cout << "   Point p(1, 2); // 这是变量定义（C++20 聚合初始化）\n\n";

    // 陷阱4：未提供的成员
    std::cout << "4. 未提供的尾部成员会被值初始化（零初始化或默认构造）:\n";
    Color partial(128);  // g=0, b=0, a=255（使用默认值）
    std::cout << "   Color(128): r=" << (int)partial.r
              << " g=" << (int)partial.g
              << " b=" << (int)partial.b
              << " a=" << (int)partial.a << "\n\n";

    // 陷阱5：只适用于聚合体
    std::cout << "5. 只有聚合体才能使用圆括号聚合初始化:\n";
    std::cout << "   有用户定义构造函数的类型用圆括号是调用构造函数\n";
    std::cout << "   （这不是新特性，一直就是这样）\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== 圆括号聚合初始化 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
