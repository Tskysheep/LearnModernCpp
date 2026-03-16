---
sidebar_position: 31
title: "协程基础 (Coroutines: co_await, co_yield, co_return)"
description: "C++20 协程基础 (Coroutines: co_await, co_yield, co_return) 教学示例"
tags: [cpp20, coroutines_basics]
---

# 协程基础 (Coroutines: co_await, co_yield, co_return)

C++20 标准引入的 协程基础 (Coroutines: co_await, co_yield, co_return) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/30_coroutines_basics.cpp"
/**
 * ============================================================
 * 特性名称: 协程基础 (Coroutines: co_await, co_yield, co_return)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: 协程是一种可以暂停和恢复执行的函数。
 *          C++20 引入了 co_await、co_yield、co_return 三个关键字，
 *          以及底层的 promise_type 和 coroutine_handle 机制。
 *
 * [为什么引入]: 异步编程、生成器、状态机等场景需要函数能够
 *             暂停执行并在之后恢复。传统的回调和状态机代码复杂，
 *             协程提供了更直观的写法。
 *
 * [关键要点]:
 *   - co_await: 暂停协程，等待异步操作完成
 *   - co_yield: 产出一个值并暂停
 *   - co_return: 返回最终结果并结束协程
 *   - promise_type: 控制协程行为的类型
 *   - coroutine_handle: 用于恢复和销毁协程的句柄
 * ============================================================
 */

#include <coroutine>
#include <iostream>
#include <optional>
#include <string>
#include <utility>

// ===================== basic_usage() =====================

// 最简单的 Task 类型：支持 co_return
template<typename T>
class Task {
public:
    // promise_type 是协程的核心：控制协程的创建、暂停、恢复和销毁
    struct promise_type {
        std::optional<T> result;

        // 创建 Task 对象（协程的返回值）
        Task get_return_object() {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        // 协程开始时的行为：suspend_never = 立即开始执行
        std::suspend_always initial_suspend() noexcept { return {}; }

        // 协程结束时的行为：suspend_always = 暂停等待销毁
        std::suspend_always final_suspend() noexcept { return {}; }

        // co_return 的处理
        void return_value(T value) {
            result = std::move(value);
        }

        // 未捕获异常的处理
        void unhandled_exception() {
            std::terminate();
        }
    };

    // 构造和析构
    explicit Task(std::coroutine_handle<promise_type> h) : handle_(h) {}
    ~Task() {
        if (handle_) handle_.destroy();
    }

    // 禁止复制
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    // 允许移动
    Task(Task&& other) noexcept : handle_(std::exchange(other.handle_, nullptr)) {}
    Task& operator=(Task&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = std::exchange(other.handle_, nullptr);
        }
        return *this;
    }

    // 恢复协程执行
    void resume() {
        if (handle_ && !handle_.done()) {
            handle_.resume();
        }
    }

    // 获取结果
    T result() const {
        return handle_.promise().result.value();
    }

    // 协程是否完成
    bool done() const {
        return handle_.done();
    }

private:
    std::coroutine_handle<promise_type> handle_;
};

// 使用 co_return 的简单协程
Task<int> compute_value(int x, int y) {
    std::cout << "  [协程] 开始计算 " << x << " + " << y << "\n";
    co_return x + y;
}

Task<std::string> greet(const std::string& name) {
    std::cout << "  [协程] 生成问候语...\n";
    co_return "你好, " + name + "!";
}

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 创建协程（initial_suspend = suspend_always，所以不会立即执行）
    std::cout << "创建协程...\n";
    auto task = compute_value(3, 5);

    std::cout << "协程已创建但未执行\n";
    std::cout << "恢复执行...\n";
    task.resume();  // 执行到 co_return

    std::cout << "结果: " << task.result() << "\n";

    // 字符串协程
    std::cout << "\n创建字符串协程...\n";
    auto greeting = greet("C++20");
    greeting.resume();
    std::cout << "问候: " << greeting.result() << "\n";
}

// ===================== advanced_usage() =====================

