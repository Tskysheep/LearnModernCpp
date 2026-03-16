/**
 * ============================================================
 * 特性名称: std::function 与 std::bind
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: std::function 是通用的可调用对象包装器，可以存储函数指针、lambda、
 *           成员函数指针、函数对象等任何可调用目标。std::bind 将可调用对象与
 *           其参数绑定，生成新的可调用对象。
 *
 * [为什么引入]: C++03中回调机制依赖函数指针，无法统一存储不同类型的可调用对象。
 *               std::function提供了类型擦除的统一接口，std::bind提供了参数绑定
 *               和适配功能。
 *
 * [关键要点]:
 *   - std::function<ReturnType(ArgTypes...)> 可以包装任何签名匹配的可调用对象
 *   - std::bind 可以绑定部分参数，使用占位符 _1, _2... 表示未绑定参数
 *   - bind 可以绑定成员函数（第一个参数为对象指针或引用）
 *   - std::function 有少量运行时开销（类型擦除）
 *   - C++11后 lambda 在很多场景下可以替代 bind
 * ============================================================
 */

#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>

// ============================================================
// 辅助定义
// ============================================================

// 普通函数
int add(int a, int b) {
    return a + b;
}

double multiply(double a, double b) {
    return a * b;
}

void greet(const std::string& name) {
    std::cout << "  你好, " << name << "!" << std::endl;
}

// 函数对象（仿函数）
struct Adder {
    int operator()(int a, int b) const {
        return a + b;
    }
};

// 包含成员函数的类
class Calculator {
public:
    int value_;
    Calculator(int v = 0) : value_(v) {}

    int add(int x) {
        value_ += x;
        return value_;
    }

    int subtract(int x) {
        value_ -= x;
        return value_;
    }

    static int staticMultiply(int a, int b) {
        return a * b;
    }

