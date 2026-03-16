/**
 * ============================================================
 * 特性名称: std::initializer_list
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: std::initializer_list<T> 是一个轻量级代理对象，
 *          用于访问花括号 {} 初始化列表中的元素。
 *
 * [为什么引入]: C++03 中没有标准方法支持 vector<int> v = {1,2,3}
 *   这样的语法。initializer_list 使容器和自定义类型可以接受
 *   花括号初始化列表作为参数。
 *
 * [关键要点]:
 *   - initializer_list 是一个轻量级视图（指针+大小），不拥有数据
 *   - 底层数据是 const 数组，元素不可修改
 *   - 支持 begin()/end()/size()
 *   - 花括号初始化时，initializer_list 构造函数优先级最高
 * ============================================================
 */

#include <iostream>
#include <initializer_list>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cstddef>

// ---- 1. 基本用法 ----

// 接受 initializer_list 的函数
void print_values(std::initializer_list<int> values) {
    std::cout << "  [ ";
    for (auto v : values) {
        std::cout << v << " ";
    }
    std::cout << "] (共 " << values.size() << " 个元素)\n";
}

// 自定义类接受 initializer_list
class IntArray {
public:
    IntArray(std::initializer_list<int> init)
        : data_(init.begin(), init.end()) {
        std::cout << "  IntArray 从 initializer_list 构造, 大小="
                  << data_.size() << "\n";
    }

    void print() const {
        std::cout << "  IntArray: [ ";
        for (int v : data_) std::cout << v << " ";
        std::cout << "]\n";
    }

    int sum() const {
        return std::accumulate(data_.begin(), data_.end(), 0);
    }

private:
    std::vector<int> data_;
};

void basic_usage() {
    std::cout << "---- 1. 基本用法 ----\n";

    // 函数参数
    print_values({1, 2, 3, 4, 5});
    print_values({10, 20});
    print_values({}); // 空列表

    // 自定义类
    IntArray arr = {10, 20, 30, 40, 50};
    arr.print();
    std::cout << "  总和: " << arr.sum() << "\n";

    // 标准库容器
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::cout << "\n  vector<int> 用花括号初始化, 大小=" << v.size() << "\n";

    std::vector<std::string> names = {"张三", "李四", "王五"};
    std::cout << "  vector<string> 用花括号初始化, 大小=" << names.size() << "\n";

    // auto 推导
    auto il = {1, 2, 3}; // 推导为 std::initializer_list<int>
    std::cout << "\n  auto il = {1,2,3} 类型大小: " << il.size() << "\n";
}

// ---- 2. 进阶用法 ----

// initializer_list 用于实现可变参数的类型安全替代方案
int max_of(std::initializer_list<int> values) {
    return *std::max_element(values.begin(), values.end());
}

int min_of(std::initializer_list<int> values) {
    return *std::min_element(values.begin(), values.end());
}

// 配置类 —— 使用 initializer_list 设置多个选项
class Config {
public:
    struct Option {
        std::string key;
        std::string value;
    };

    Config(std::initializer_list<Option> options) {
        for (const auto& opt : options) {
            options_.push_back(opt);
        }
    }

    void print() const {
        std::cout << "  配置项:\n";
        for (const auto& opt : options_) {
            std::cout << "    " << opt.key << " = " << opt.value << "\n";
        }
    }

private:
    std::vector<Option> options_;
};

// 赋值运算符也可以使用 initializer_list
class DynamicArray {
public:
    DynamicArray() = default;

    DynamicArray(std::initializer_list<double> init)
        : data_(init) {}

    DynamicArray& operator=(std::initializer_list<double> init) {
        data_.assign(init.begin(), init.end());
        return *this;
    }

    void print() const {
        std::cout << "  [ ";
        for (double v : data_) std::cout << v << " ";
        std::cout << "]\n";
    }

private:
    std::vector<double> data_;
};

