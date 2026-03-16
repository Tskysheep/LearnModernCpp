/**
 * ============================================================
 * 特性名称: std::make_unique
 * 所属标准: C++14
 * ============================================================
 *
 * [是什么]: std::make_unique是创建std::unique_ptr的工厂函数，
 *          类似于C++11中已有的std::make_shared。它封装了new操作，
 *          直接构造并返回一个unique_ptr对象。
 *
 * [为什么引入]: C++11引入了unique_ptr和make_shared，但遗漏了make_unique。
 *              直接使用new存在异常安全问题（参数求值顺序未定义时可能内存泄漏），
 *              而且new/delete的显式使用不符合现代C++的资源管理理念。
 *              make_unique填补了这个空白，使"零new"编程成为可能。
 *
 * [关键要点]:
 *   - make_unique<T>(args...) 创建单个对象的unique_ptr
 *   - make_unique<T[]>(n) 创建数组类型的unique_ptr
 *   - 比直接new更安全（异常安全）
 *   - 避免代码中出现裸new，符合现代C++最佳实践
 * ============================================================
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <utility>

// 辅助类
class Widget {
public:
    Widget(int id, const std::string& name)
        : id_(id), name_(name) {
        std::cout << "    Widget(" << id_ << ", \"" << name_ << "\") 构造\n";
    }

    ~Widget() {
        std::cout << "    Widget(" << id_ << ", \"" << name_ << "\") 析构\n";
    }

    void display() const {
        std::cout << "    Widget[id=" << id_ << ", name=\"" << name_ << "\"]\n";
    }

    int id() const { return id_; }

private:
    int id_;
    std::string name_;
};

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 创建单个对象
    std::cout << "  [创建单个对象]\n";
    auto p1 = std::make_unique<Widget>(1, "Alpha");
    p1->display();

    // 2. 创建基本类型
    auto p2 = std::make_unique<int>(42);
    std::cout << "  make_unique<int>(42) = " << *p2 << "\n";

    auto p3 = std::make_unique<std::string>("Hello C++14");
    std::cout << "  make_unique<string> = \"" << *p3 << "\"\n";

    // 3. 创建数组
    std::cout << "\n  [创建数组]\n";
    auto arr = std::make_unique<int[]>(5);  // 5个int，值初始化为0
    for (int i = 0; i < 5; ++i) {
        arr[i] = (i + 1) * 10;
    }
    std::cout << "  数组内容: ";
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";

    // 4. 与手动new的对比
    std::cout << "\n  [与new对比]\n";

    // 旧写法（不推荐）
    std::unique_ptr<Widget> old_way(new Widget(2, "OldWay"));

    // 新写法（推荐）
    auto new_way = std::make_unique<Widget>(3, "NewWay");

    std::cout << "  两种方式功能等价，但make_unique更安全更简洁\n";
    std::cout << "\n  --- 作用域结束，自动析构 ---\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 异常安全问题演示
int might_throw() {
    // 模拟可能抛出异常的函数
    return 42;
}

void process(std::unique_ptr<Widget> w, int value) {
    std::cout << "  处理Widget " << w->id() << " with value=" << value << "\n";
}

// 使用unique_ptr的工厂模式
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}
    void draw() const override {
        std::cout << "    绘制圆形 (半径=" << radius_ << ")\n";
    }
    std::string name() const override { return "Circle"; }
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    void draw() const override {
        std::cout << "    绘制矩形 (" << w_ << "x" << h_ << ")\n";
    }
    std::string name() const override { return "Rectangle"; }
};

// 工厂函数返回unique_ptr
std::unique_ptr<Shape> create_shape(const std::string& type) {
    if (type == "circle")
        return std::make_unique<Circle>(5.0);
    else if (type == "rectangle")
        return std::make_unique<Rectangle>(3.0, 4.0);
    return nullptr;
}

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. 异常安全问题
    std::cout << "  [异常安全]\n";
    // 危险的写法（C++17之前参数求值顺序未定义）：
    // process(std::unique_ptr<Widget>(new Widget(1, "Unsafe")), might_throw());
    // 如果new成功但might_throw()在unique_ptr构造前抛出异常 -> 内存泄漏！

    // 安全的写法：
    process(std::make_unique<Widget>(1, "Safe"), might_throw());
    // make_unique保证：要么完全成功，要么不分配内存

    // 2. 工厂模式
    std::cout << "\n  [工厂模式]\n";
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(create_shape("circle"));
    shapes.push_back(create_shape("rectangle"));
    shapes.push_back(create_shape("circle"));

    for (const auto& shape : shapes) {
        if (shape) {
            std::cout << "  " << shape->name() << " -> ";
            shape->draw();
        }
    }

    // 3. 移动语义与make_unique
    std::cout << "\n  [移动语义]\n";
    auto w1 = std::make_unique<Widget>(10, "Movable");
    std::cout << "  w1 有效? " << (w1 ? "是" : "否") << "\n";

    auto w2 = std::move(w1);  // 移动所有权
    std::cout << "  移动后:\n";
    std::cout << "  w1 有效? " << (w1 ? "是" : "否") << "\n";
    std::cout << "  w2 有效? " << (w2 ? "是" : "否") << "\n";
    if (w2) w2->display();

    // 4. 在容器中使用
    std::cout << "\n  [容器中使用]\n";
    std::vector<std::unique_ptr<Widget>> widgets;
    for (int i = 0; i < 3; ++i) {
        widgets.push_back(std::make_unique<Widget>(i, "Item" + std::to_string(i)));
    }
    std::cout << "  容器包含 " << widgets.size() << " 个Widget\n";
    std::cout << "  --- 清空容器 ---\n";
    widgets.clear();  // 自动析构所有Widget
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱1：make_unique不支持自定义删除器
    std::cout << "  陷阱1: make_unique不支持自定义删除器\n";
    // 需要自定义删除器时，只能用构造函数
    auto custom_deleter = [](Widget* w) {
        std::cout << "    [自定义删除器] ";
        delete w;
    };
    std::unique_ptr<Widget, decltype(custom_deleter)>
        p(new Widget(99, "CustomDel"), custom_deleter);
    std::cout << "  需要自定义删除器时，只能用 unique_ptr(new T, deleter)\n";

    // 陷阱2：make_unique<T[]>不支持初始化列表
    std::cout << "\n  陷阱2: make_unique<T[]>(n) 只能值初始化（归零）\n";
    auto arr = std::make_unique<int[]>(5);
    std::cout << "    make_unique<int[]>(5) 初始值: ";
    for (int i = 0; i < 5; ++i) std::cout << arr[i] << " ";
    std::cout << "(全是0)\n";
    // 不能写 make_unique<int[]>({1,2,3,4,5})

    // 陷阱3：不要用make_unique创建已有对象的指针
    std::cout << "\n  陷阱3: make_unique总是创建新对象，不要用于管理已有指针\n";
    std::cout << "    int* raw = new int(42);\n";
    std::cout << "    auto up = std::make_unique<int>(42); // 创建新对象，不接管raw\n";
    std::cout << "    // 要接管已有指针，用: unique_ptr<int>(raw)\n";

    // 陷阱4：make_unique使用()初始化，不是{}
    std::cout << "\n  陷阱4: make_unique使用圆括号初始化\n";
    // 对于聚合类型，C++14的make_unique不支持列表初始化
    // struct Agg { int x; double y; };
    // auto p = std::make_unique<Agg>(1, 2.0);  // C++14可能编译失败（无构造函数）
    // C++20的make_unique才支持聚合初始化
    std::cout << "    make_unique<T>(args...) 使用T(args...)形式初始化\n";
    std::cout << "    对于没有构造函数的聚合类型，C++14中可能不支持\n";

    std::cout << "\n  --- 析构演示 ---\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::make_unique - C++14 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
