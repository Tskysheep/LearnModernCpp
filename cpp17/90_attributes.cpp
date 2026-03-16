/**
 * ============================================================
 * 特性名称: C++17 属性 [[nodiscard]]、[[maybe_unused]]、[[fallthrough]]
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: C++17 新增三个标准属性(attributes)：
 *   - [[nodiscard]]: 标记函数返回值不应被忽略
 *   - [[maybe_unused]]: 标记变量/参数/函数可能未使用，消除编译器警告
 *   - [[fallthrough]]: 在 switch 的 case 中显式标注故意贯穿
 *
 * [为什么引入]: 编译器对忽略返回值、未使用变量、switch 贯穿等会
 *              产生警告。这三个属性让程序员的意图更明确，帮助编译器
 *              区分「真正的 bug」和「故意为之」。
 *
 * [关键要点]:
 *   - [[nodiscard]] 可用于函数、类、枚举
 *   - [[maybe_unused]] 可用于变量、函数、参数、typedef
 *   - [[fallthrough]] 只能用于 switch 的 case 标签之前
 *   - C++20 扩展了 [[nodiscard("reason")]] 带原因字符串
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <cstdlib>

// ============================================================
// 基础用法
// ============================================================

// ---- [[nodiscard]] ----

// 1. 标记函数返回值不应被忽略
[[nodiscard]] int compute_result() {
    return 42;
}

// 2. 标记错误码 —— 最常见的用途
[[nodiscard]] bool save_to_file(const std::string& filename) {
    // 模拟保存操作
    std::cout << "  保存到 " << filename << "\n";
    return true;  // 返回是否成功
}

// 3. 工厂函数 —— 忽略返回值意味着内存泄漏
[[nodiscard]] int* create_resource() {
    return new int(100);
}

// 4. 标记整个类/结构体
struct [[nodiscard]] ErrorCode {
    int code;
    std::string message;
};

ErrorCode validate(int value) {
    if (value < 0) return {-1, "负数"};
    return {0, "正常"};
}

// ---- [[maybe_unused]] ----

// 5. 标记可能未使用的函数
[[maybe_unused]] static void debug_helper() {
    std::cout << "调试辅助函数\n";
}

// 6. 标记可能未使用的参数
void event_handler([[maybe_unused]] int event_id,
                   [[maybe_unused]] const std::string& data) {
    // 某些事件不需要 data 参数
    std::cout << "  处理事件 " << event_id << "\n";
}

// ---- [[fallthrough]] ----

// 7. switch 中的故意贯穿
std::string get_permission_name(int level) {
    std::string perms;
    switch (level) {
        case 3:
            perms += "执行 ";
            [[fallthrough]];  // 故意贯穿到 case 2
        case 2:
            perms += "写入 ";
            [[fallthrough]];  // 故意贯穿到 case 1
        case 1:
            perms += "读取";
            break;
        case 0:
            perms = "无权限";
            break;
        default:
            perms = "未知";
            break;
    }
    return perms;
}

void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // [[nodiscard]]
    std::cout << "== [[nodiscard]] ==\n";
    int result = compute_result();  // OK: 使用了返回值
    std::cout << "计算结果: " << result << "\n";

    // compute_result();  // 警告！忽略了 nodiscard 返回值
    // 如果确实要忽略，显式转换：
    (void)compute_result();  // OK: 显式忽略

    bool ok = save_to_file("data.txt");  // OK: 检查了返回值
    std::cout << "保存" << (ok ? "成功" : "失败") << "\n";

    auto* ptr = create_resource();  // OK: 使用了返回值
    std::cout << "资源值: " << *ptr << "\n";
    delete ptr;

    // ErrorCode 类型标记为 nodiscard
    auto err = validate(42);  // OK: 使用了返回值
    std::cout << "验证结果: code=" << err.code << ", msg=" << err.message << "\n";
    // validate(-1);  // 警告！忽略了 nodiscard 类型的返回值

    // [[maybe_unused]]
    std::cout << "\n== [[maybe_unused]] ==\n";
    [[maybe_unused]] int debug_var = 42;  // 不会产生"未使用变量"警告
    event_handler(1, "click");

    // [[fallthrough]]
    std::cout << "\n== [[fallthrough]] ==\n";
    for (int level = 0; level <= 3; ++level) {
        std::cout << "  权限等级 " << level << ": " << get_permission_name(level) << "\n";
    }
}

// ============================================================
// 进阶用法
// ============================================================

// 1. [[nodiscard]] 用于 RAII 锁
class [[nodiscard]] ScopedLock {
public:
    ScopedLock(const std::string& name) : name_(name) {
        std::cout << "    锁定: " << name_ << "\n";
    }
    ~ScopedLock() {
        std::cout << "    解锁: " << name_ << "\n";
    }
private:
    std::string name_;
};

[[nodiscard]] ScopedLock acquire_lock(const std::string& name) {
    return ScopedLock(name);
}

// 2. [[nodiscard]] 用于算术运算结果
class SafeInt {
    int value_;
public:
    explicit SafeInt(int v) : value_(v) {}
    int get() const { return value_; }

    [[nodiscard]] SafeInt operator+(SafeInt other) const {
        return SafeInt(value_ + other.value_);
    }

    [[nodiscard]] SafeInt operator*(SafeInt other) const {
        return SafeInt(value_ * other.value_);
    }
};

// 3. [[maybe_unused]] 在条件编译中
void platform_specific() {
    [[maybe_unused]] auto start = std::chrono::steady_clock::now();

    // 执行一些操作...
    volatile int sum = 0;
    for (int i = 0; i < 1000; ++i) sum += i;

#ifdef ENABLE_PROFILING
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "  耗时: " << elapsed.count() << " 微秒\n";
#endif
    std::cout << "  平台特定操作完成\n";
}

// 4. [[fallthrough]] 在状态机中
enum class TokenType { Number, Operator, Whitespace, Unknown };

TokenType classify_char(char c) {
    TokenType type = TokenType::Unknown;
    switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            type = TokenType::Number;
            break;

        case '+':
        case '-':
            // + 和 - 既是运算符也可能是数字的符号
            // 这里简单当作运算符
            [[fallthrough]];
        case '*':
        case '/':
            type = TokenType::Operator;
            break;

        case ' ':
        case '\t':
        case '\n':
            type = TokenType::Whitespace;
            break;

        default:
            type = TokenType::Unknown;
            break;
    }
    return type;
}

// 5. 模板中的 [[maybe_unused]]
template<typename T>
void process_value(T value) {
    // 根据类型，某些变量可能不被使用
    [[maybe_unused]] auto size = sizeof(T);
    [[maybe_unused]] bool is_int = std::is_integral_v<T>;

    if constexpr (std::is_integral_v<T>) {
        std::cout << "  整数处理: " << value << " (大小: " << size << ")\n";
    } else {
        std::cout << "  非整数处理: " << value << "\n";
    }
}

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // RAII 锁
    std::cout << "RAII 锁:\n";
    {
        auto lock = acquire_lock("数据库");
        std::cout << "    在锁保护下执行操作\n";
    }
    // acquire_lock("temp");  // 警告！锁立即销毁

    // SafeInt
    SafeInt a(3), b(4);
    SafeInt c = a + b;  // OK: 使用了返回值
    std::cout << "\nSafeInt: 3 + 4 = " << c.get() << "\n";
    // a + b;  // 警告！忽略了 nodiscard 的运算结果

    // 条件编译
    std::cout << "\n条件编译:\n";
    platform_specific();

    // 字符分类
    std::cout << "\n字符分类:\n";
    for (char c : {'5', '+', ' ', 'x'}) {
        auto type = classify_char(c);
        std::cout << "  '" << c << "' -> ";
        switch (type) {
            case TokenType::Number:     std::cout << "数字\n"; break;
            case TokenType::Operator:   std::cout << "运算符\n"; break;
            case TokenType::Whitespace: std::cout << "空白\n"; break;
            case TokenType::Unknown:    std::cout << "未知\n"; break;
        }
    }

    // 模板中的 maybe_unused
    std::cout << "\n模板处理:\n";
    process_value(42);
    process_value(3.14);
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: [[nodiscard]] 可以被 (void) 显式忽略
    (void)compute_result();  // 合法：显式表示"我知道返回值，但不需要"
    std::cout << "陷阱1 - (void) 可以显式忽略 nodiscard 警告\n";

    // 陷阱 2: [[fallthrough]] 必须在 case 的最后一个语句之后
    //   不能放在 case 的中间
    std::cout << "陷阱2 - [[fallthrough]] 必须紧接在下一个 case 标签之前\n";

    // 陷阱 3: [[fallthrough]] 不等于 break
    //   它只是告诉编译器"这是故意的"，不改变控制流
    std::cout << "陷阱3 - [[fallthrough]] 不改变控制流，只消除警告\n";

    // 陷阱 4: [[maybe_unused]] 不意味着"应该不使用"
    //   它只是"可能在某些配置下不使用"
    std::cout << "陷阱4 - [[maybe_unused]] 表示'可能不使用'，不是'不应使用'\n";

    // 陷阱 5: [[nodiscard]] 在 C++17 只能加消息在 C++20
    //   C++17: [[nodiscard]]
    //   C++20: [[nodiscard("必须检查错误码")]]
    std::cout << "陷阱5 - [[nodiscard(\"原因\")]] 是 C++20 特性\n";

    // 陷阱 6: 属性的位置很重要
    //   [[nodiscard]] 放在函数声明前
    //   [[maybe_unused]] 放在变量/参数声明中
    //   [[fallthrough]]; 是一个语句（注意分号）
    std::cout << "陷阱6 - 属性的位置: nodiscard 在函数前, "
              << "maybe_unused 在声明中, fallthrough 是语句\n";

    // 陷阱 7: switch 没有 break 也没有 fallthrough 会产生警告
    //   每个非空 case 要么 break/return，要么 [[fallthrough]]
    std::cout << "陷阱7 - 非空 case 应该有 break 或 [[fallthrough]]\n";

    // 展示三个属性的组合使用
    std::cout << "\n--- 组合使用示例 ---\n";
    [[maybe_unused]] auto err = validate(42);  // nodiscard + maybe_unused
    std::cout << "validate 返回值被标记为 maybe_unused (合法但不推荐)\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== [[nodiscard]] / [[maybe_unused]] / [[fallthrough]] - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
