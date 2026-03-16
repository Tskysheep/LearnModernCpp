/**
 * ============================================================
 * 特性名称: Lambda表达式
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: Lambda是一种匿名函数对象，可以在代码中就地定义并使用。
 *          语法: [捕获列表](参数列表) -> 返回类型 { 函数体 }
 *
 * [为什么引入]: C++03中使用STL算法需要单独定义函数对象或函数指针，
 *   代码分散且不直观。Lambda让代码更紧凑，逻辑更集中。
 *
 * [关键要点]:
 *   - 捕获方式: [=]值捕获全部, [&]引用捕获全部, 可混合指定
 *   - 默认情况下Lambda的operator()是const的，用mutable可修改值捕获的副本
 *   - 返回类型可省略（编译器推导），复杂时需显式指定
 *   - Lambda可赋值给std::function或用auto保存
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <numeric>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "===== 基础用法 =====" << std::endl;

    // 1. 最简单的Lambda
    auto hello = []() {
        std::cout << "你好，Lambda世界!" << std::endl;
    };
    hello();

    // 2. 带参数的Lambda
    auto add = [](int a, int b) { return a + b; };
    std::cout << "3 + 5 = " << add(3, 5) << std::endl;

    // 3. Lambda配合STL算法
    std::vector<int> nums = {5, 2, 8, 1, 9, 3, 7};

    // 排序（降序）
    std::sort(nums.begin(), nums.end(), [](int a, int b) {
        return a > b;
    });
    std::cout << "降序排序: ";
    for (auto n : nums) std::cout << n << " ";
    std::cout << std::endl;

    // 查找第一个偶数
    auto it = std::find_if(nums.begin(), nums.end(), [](int n) {
        return n % 2 == 0;
    });
    if (it != nums.end()) {
        std::cout << "第一个偶数: " << *it << std::endl;
    }

    // 4. 各种捕获方式
    int x = 10, y = 20;

    auto val_capture = [=]() { return x + y; };                 // 值捕获全部
    auto ref_capture = [&]() { x += 1; y += 1; };              // 引用捕获全部
    auto mix_capture = [x, &y]() { y = x * 2; };               // x值捕获, y引用捕获
    auto specific    = [&x, y]() { x = y + 100; };             // x引用捕获, y值捕获

    std::cout << "捕获前: x=" << x << ", y=" << y << std::endl;
    ref_capture();
    std::cout << "引用捕获后: x=" << x << ", y=" << y << std::endl;
    mix_capture();
    std::cout << "混合捕获后: x=" << x << ", y=" << y << std::endl;
    specific();
    std::cout << "指定捕获后: x=" << x << ", y=" << y << std::endl;
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n===== 进阶用法 =====" << std::endl;

    // 1. mutable: 允许修改值捕获的副本
    int count = 0;
    auto counter = [count]() mutable -> int {
        return ++count; // 修改的是Lambda内部的副本
    };
    std::cout << "计数器: " << counter() << ", " << counter() << ", " << counter() << std::endl;
    std::cout << "外部count仍为: " << count << std::endl;

    // 2. 显式指定返回类型
    auto safe_divide = [](double a, double b) -> double {
        if (b == 0.0) return 0.0;
        return a / b;
    };
    std::cout << "10 / 3 = " << safe_divide(10, 3) << std::endl;
    std::cout << "10 / 0 = " << safe_divide(10, 0) << std::endl;

    // 3. Lambda作为回调函数
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 用accumulate进行自定义求和（只累加偶数）
    int even_sum = std::accumulate(data.begin(), data.end(), 0,
        [](int sum, int val) {
            return val % 2 == 0 ? sum + val : sum;
        });
    std::cout << "偶数之和: " << even_sum << std::endl;

    // 用for_each处理每个元素
    std::cout << "平方: ";
    std::for_each(data.begin(), data.end(), [](int n) {
        std::cout << n * n << " ";
    });
    std::cout << std::endl;

    // 4. 保存Lambda到std::function（可以存储在容器中）
    std::vector<std::function<int(int, int)>> operations;
    operations.push_back([](int a, int b) { return a + b; });
    operations.push_back([](int a, int b) { return a - b; });
    operations.push_back([](int a, int b) { return a * b; });

    const char* op_names[] = {"加", "减", "乘"};
    for (size_t i = 0; i < operations.size(); ++i) {
        std::cout << "6 " << op_names[i] << " 3 = " << operations[i](6, 3) << std::endl;
    }

    // 5. 捕获this指针
    struct Calculator {
        int base = 100;
        std::function<int(int)> getAdder() {
            return [this](int x) { return base + x; }; // 捕获this访问成员
        }
    };
    Calculator calc;
    auto adder = calc.getAdder();
    std::cout << "base(100) + 42 = " << adder(42) << std::endl;
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n===== 易错点与陷阱 =====" << std::endl;

    // 陷阱1: 引用捕获悬挂引用
    std::function<int()> bad_lambda;
    {
        int local_var = 42;
        bad_lambda = [&local_var]() { return local_var; };
        // local_var 在此作用域结束后被销毁
    }
    // bad_lambda(); // 未定义行为! local_var已销毁!
    std::cout << "警告: 引用捕获的变量必须在Lambda生命周期内有效!" << std::endl;

    // 陷阱2: [=]默认值捕获可能意外捕获this
    // 在成员函数中 [=] 会捕获this指针（而非成员的拷贝）
    // 如果对象先于Lambda销毁，使用this就是未定义行为
    std::cout << "注意: 在成员函数中[=]捕获的是this指针，不是成员的副本" << std::endl;

    // 陷阱3: Lambda类型是唯一的
    auto f1 = [](int x) { return x; };
    auto f2 = [](int x) { return x; };
    // f1和f2的类型是不同的! 即使函数体完全一样
    // f1 = f2; // 编译错误! 类型不同
    // 要存储不同的Lambda，请用std::function
    std::function<int(int)> func = f1;
    func = f2; // OK，std::function可以存储不同的可调用对象

    // 陷阱4: 值捕获的时机
    int x = 10;
    auto capture_now = [x]() { return x; }; // 此刻x=10被捕获
    x = 999;
    std::cout << "捕获时x=10, 现在x=" << x
              << ", Lambda返回: " << capture_now() << std::endl;

    // 陷阱5: 大对象的值捕获会产生拷贝开销
    std::cout << "提示: 对大对象优先使用引用捕获[&]，注意生命周期" << std::endl;
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  C++11 特性: Lambda表达式" << std::endl;
    std::cout << "========================================" << std::endl;

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
