/**
 * ============================================================
 * 特性名称: std::any (任意类型容器)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: std::any 可以存储任意可拷贝类型的单个值，运行时保存
 *          类型信息。是 void* 的类型安全替代品。
 *
 * [为什么引入]: C 风格的 void* 完全没有类型安全性，转换错误只会
 *              在运行时崩溃。std::any 提供类型安全的「万能容器」，
 *              转换类型不匹配时抛出异常而非导致 UB。
 *
 * [关键要点]:
 *   - 可以存储任何可拷贝构造的类型
 *   - std::any_cast<T> 取值，类型不匹配抛 std::bad_any_cast
 *   - std::any_cast<T*>(&a) 返回指针，不匹配返回 nullptr
 *   - .type() 返回 std::type_info，可用于运行时类型检查
 *   - 小对象优化(SBO)：小类型通常不会堆分配
 * ============================================================
 */

#include <iostream>
#include <any>
#include <optional>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 创建 any
    std::any a;                          // 空
    std::any b = 42;                     // int
    std::any c = 3.14;                   // double
    std::any d = std::string("hello");   // string

    // 2. 检查是否有值
    std::cout << "a 有值: " << a.has_value() << "\n";
    std::cout << "b 有值: " << b.has_value() << "\n";

    // 3. 获取类型信息
    std::cout << "b 的类型: " << b.type().name() << "\n";
    std::cout << "c 的类型: " << c.type().name() << "\n";
    std::cout << "d 的类型: " << d.type().name() << "\n";

    // 4. 用 any_cast 取值
    std::cout << "b = " << std::any_cast<int>(b) << "\n";
    std::cout << "c = " << std::any_cast<double>(c) << "\n";
    std::cout << "d = " << std::any_cast<std::string>(d) << "\n";

    // 5. 修改 any 的值（可以改变类型）
    b = std::string("now a string");
    std::cout << "b 现在是: " << std::any_cast<std::string>(b) << "\n";

    // 6. 重置
    b.reset();
    std::cout << "reset 后 b 有值: " << b.has_value() << "\n";

    // 7. any_cast 指针版本 —— 安全检查不抛异常
    std::any x = 100;
    if (auto* p = std::any_cast<int>(&x)) {
        std::cout << "x 是 int: " << *p << "\n";
    }
    if (auto* p = std::any_cast<double>(&x)) {
        std::cout << "x 是 double\n";
    } else {
        std::cout << "x 不是 double\n";
    }
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 属性系统 —— 用 map<string, any> 存储任意属性
class PropertyBag {
public:
    template<typename T>
    void set(const std::string& key, T value) {
        properties_[key] = std::move(value);
    }

    template<typename T>
    std::optional<T> get(const std::string& key) const {
        auto it = properties_.find(key);
        if (it == properties_.end()) return std::nullopt;
        try {
            return std::any_cast<T>(it->second);
        } catch (const std::bad_any_cast&) {
            return std::nullopt;
        }
    }

    void print_types() const {
        for (const auto& [key, value] : properties_) {
            std::cout << "  " << key << ": " << value.type().name() << "\n";
        }
    }

private:
    std::map<std::string, std::any> properties_;
};

// 2. 异构容器 —— vector<any>
void print_any_vector(const std::vector<std::any>& vec) {
    for (const auto& item : vec) {
        if (!item.has_value()) {
            std::cout << "[空] ";
        } else if (item.type() == typeid(int)) {
            std::cout << "[int:" << std::any_cast<int>(item) << "] ";
        } else if (item.type() == typeid(double)) {
            std::cout << "[double:" << std::any_cast<double>(item) << "] ";
        } else if (item.type() == typeid(std::string)) {
            std::cout << "[string:" << std::any_cast<std::string>(item) << "] ";
        } else {
            std::cout << "[未知类型] ";
        }
    }
    std::cout << "\n";
}

// 3. 回调存储
class EventSystem {
public:
    void store_callback(const std::string& event, std::any callback) {
        callbacks_[event] = std::move(callback);
    }

