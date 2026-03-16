/**
 * ============================================================
 * 特性名称: override 与 final 关键字
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: override显式声明函数是覆盖基类虚函数；
 *          final阻止类被继承或虚函数被进一步覆盖。
 *
 * [为什么引入]: C++03中如果派生类函数签名不小心写错（参数类型、
 *   const限定等），编译器会默默创建一个新函数而非覆盖，导致
 *   运行时多态行为不符合预期。override和final在编译期捕获这些错误。
 *
 * [关键要点]:
 *   - override让编译器验证函数确实覆盖了基类虚函数
 *   - 签名不匹配（参数类型、const、返回类型等）会立即报编译错误
 *   - final可用于类（阻止继承）或虚函数（阻止覆盖）
 *   - 它们是上下文关键字(context-sensitive keyword)，不影响已有代码
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>

// ============================================================
// 基类
// ============================================================
class Animal {
public:
    virtual ~Animal() = default;

    virtual std::string name() const { return "动物"; }
    virtual void speak() const { std::cout << "..." << std::endl; }
    virtual int legs() const { return 0; }
    virtual void eat(const std::string& food) {
        std::cout << name() << "吃" << food << std::endl;
    }
};

// ============================================================
// 使用override的派生类
// ============================================================
class Dog : public Animal {
public:
    std::string name() const override { return "狗"; }
    void speak() const override { std::cout << "汪汪!" << std::endl; }
    int legs() const override { return 4; }
    void eat(const std::string& food) override {
        std::cout << "狗开心地吃" << food << std::endl;
    }

    // 以下示例展示override如何捕获错误（已注释掉）：
    // void speak() override { }          // 错误! 缺少const限定符
    // int legs() override { return 4; }  // 错误! 缺少const限定符
    // void eat(std::string food) override {} // 错误! 参数类型不匹配(少了const&)
    // void speek() const override {}     // 错误! 基类没有speek函数(拼写错误)
};

// ============================================================
// 使用final的类和方法
// ============================================================
class Cat : public Animal {
public:
    std::string name() const override { return "猫"; }
    void speak() const override final {  // final: 不允许子类再覆盖speak
        std::cout << "喵~" << std::endl;
    }
    int legs() const override { return 4; }
};

// final类 —— 不能被继承
class PersianCat final : public Cat {
public:
    std::string name() const override { return "波斯猫"; }
    // void speak() const override {} // 编译错误! Cat::speak()是final的
    int legs() const override { return 4; }
};

// class SuperPersianCat : public PersianCat {}; // 编译错误! PersianCat是final的

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. 多态调用
    std::vector<std::unique_ptr<Animal>> zoo;
    zoo.push_back(std::unique_ptr<Animal>(new Animal()));
    zoo.push_back(std::unique_ptr<Animal>(new Dog()));
    zoo.push_back(std::unique_ptr<Animal>(new Cat()));
    zoo.push_back(std::unique_ptr<Animal>(new PersianCat()));

    for (const auto& animal : zoo) {
        std::cout << animal->name() << " (" << animal->legs() << "条腿): ";
        animal->speak();
    }

    // 2. override 确保正确覆盖
    std::cout << "\noverride确保签名完全匹配:" << std::endl;
    Dog dog;
    Animal& ref = dog;
    ref.speak();       // 输出"汪汪!" —— 多态正确工作
    ref.eat("骨头");   // 输出"狗开心地吃骨头"
}

// ============================================================
// 进阶用法
// ============================================================

// 接口类（纯虚函数）
class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void draw() const = 0;          // 纯虚函数
    virtual void resize(double factor) = 0; // 纯虚函数
};

class Circle : public Drawable {
public:
    Circle(double r) : radius_(r) {}
    void draw() const override {
        std::cout << "  绘制圆形，半径=" << radius_ << std::endl;
    }
    void resize(double factor) override {
        radius_ *= factor;
        std::cout << "  圆形缩放后半径=" << radius_ << std::endl;
    }
private:
    double radius_;
};

class Square final : public Drawable {
public:
    Square(double s) : side_(s) {}
    void draw() const override {
        std::cout << "  绘制正方形，边长=" << side_ << std::endl;
    }
    void resize(double factor) override {
        side_ *= factor;
        std::cout << "  正方形缩放后边长=" << side_ << std::endl;
    }
private:
    double side_;
};

// 模板中使用override
template <typename T>
class TypedAnimal : public Animal {
public:
    TypedAnimal(const std::string& n) : name_(n) {}
    std::string name() const override { return name_; }
    void speak() const override {
        std::cout << name_ << "发出声音" << std::endl;
    }
private:
    std::string name_;
};

void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. 接口实现
    std::vector<std::unique_ptr<Drawable>> shapes;
    shapes.push_back(std::unique_ptr<Drawable>(new Circle(5.0)));
    shapes.push_back(std::unique_ptr<Drawable>(new Square(3.0)));

    for (const auto& shape : shapes) {
        shape->draw();
        shape->resize(2.0);
    }

    // 2. 模板派生类中使用override
    std::cout << "\n模板派生类:" << std::endl;
    TypedAnimal<int> tiger("老虎");
    Animal& a = tiger;
    a.speak();

    // 3. final优化 —— 编译器知道不会有更多覆盖，可去虚化(devirtualize)
    std::cout << "\nfinal允许编译器进行去虚化优化" << std::endl;
    PersianCat pc;
    pc.speak(); // 编译器可能直接调用，无需虚表查找
}

// ============================================================
// 易错点与陷阱
// ============================================================

// 演示：没有override时的隐蔽bug
class Base {
public:
    virtual ~Base() = default;
    virtual void process(int value) const {
        std::cout << "  Base::process(int): " << value << std::endl;
    }
};

class DerivedBuggy : public Base {
public:
    // 注意: 参数是double，不是int! 这不是覆盖，而是隐藏!
    // 如果加了override就能发现这个错误
    virtual void process(double value) const {
        std::cout << "  DerivedBuggy::process(double): " << value << std::endl;
    }
};

class DerivedCorrect : public Base {
public:
    void process(int value) const override { // override确保签名正确
        std::cout << "  DerivedCorrect::process(int): " << value << std::endl;
    }
};

void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: 不用override时的隐蔽bug
    std::cout << "陷阱1: 没有override时的隐蔽bug" << std::endl;
    DerivedBuggy buggy;
    Base& ref_buggy = buggy;
    ref_buggy.process(42);  // 调用Base::process(int)! 不是DerivedBuggy的版本!
    std::cout << "  (期望调用派生类版本，实际调用了基类版本!)" << std::endl;

    DerivedCorrect correct;
    Base& ref_correct = correct;
    ref_correct.process(42); // 正确调用DerivedCorrect::process(int)

    // 陷阱2: override和final是上下文关键字
    // 它们只在特定位置有特殊含义，可以作为变量名（但不推荐）
    int override = 10;    // 合法但极不推荐!
    int final = 20;       // 合法但极不推荐!
    std::cout << "\n陷阱2: override和final可以作为变量名(极不推荐): "
              << override << ", " << final << std::endl;

    // 陷阱3: final不能用于非虚函数
    // class Foo {
    //     void bar() final; // 编译错误! bar不是虚函数
    // };
    std::cout << "\n陷阱3: final只能用于虚函数" << std::endl;

    // 陷阱4: 析构函数也是虚函数（可以override）
    // 基类析构函数应该是virtual的，否则通过基类指针删除派生类对象是未定义行为
    std::cout << "陷阱4: 别忘了让基类析构函数为virtual" << std::endl;

    std::cout << "\n最佳实践:" << std::endl;
    std::cout << "  1. 所有覆盖的虚函数都加上override" << std::endl;
    std::cout << "  2. 不需要进一步派生的类用final标记" << std::endl;
    std::cout << "  3. 基类析构函数必须是virtual的" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: override 与 final" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