// 支持暂停/恢复多次的协程（使用 co_await）
class Resumable {
public:
    struct promise_type {
        Resumable get_return_object() {
            return Resumable{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    explicit Resumable(std::coroutine_handle<promise_type> h) : handle_(h) {}
    ~Resumable() { if (handle_) handle_.destroy(); }

    Resumable(const Resumable&) = delete;
    Resumable& operator=(const Resumable&) = delete;
    Resumable(Resumable&& other) noexcept : handle_(std::exchange(other.handle_, nullptr)) {}

    void resume() {
        if (handle_ && !handle_.done()) {
            handle_.resume();
        }
    }

    bool done() const { return handle_.done(); }

private:
    std::coroutine_handle<promise_type> handle_;
};

// 多次暂停的协程
Resumable multi_step_task() {
    std::cout << "  [步骤 1] 初始化...\n";
    co_await std::suspend_always{};  // 暂停

    std::cout << "  [步骤 2] 处理数据...\n";
    co_await std::suspend_always{};  // 再次暂停

    std::cout << "  [步骤 3] 清理...\n";
    co_await std::suspend_always{};  // 再次暂停

    std::cout << "  [步骤 4] 完成!\n";
    // co_return; (隐式)
}

// 自定义 Awaitable 类型
struct TimedSuspend {
    int id;

    // Awaitable 三件套
    bool await_ready() const noexcept {
        std::cout << "    await_ready(" << id << "): 检查是否需要暂停\n";
        return false;  // false = 需要暂停
    }

    void await_suspend(std::coroutine_handle<>) const noexcept {
        std::cout << "    await_suspend(" << id << "): 协程已暂停\n";
    }

    int await_resume() const noexcept {
        std::cout << "    await_resume(" << id << "): 协程恢复，返回 " << id * 10 << "\n";
        return id * 10;  // co_await 表达式的结果
    }
};

// 支持 co_await 返回值的 Task
Resumable awaitable_demo() {
    std::cout << "  开始 awaitable 演示\n";

    // co_await 的过程：
    // 1. 调用 await_ready() → 如果 true，不暂停
    // 2. 如果 false，调用 await_suspend() → 暂停协程
    // 3. 恢复时调用 await_resume() → 返回结果
    co_await TimedSuspend{1};

    std::cout << "  第一次恢复后\n";
    co_await TimedSuspend{2};

    std::cout << "  第二次恢复后，完成!\n";
}

void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 多步骤协程
    std::cout << "-- 多步骤协程 (co_await suspend_always) --\n";
    auto task = multi_step_task();

    int step = 0;
    while (!task.done()) {
        std::cout << "[调用者] 恢复第 " << ++step << " 步\n";
        task.resume();
    }

    // 自定义 Awaitable
    std::cout << "\n-- 自定义 Awaitable --\n";
    auto demo = awaitable_demo();

    std::cout << "[调用者] 第1次恢复\n";
    demo.resume();

    std::cout << "[调用者] 第2次恢复\n";
    demo.resume();

    std::cout << "[调用者] 第3次恢复\n";
    demo.resume();

    // 协程句柄的生命周期
    std::cout << "\n-- 协程的生命周期 --\n";
    std::cout << "1. 调用协程函数 → 创建协程帧(堆分配)\n";
    std::cout << "2. initial_suspend → 决定是否立即执行\n";
    std::cout << "3. 执行协程体 → 遇到 co_await/co_yield 暂停\n";
    std::cout << "4. resume() → 从暂停点恢复执行\n";
    std::cout << "5. co_return 或函数结束 → final_suspend\n";
    std::cout << "6. destroy() → 销毁协程帧\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：协程帧的内存管理
    std::cout << "1. 协程帧在堆上分配，必须正确销毁:\n";
    std::cout << "   handle.destroy() 销毁协程帧\n";
    std::cout << "   通常在 Task 的析构函数中调用\n";
    std::cout << "   双重销毁 = 未定义行为!\n\n";

    // 陷阱2：悬垂引用
    std::cout << "2. 协程中的引用参数可能悬垂:\n";
    std::cout << "   Task foo(const string& s) {  // s 可能在暂停后失效!\n";
    std::cout << "       co_await something;\n";
    std::cout << "       cout << s;  // 如果调用者已销毁 s → 未定义行为\n";
    std::cout << "   }\n";
    std::cout << "   解决：按值传递参数\n\n";

    // 陷阱3：promise_type 必须包含特定成员
    std::cout << "3. promise_type 必须包含以下成员:\n";
    std::cout << "   - get_return_object()     → 创建返回对象\n";
    std::cout << "   - initial_suspend()       → 初始暂停策略\n";
    std::cout << "   - final_suspend()         → 最终暂停策略\n";
    std::cout << "   - unhandled_exception()   → 异常处理\n";
    std::cout << "   - return_void() 或 return_value()  → co_return 处理\n\n";

    // 陷阱4：final_suspend 通常应该返回 suspend_always
    std::cout << "4. final_suspend() 通常返回 suspend_always:\n";
    std::cout << "   如果返回 suspend_never，协程帧在结束时自动销毁\n";
    std::cout << "   这时 handle.destroy() 就不应该再调用\n";
    std::cout << "   而且结果可能还没被读取就销毁了\n\n";

    // 陷阱5：不能在构造函数/析构函数中使用 co_await
    std::cout << "5. 限制:\n";
    std::cout << "   - 构造函数和析构函数不能是协程\n";
    std::cout << "   - main 函数不能是协程\n";
    std::cout << "   - constexpr 函数不能是协程\n";
    std::cout << "   - 可变参数(varargs)函数不能是协程\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== 协程基础 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 协程基础 (Coroutines: co_await, co_yield, co_return)](https://en.cppreference.com/w/cpp/language/coroutines)