    template<typename Func, typename... Args>
    void trigger(const std::string& event, Args&&... args) {
        auto it = callbacks_.find(event);
        if (it != callbacks_.end()) {
            auto& func = std::any_cast<Func&>(it->second);
            func(std::forward<Args>(args)...);
        }
    }

private:
    std::map<std::string, std::any> callbacks_;
};

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 属性系统
    PropertyBag bag;
    bag.set("name", std::string("Widget"));
    bag.set("width", 100);
    bag.set("height", 200);
    bag.set("visible", true);
    bag.set("opacity", 0.95);

    std::cout << "属性列表:\n";
    bag.print_types();

    std::cout << "name = " << bag.get<std::string>("name").value_or("N/A") << "\n";
    std::cout << "width = " << bag.get<int>("width").value_or(0) << "\n";
    // 类型不匹配时返回 nullopt
    std::cout << "width as string = " << bag.get<std::string>("width").value_or("类型不匹配") << "\n";

    // 异构容器
    std::cout << "\n异构容器: ";
    std::vector<std::any> mixed{42, 3.14, std::string("hello"), std::any{}};
    print_any_vector(mixed);

    // emplace 构造
    std::any a;
    a.emplace<std::string>(5, 'X');  // 构造 "XXXXX"
    std::cout << "emplace: " << std::any_cast<std::string>(a) << "\n";

    // make_any
    auto b = std::make_any<std::vector<int>>(std::initializer_list<int>{1, 2, 3});
    auto& vec = std::any_cast<std::vector<int>&>(b);
    std::cout << "make_any vector 大小: " << vec.size() << "\n";
}

// ============================================================
// 与 void* 的对比
// ============================================================
void compare_with_void_ptr() {
    std::cout << "\n--- 与 void* 对比 ---\n";

    // void* 方式 —— 危险！
    int value = 42;
    void* vp = &value;
    // double d = *(double*)vp;  // UB！类型错误但不会报错
    int i = *(int*)vp;  // 必须自己记住类型
    std::cout << "void* 方式: " << i << " (需要自己跟踪类型)\n";

    // std::any 方式 —— 安全
    std::any a = 42;
    try {
        double d = std::any_cast<double>(a);  // 类型不匹配 → 抛异常
        (void)d;
    } catch (const std::bad_any_cast& e) {
        std::cout << "any 方式: 类型不匹配被安全捕获 - " << e.what() << "\n";
    }

    // any 的优势：
    // 1. 类型安全 —— 转换错误是异常，不是 UB
    // 2. 值语义 —— 拷贝 any 会深拷贝内容
    // 3. 自动管理生命周期 —— 不需要手动 delete
    std::any original = std::string("deep copy test");
    std::any copied = original;  // 深拷贝
    std::any_cast<std::string&>(copied) = "modified";
    std::cout << "原始: " << std::any_cast<std::string>(original) << "\n";
    std::cout << "拷贝: " << std::any_cast<std::string>(copied) << "\n";
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: any_cast 类型必须精确匹配
    std::any a = 42;  // int
    // std::any_cast<long>(a);  // 抛异常！long != int
    // std::any_cast<const int>(a);  // 也可能出问题
    std::cout << "陷阱1 - any_cast 类型必须精确匹配 (int != long)\n";

    // 陷阱 2: 字符串字面量存储为 const char*，不是 string
    std::any s1 = "hello";                   // const char*！
    std::any s2 = std::string("hello");      // string
    std::cout << "陷阱2 - \"hello\" 的类型: " << s1.type().name() << "\n";
    std::cout << "         string(\"hello\"): " << s2.type().name() << "\n";

    // 陷阱 3: any 有运行时开销 —— 大对象需要堆分配
    std::cout << "陷阱3 - sizeof(any) = " << sizeof(std::any)
              << " (通常有 SBO 缓冲区)\n";

    // 陷阱 4: any 只能存储可拷贝的类型
    //   std::any a = std::unique_ptr<int>(new int(42));  // 编译错误！
    //   unique_ptr 不可拷贝
    std::cout << "陷阱4 - any 只能存储可拷贝类型 (unique_ptr 不可以)\n";

    // 陷阱 5: 不要过度使用 any —— 如果类型已知，用 variant
    //   any 是「类型擦除」，丢失了编译期类型信息
    //   variant 保留类型信息，编译期检查更完善
    std::cout << "陷阱5 - 类型已知时优先用 variant，any 是最后手段\n";

    // 陷阱 6: 用引用 any_cast 可以修改 any 中的值
    std::any val = 42;
    std::any_cast<int&>(val) = 100;
    std::cout << "陷阱6 - 引用 any_cast 修改: " << std::any_cast<int>(val) << "\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::any (任意类型容器) - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    compare_with_void_ptr();
    pitfalls();

    return 0;
}
