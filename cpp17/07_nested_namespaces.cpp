/**
 * ============================================================
 * 特性名称: 嵌套命名空间 (Nested Namespaces)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: 允许用 namespace A::B::C { } 的简洁语法定义多层
 *          嵌套的命名空间，而无需逐层嵌套花括号。
 *
 * [为什么引入]: C++11/14 中定义深层命名空间需要多层缩进和花括号，
 *              代码冗长且容易出错。C++17 语法一行搞定。
 *
 * [关键要点]:
 *   - namespace A::B::C { } 等价于 namespace A { namespace B { namespace C { } } }
 *   - 每层命名空间如果不存在会自动创建
 *   - 可以在不同地方分别扩展同一个嵌套命名空间
 *   - 极大减少了深层命名空间定义时的缩进层级
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>

// ============================================================
// 基础用法
// ============================================================

// C++11/14 写法：层层嵌套
namespace OldStyle {
    namespace Graphics {
        namespace Shapes {
            struct Circle {
                double radius;
                void draw() const {
                    std::cout << "C++11 写法: 绘制圆形 r=" << radius << "\n";
                }
            };
        }
    }
}

// C++17 写法：一行搞定
namespace NewStyle::Graphics::Shapes {
    struct Circle {
        double radius;
        void draw() const {
            std::cout << "C++17 写法: 绘制圆形 r=" << radius << "\n";
        }
    };
}

void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 使用旧式嵌套命名空间
    OldStyle::Graphics::Shapes::Circle c1{5.0};
    c1.draw();

    // 使用新式嵌套命名空间
    NewStyle::Graphics::Shapes::Circle c2{10.0};
    c2.draw();

    // 使用 using 简化
    using namespace NewStyle::Graphics::Shapes;
    Circle c3{7.5};
    c3.draw();
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 在不同地方扩展同一命名空间
namespace Company::Product::Core {
    int version = 1;

    std::string get_name() {
        return "核心模块";
    }
}

// 在另一个地方继续添加
namespace Company::Product::Core {
    void print_info() {
        std::cout << "产品: " << get_name()
                  << " v" << version << "\n";
    }
}

// 2. 多个平行的子命名空间
namespace Company::Product::UI {
    void render() {
        std::cout << "渲染 UI 界面\n";
    }
}

namespace Company::Product::Network {
    void connect() {
        std::cout << "建立网络连接\n";
    }
}

// 3. 深层嵌套的实际应用
namespace MyLib::Math::Linear::Matrix {
    struct Mat2x2 {
        double data[2][2];

        void print() const {
            std::cout << "矩阵:\n";
            for (int i = 0; i < 2; ++i) {
                std::cout << "  [";
                for (int j = 0; j < 2; ++j) {
                    std::cout << data[i][j];
                    if (j < 1) std::cout << ", ";
                }
                std::cout << "]\n";
            }
        }
    };

    Mat2x2 identity() {
        return {{{1.0, 0.0}, {0.0, 1.0}}};
    }
}

// 4. 命名空间别名配合使用
namespace MLM = MyLib::Math::Linear::Matrix;

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 扩展的命名空间
    Company::Product::Core::print_info();

    // 平行子命名空间
    Company::Product::UI::render();
    Company::Product::Network::connect();

    // 深层嵌套 + 别名
    auto mat = MLM::identity();
    mat.print();

    // 直接使用完整路径
    MyLib::Math::Linear::Matrix::Mat2x2 m{{{1, 2}, {3, 4}}};
    m.print();
}

// ============================================================
// 对比展示
// ============================================================

// C++11 写法 —— 看看缩进有多深
namespace A {
    namespace B {
        namespace C {
            namespace D {
                void deep_func() {
                    std::cout << "C++11: 四层嵌套，缩进很深\n";
                }
            }
        }
    }
}

// C++17 写法 —— 清爽
namespace A::B::C::D {
    void deep_func_v2() {
        std::cout << "C++17: 一行定义四层，代码扁平\n";
    }
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 对比调用
    A::B::C::D::deep_func();
    A::B::C::D::deep_func_v2();

    // 陷阱 1: 不能在嵌套声明中使用 inline namespace
    //   namespace A::inline B::C { }  // C++17 不支持！
    //   C++20 才支持 namespace A::inline B::C { }
    std::cout << "陷阱1 - C++17 不支持 namespace A::inline B::C {}\n";
    std::cout << "        C++20 才支持嵌套中的 inline namespace\n";

    // 陷阱 2: 嵌套声明中每一层都是独立的命名空间
    //   namespace A::B::C {} 会创建 A、A::B 和 A::B::C 三个命名空间
    std::cout << "陷阱2 - namespace A::B::C 会创建三层命名空间\n";

    // 陷阱 3: using namespace 只导入指定层级
    {
        using namespace Company::Product;
        // 可以直接用 Core::version, UI::render()
        // 但不能直接用 version（它在 Core 里）
        std::cout << "陷阱3 - using namespace Company::Product 后，"
                  << "仍需 Core::version 来访问: "
                  << Core::version << "\n";
    }

    // 陷阱 4: 不能用这种语法「重新打开」一个不存在的中间层
    //   如果 X 不存在，namespace X::Y::Z {} 会自动创建所有层
    //   这不是错误，但要注意拼写
    std::cout << "陷阱4 - 嵌套声明会自动创建所有中间层，注意拼写\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 嵌套命名空间 - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