    void display() const {
        std::cout << "  Calculator值: " << value_ << std::endl;
    }
};

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. std::function 存储各种可调用对象
    std::cout << "[1] std::function 存储各种可调用对象:\n";
    {
        // 存储普通函数
        std::function<int(int, int)> fn1 = add;
        std::cout << "  函数指针: add(3, 4) = " << fn1(3, 4) << std::endl;

        // 存储 lambda
        std::function<int(int, int)> fn2 = [](int a, int b) { return a * b; };
        std::cout << "  lambda: 3 * 4 = " << fn2(3, 4) << std::endl;

        // 存储函数对象
        std::function<int(int, int)> fn3 = Adder();
        std::cout << "  函数对象: Adder(3, 4) = " << fn3(3, 4) << std::endl;

        // 存储静态成员函数
        std::function<int(int, int)> fn4 = Calculator::staticMultiply;
        std::cout << "  静态成员函数: 3 * 4 = " << fn4(3, 4) << std::endl;
    }
    std::cout << std::endl;

    // 2. std::function 存储成员函数
    std::cout << "[2] std::function 存储成员函数:\n";
    {
        Calculator calc(10);

        // 成员函数需要对象实例作为第一个参数
        std::function<int(Calculator&, int)> fn = &Calculator::add;
        int result = fn(calc, 5);
        std::cout << "  成员函数 calc.add(5): " << result << std::endl;
    }
    std::cout << std::endl;

    // 3. std::bind 基本用法
    std::cout << "[3] std::bind 绑定参数:\n";
    {
        using namespace std::placeholders;  // _1, _2, ...

        // 绑定第一个参数为10，第二个参数由调用时提供
        auto add10 = std::bind(add, 10, _1);
        std::cout << "  add10(5) = " << add10(5) << std::endl;

        // 绑定所有参数
        auto add_3_4 = std::bind(add, 3, 4);
        std::cout << "  add_3_4() = " << add_3_4() << std::endl;

        // 交换参数顺序
        auto reversed = std::bind(add, _2, _1);
        std::cout << "  reversed(100, 1) = " << reversed(100, 1) << std::endl;
    }
    std::cout << std::endl;

    // 4. 占位符
    std::cout << "[4] 占位符 _1, _2, ...:\n";
    {
        using namespace std::placeholders;

        auto fn = std::bind(multiply, _2, _1);
        // fn(a, b) 实际调用 multiply(b, a)
        std::cout << "  multiply 交换参数: fn(2.0, 3.0) = " << fn(2.0, 3.0) << std::endl;

        // 部分绑定
        auto double_it = std::bind(multiply, _1, 2.0);
        std::cout << "  double_it(5.0) = " << double_it(5.0) << std::endl;
    }
    std::cout << std::endl;

    // 5. function 的空检查
    std::cout << "[5] function 的空检查:\n";
    {
        std::function<void()> fn;  // 空的 function
        std::cout << "  fn 是否为空: " << (!fn ? "是" : "否") << std::endl;

        fn = []() { std::cout << "  现在有值了！" << std::endl; };
        std::cout << "  赋值后是否为空: " << (!fn ? "是" : "否") << std::endl;
        fn();
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. bind 绑定成员函数
    std::cout << "[1] bind 绑定成员函数:\n";
    {
        using namespace std::placeholders;

        Calculator calc(100);

        // 绑定成员函数和对象
        auto addToCalc = std::bind(&Calculator::add, &calc, _1);
        std::cout << "  calc.add(20) = " << addToCalc(20) << std::endl;
        std::cout << "  calc.add(30) = " << addToCalc(30) << std::endl;

        // 绑定显示函数
        auto showCalc = std::bind(&Calculator::display, &calc);
        showCalc();
    }
    std::cout << std::endl;

    // 2. function 用作回调
    std::cout << "[2] function 用作回调机制:\n";
    {
        // 按钮模拟：注册回调
        class Button {
        public:
            std::function<void()> onClick;

            void click() {
                if (onClick) {
                    std::cout << "  按钮被点击，执行回调:" << std::endl;
                    onClick();
                }
            }
        };

        Button btn;

        // 注册 lambda 回调
        btn.onClick = []() {
            std::cout << "    处理点击事件！" << std::endl;
        };
        btn.click();

        // 更换回调
        int count = 0;
        btn.onClick = [&count]() {
            ++count;
            std::cout << "    点击次数: " << count << std::endl;
        };
        btn.click();
        btn.click();
    }
    std::cout << std::endl;

    // 3. function 容器（策略模式）
    std::cout << "[3] function 容器实现策略模式:\n";
    {
        using Strategy = std::function<int(int, int)>;

        std::vector<std::pair<std::string, Strategy>> strategies = {
            {"加法", [](int a, int b) { return a + b; }},
            {"减法", [](int a, int b) { return a - b; }},
            {"乘法", [](int a, int b) { return a * b; }},
            {"最大值", [](int a, int b) { return std::max(a, b); }}
        };

        int x = 10, y = 3;
        for (const auto& s : strategies) {
            std::cout << "  " << s.first << "(" << x << ", " << y << ") = "
                      << s.second(x, y) << std::endl;
        }
    }
    std::cout << std::endl;

    // 4. bind 嵌套和组合
    std::cout << "[4] bind 嵌套和组合:\n";
    {
        using namespace std::placeholders;

        // 创建一个判断是否在范围内的函数
        auto between = [](int val, int low, int high) {
            return val >= low && val <= high;
        };

        // 绑定范围 [10, 50]
        auto in_range = std::bind(between, _1, 10, 50);

        std::vector<int> nums = {5, 15, 25, 55, 35, 3, 45};
        std::cout << "  范围[10,50]内的数: ";
        for (int n : nums) {
            if (in_range(n)) std::cout << n << " ";
        }
        std::cout << std::endl;

        // 用 bind 和 STL 算法
        int count = std::count_if(nums.begin(), nums.end(), in_range);
        std::cout << "  范围内元素个数: " << count << std::endl;
    }
    std::cout << std::endl;

    // 5. bind 与 function 配合用于事件系统
    std::cout << "[5] 简易事件系统:\n";
    {
        using EventHandler = std::function<void(const std::string&)>;
        std::vector<EventHandler> handlers;

        // 注册多个处理器
        handlers.push_back([](const std::string& msg) {
            std::cout << "    处理器A: " << msg << std::endl;
        });

        handlers.push_back([](const std::string& msg) {
            std::cout << "    处理器B: [" << msg << "]" << std::endl;
        });

        // 触发事件
        std::string event = "用户登录";
        std::cout << "  触发事件: " << event << std::endl;
        for (const auto& handler : handlers) {
            handler(event);
        }
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. 调用空的 function 会抛异常
    std::cout << "[1] 调用空的 function 抛出 std::bad_function_call:\n";
    {
        std::function<void()> fn;
        try {
            fn();  // 空的 function！
        } catch (const std::bad_function_call& e) {
            std::cout << "  捕获异常: " << e.what() << std::endl;
        }
        std::cout << "  调用前应检查: if (fn) fn();\n";
    }
    std::cout << std::endl;

    // 2. bind 默认按值捕获
    std::cout << "[2] bind 默认按值捕获参数:\n";
    {
        using namespace std::placeholders;
        int x = 10;
        auto fn = std::bind(add, x, _1);  // x 是副本
        x = 100;
        std::cout << "  x改为100后, fn(5) = " << fn(5)
                  << " (仍是15，因为bind时x=10)" << std::endl;

        // 需要引用语义时用 std::ref
        int y = 10;
        auto fn2 = std::bind([](int& val, int b) { return val + b; },
                             std::ref(y), _1);
        y = 100;
        std::cout << "  用ref后, y改为100, fn2(5) = " << fn2(5) << std::endl;
    }
    std::cout << std::endl;

    // 3. function 的性能开销
    std::cout << "[3] std::function 有运行时开销:\n";
    std::cout << "  std::function 使用类型擦除，可能涉及堆分配\n";
    std::cout << "  对性能敏感的场景，考虑直接用 lambda 或模板\n";
    std::cout << "  // 模板方式（零开销）:\n";
    std::cout << "  // template<typename F> void apply(F&& fn) { fn(); }\n\n";

    // 4. bind 的返回类型
    std::cout << "[4] bind 返回的类型是未指定的（不要试图推断具体类型）:\n";
    std::cout << "  // auto fn = std::bind(...);  // 正确，用auto\n";
    std::cout << "  // std::function<...> fn = std::bind(...);  // 也可以\n";
    std::cout << "  // 不要尝试写出 bind 返回值的具体类型\n\n";

    // 5. 优先使用 lambda 替代 bind
    std::cout << "[5] C++11后 lambda 通常比 bind 更清晰:\n";
    {
        using namespace std::placeholders;

        // bind 版本
        auto fn1 = std::bind(add, 10, _1);

        // lambda 版本（更直观）
        auto fn2 = [](int x) { return add(10, x); };

        std::cout << "  bind版: fn1(5) = " << fn1(5) << std::endl;
        std::cout << "  lambda版: fn2(5) = " << fn2(5) << std::endl;
        std::cout << "  lambda 更直观、可能更高效，推荐优先使用\n";
    }
    std::cout << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::function 与 std::bind - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