void advanced_usage() {
    std::cout << "\n---- 2. 进阶用法 ----\n";

    // 类型安全的可变参数
    std::cout << "max_of({3, 7, 2, 9, 1}): " << max_of({3, 7, 2, 9, 1}) << "\n";
    std::cout << "min_of({3, 7, 2, 9, 1}): " << min_of({3, 7, 2, 9, 1}) << "\n";

    // 复合类型的 initializer_list
    std::cout << "\n结构体的 initializer_list:\n";
    Config cfg = {
        {"host", "localhost"},
        {"port", "8080"},
        {"debug", "true"}
    };
    cfg.print();

    // 赋值操作符中使用
    std::cout << "\n赋值中使用 initializer_list:\n";
    DynamicArray da;
    da = {1.1, 2.2, 3.3};
    da.print();
    da = {10.0, 20.0};
    da.print();

    // 遍历 initializer_list
    std::cout << "\n直接遍历花括号列表:\n";
    for (auto x : {10, 20, 30, 40}) {
        std::cout << "  " << x;
    }
    std::cout << "\n";
}

// ---- 3. 常见陷阱与最佳实践 ----

class Ambiguous {
public:
    // 构造函数1: 两个 int 参数
    Ambiguous(int a, int b) {
        std::cout << "  Ambiguous(int, int): a=" << a << ", b=" << b << "\n";
    }

    // 构造函数2: initializer_list
    Ambiguous(std::initializer_list<int> il) {
        std::cout << "  Ambiguous(initializer_list): 大小=" << il.size() << "\n";
    }
};

void pitfalls() {
    std::cout << "\n---- 3. 常见陷阱与最佳实践 ----\n";

    // 陷阱1: initializer_list 构造函数优先级更高
    std::cout << "陷阱1: initializer_list 构造函数优先!\n";
    Ambiguous a1(1, 2);    // 调用 (int, int)
    Ambiguous a2{1, 2};    // 调用 initializer_list! 不是 (int, int)!
    Ambiguous a3({1, 2});  // 调用 initializer_list

    // vector 的经典陷阱
    std::cout << "\nvector 的经典例子:\n";
    std::vector<int> v1(5, 1);   // 5 个 1: [1, 1, 1, 1, 1]
    std::vector<int> v2{5, 1};   // 2 个元素: [5, 1]
    std::cout << "  vector<int>(5, 1) 大小: " << v1.size() << " (5个1)\n";
    std::cout << "  vector<int>{5, 1} 大小: " << v2.size() << " (元素5和1)\n";

    // 陷阱2: initializer_list 的元素是 const 的
    std::cout << "\n陷阱2: 元素不可修改\n";
    auto il = {1, 2, 3};
    // *il.begin() = 10; // 编译错误! 元素是 const int
    std::cout << "  initializer_list 的元素类型是 const T\n";

    // 陷阱3: initializer_list 不拥有数据
    std::cout << "\n陷阱3: initializer_list 只是视图\n";
    std::cout << "  底层数据是临时 const 数组\n";
    std::cout << "  不要在 initializer_list 超出作用域后访问其元素\n";

    // 陷阱4: 窄化转换在 {} 中是被禁止的
    std::cout << "\n陷阱4: {} 禁止窄化转换\n";
    // std::initializer_list<int> bad = {1.5, 2.7}; // 编译错误! double->int 窄化
    std::cout << "  {1.5, 2.7} 不能初始化 initializer_list<int>\n";

    // 最佳实践
    std::cout << "\n最佳实践:\n";
    std::cout << "  1. 对容器类型，提供 initializer_list 构造函数\n";
    std::cout << "  2. 注意 () 和 {} 初始化的区别\n";
    std::cout << "  3. initializer_list 按值传递（它本身很轻量）\n";
    std::cout << "  4. 不要从函数返回局部数组的 initializer_list\n";
}

int main() {
    std::cout << "===== std::initializer_list - C++11 =====\n\n";
    basic_usage();
    advanced_usage();
    pitfalls();
    return 0;
}
