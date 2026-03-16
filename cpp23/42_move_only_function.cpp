/**
 * ============================================================
 * 特性名称: std::move_only_function（仅移动函数包装器）
 * 所属标准: C++23
 * ============================================================
 *
 * [是什么]: std::move_only_function 是一个类似 std::function 的
 *          类型擦除函数包装器，但它只要求可调用对象可移动（不要求
 *          可复制）。它可以存储仅移动的可调用对象，如持有
 *          unique_ptr 的 Lambda。
 *
 * [为什么引入]: std::function 要求存储的可调用对象可复制，这导致
 *              无法存储持有 unique_ptr 等仅移动资源的 Lambda。
 *              move_only_function 解除了这一限制，同时签名中还能
 *              指定 const、引用限定符和 noexcept。
 *
 * [关键要点]:
 *   - 只要求可调用对象可移动，不要求可复制
 *   - move_only_function 本身也是仅移动的（不可复制）
 *   - 签名可以包含 const、&/&&、noexcept 限定
 *   - 比 std::function 更轻量（不需要处理复制语义）
 *   - 调用空的 move_only_function 会导致未定义行为（不抛异常）
 * ============================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>

// ============================================================
// 特性检测
// ============================================================
#if __has_include(<functional>)
#ifdef __cpp_lib_move_only_function
#define HAS_MOVE_ONLY_FUNCTION 1
#else
#define HAS_MOVE_ONLY_FUNCTION 0
#endif
#else
#define HAS_MOVE_ONLY_FUNCTION 0
#endif

#if HAS_MOVE_ONLY_FUNCTION

// ============================================================
// 基础用法
// ============================================================
namespace basic_usage {

// 一个持有资源的可调用对象（仅移动）
struct ResourceHandler {
    std::unique_ptr<std::string> resource;

    ResourceHandler(const std::string& name)
        : resource(std::make_unique<std::string>(name)) {}

    // 不可复制
    ResourceHandler(const ResourceHandler&) = delete;
    ResourceHandler& operator=(const ResourceHandler&) = delete;

    // 可移动
    ResourceHandler(ResourceHandler&&) = default;
    ResourceHandler& operator=(ResourceHandler&&) = default;

    std::string operator()() const {
        return resource ? ("处理资源: " + *resource) : "资源已移走";
    }
};

void demonstrate() {
    std::cout << "[基础用法]" << std::endl;

    // --- 基本使用 ---
    std::cout << "  -- 基本的 move_only_function --" << std::endl;

    // 存储普通 Lambda
    std::move_only_function<int(int, int)> add = [](int a, int b) { return a + b; };
    std::cout << "  add(3, 4) = " << add(3, 4) << std::endl;

    // 存储仅移动的 Lambda（持有 unique_ptr）
    auto ptr = std::make_unique<int>(42);
    std::move_only_function<int()> get_value = [p = std::move(ptr)]() {
        return *p;
    };
    std::cout << "  get_value() = " << get_value() << std::endl;

    // 存储仅移动的可调用对象
    ResourceHandler handler("数据库连接");
    std::move_only_function<std::string()> func = std::move(handler);
    std::cout << "  " << func() << std::endl;

    // 注意：以下代码会编译失败（std::function 无法存储仅移动对象）
    // std::function<int()> bad = [p = std::make_unique<int>(42)]() { return *p; };
    // 错误：unique_ptr 不可复制！

    // --- 检查是否有值 ---
    std::cout << "  -- 检查状态 --" << std::endl;
    std::move_only_function<void()> empty_func;
    std::cout << "  empty_func 有值: " << (empty_func ? "是" : "否") << std::endl;

    std::move_only_function<void()> valid_func = []() {};
    std::cout << "  valid_func 有值: " << (valid_func ? "是" : "否") << std::endl;

    // --- 移动语义 ---
    std::cout << "  -- 移动语义 --" << std::endl;
    std::move_only_function<int()> f1 = []() { return 100; };
    std::move_only_function<int()> f2 = std::move(f1);
    // f1 现在为空
    std::cout << "  移动后 f1 有值: " << (f1 ? "是" : "否") << std::endl;
    std::cout << "  移动后 f2() = " << f2() << std::endl;

    std::cout << std::endl;
}

} // namespace basic_usage

// ============================================================
// 进阶用法
// ============================================================
namespace advanced_usage {

// 任务队列：使用 move_only_function 存储仅移动的任务
class TaskQueue {
public:
    using Task = std::move_only_function<void()>;

    void push(Task task) {
        tasks_.push_back(std::move(task));
    }

    void execute_all() {
        for (auto& task : tasks_) {
            if (task) task();
        }
        tasks_.clear();
    }

    std::size_t size() const { return tasks_.size(); }

private:
    std::vector<Task> tasks_;
};

// 带 const 限定的签名
void demonstrate_const_qualified() {
    std::cout << "  -- const 限定签名 --" << std::endl;

    // const 限定：调用时 this 是 const 的
    std::move_only_function<int() const> const_func = []() { return 42; };
    // 即使通过 const 引用也能调用
    const auto& ref = const_func;
    std::cout << "  const 限定函数: " << ref() << std::endl;

    // 非 const 版本（默认）：不能通过 const 引用调用
    std::move_only_function<int()> non_const_func = []() { return 42; };
    // const auto& ref2 = non_const_func;
    // ref2();  // 编译错误！非 const 签名不能通过 const 引用调用
    std::cout << "  非 const 函数: " << non_const_func() << std::endl;
}

void demonstrate() {
    std::cout << "[进阶用法]" << std::endl;

    // 场景1：任务队列
    std::cout << "  -- 任务队列（仅移动任务） --" << std::endl;
    TaskQueue queue;

    // 添加普通任务
    queue.push([]() { std::cout << "    任务1: 简单打印" << std::endl; });

    // 添加持有资源的任务
    auto data = std::make_unique<std::vector<int>>(std::vector<int>{1, 2, 3});
    queue.push([d = std::move(data)]() {
        std::cout << "    任务2: 处理数据，大小 = " << d->size() << std::endl;
    });

    // 添加一次性清理任务
    auto resource = std::make_unique<std::string>("临时文件.tmp");
    queue.push([r = std::move(resource)]() {
        std::cout << "    任务3: 清理 " << *r << std::endl;
    });

    std::cout << "  队列中有 " << queue.size() << " 个任务" << std::endl;
    queue.execute_all();
    std::cout << "  执行后队列大小: " << queue.size() << std::endl;

    // 场景2：回调注册
    std::cout << "  -- 回调注册 --" << std::endl;
    using Callback = std::move_only_function<void(const std::string&)>;
    std::vector<Callback> callbacks;

    // 注册不同类型的回调
    callbacks.push_back([](const std::string& msg) {
        std::cout << "    [日志] " << msg << std::endl;
    });

    auto counter = std::make_unique<int>(0);
    callbacks.push_back([c = std::move(counter)](const std::string& msg) {
        ++(*c);
        std::cout << "    [计数 #" << *c << "] " << msg << std::endl;
    });

    // 触发回调
    for (auto& cb : callbacks) {
        cb("事件发生");
    }
    for (auto& cb : callbacks) {
        cb("又一个事件");
    }

    // 场景3：const 限定
    demonstrate_const_qualified();

    // 场景4：与 std::function 对比
    std::cout << "  -- 与 std::function 对比 --" << std::endl;
    std::cout << "  std::function:           可复制，可移动，可存储可复制对象" << std::endl;
    std::cout << "  std::move_only_function: 仅移动，可存储仅移动对象" << std::endl;
    std::cout << "  std::function 大小:           " << sizeof(std::function<void()>) << " 字节" << std::endl;
    std::cout << "  std::move_only_function 大小: " << sizeof(std::move_only_function<void()>) << " 字节" << std::endl;

    std::cout << std::endl;
}

} // namespace advanced_usage

// ============================================================
// 易错点与陷阱
// ============================================================
namespace pitfalls {

void demonstrate() {
    std::cout << "[易错点与陷阱]" << std::endl;

    // 陷阱1：调用空的 move_only_function 是未定义行为
    std::cout << "  陷阱1: 调用空的 move_only_function 是未定义行为" << std::endl;
    std::cout << "    - std::function: 调用空对象抛 std::bad_function_call" << std::endl;
    std::cout << "    - move_only_function: 调用空对象是 UB！" << std::endl;
    std::cout << "    - 必须在调用前检查: if (func) func();" << std::endl;

    // 陷阱2：不可复制
    std::cout << "  陷阱2: move_only_function 本身不可复制" << std::endl;
    std::move_only_function<int()> f = []() { return 42; };
    // auto f2 = f;  // 编译错误！不可复制
    auto f2 = std::move(f);  // 正确：可以移动
    std::cout << "    只能 std::move，不能复制" << std::endl;
    (void)f2;

    // 陷阱3：移动后原对象为空
    std::cout << "  陷阱3: 移动后原对象变为空" << std::endl;
    std::cout << "    移动后 f 有值: " << (f ? "是" : "否") << std::endl;
    std::cout << "    不要使用已移动的 move_only_function" << std::endl;

    // 陷阱4：签名中的限定符
    std::cout << "  陷阱4: 注意签名中的限定符" << std::endl;
    std::cout << "    move_only_function<int()>        // 非 const，左值调用" << std::endl;
    std::cout << "    move_only_function<int() const>  // const，左值和 const 调用" << std::endl;
    std::cout << "    move_only_function<int() &&>     // 仅右值调用（一次性）" << std::endl;
    std::cout << "    move_only_function<int() noexcept> // noexcept 调用" << std::endl;

    // 陷阱5：不能与需要复制的容器操作一起使用
    std::cout << "  陷阱5: 某些算法需要可复制的函数对象" << std::endl;
    std::cout << "    - std::sort 的比较器需要可复制 -> 不能用 move_only_function" << std::endl;
    std::cout << "    - 可以在 vector 中 push_back(std::move(f))" << std::endl;

    std::cout << std::endl;
}

} // namespace pitfalls

#endif // HAS_MOVE_ONLY_FUNCTION

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::move_only_function（仅移动函数包装器） - C++23 =====\n" << std::endl;

#if HAS_MOVE_ONLY_FUNCTION
    basic_usage::demonstrate();
    advanced_usage::demonstrate();
    pitfalls::demonstrate();
#else
    std::cout << "[提示] 当前编译器不支持 std::move_only_function 特性。" << std::endl;
    std::cout << "请使用支持 C++23 的编译器（如 GCC 14+ 或 Clang 18+）。" << std::endl;
    std::cout << std::endl;
    std::cout << "std::move_only_function 语法示例（供参考）：" << std::endl;
    std::cout << R"(
    #include <functional>
    #include <memory>

    // 存储仅移动的 Lambda
    auto ptr = std::make_unique<int>(42);
    std::move_only_function<int()> func = [p = std::move(ptr)]() {
        return *p;
    };

    int result = func();  // 调用

    // 与 std::function 对比：
    // std::function<int()> bad = [p = std::move(ptr)]() { return *p; };
    // 上面会编译失败！std::function 要求可复制

    // 任务队列
    std::vector<std::move_only_function<void()>> tasks;
    tasks.push_back([data = std::make_unique<int>(1)]() {
        std::cout << *data;
    });
)" << std::endl;
#endif

    return 0;
}
