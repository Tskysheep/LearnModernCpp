---
sidebar_position: 74
slug: 73-future-promise
title: "future、promise、async、packaged_task"
description: "C++11 future、promise、async、packaged_task 教学示例"
tags: [cpp11, future_promise]
---

# future、promise、async、packaged_task

C++11 标准引入的 future、promise、async、packaged_task 特性完整教学示例。

## 完整源码

```cpp title="cpp11/73_future_promise.cpp"
/**
 * ============================================================
 * 特性名称: future、promise、async、packaged_task
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: C++11提供了高级异步编程工具：std::future 表示异步结果的占位符，
 *           std::promise 是结果的提供者，std::async 启动异步任务，
 *           std::packaged_task 包装可调用对象为异步任务。
 *
 * [为什么引入]: 直接使用 thread + mutex + condition_variable 进行异步编程
 *               代码复杂且容易出错。future/promise 提供了更高层次的抽象，
 *               简化了异步结果的传递。
 *
 * [关键要点]:
 *   - std::async: 最简单的异步执行方式，返回 future
 *   - std::future: 获取异步操作的结果（通过 get() 阻塞等待）
 *   - std::promise: 手动设置异步结果，配合 future 使用
 *   - std::packaged_task: 将可调用对象包装为异步任务
 *   - future::get() 只能调用一次（如需多次获取用 shared_future）
 * ============================================================
 */

#include <iostream>
#include <future>
#include <thread>
#include <string>
#include <vector>
#include <numeric>
#include <chrono>
#include <functional>
#include <stdexcept>
#include <mutex>

std::mutex g_cout_mtx;

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. std::async 异步计算
    std::cout << "[1] std::async 异步计算:\n";
    {
        // 异步执行一个耗时计算
        auto fut = std::async(std::launch::async, []() -> int {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return 42;
        });

        std::cout << "  异步任务已启动，主线程继续...\n";
        std::cout << "  做一些其他工作...\n";

        // get() 阻塞等待结果
        int result = fut.get();
        std::cout << "  异步结果: " << result << std::endl;
    }
    std::cout << std::endl;

    // 2. async 返回值类型推导
    std::cout << "[2] async 支持各种返回类型:\n";
    {
        // 返回 string
        auto fut1 = std::async(std::launch::async, []() -> std::string {
            return "异步字符串";
        });

        // 返回 double
        auto fut2 = std::async(std::launch::async, [](double x) {
            return x * x;
        }, 3.14);

        std::cout << "  字符串结果: " << fut1.get() << std::endl;
        std::cout << "  计算结果: " << fut2.get() << std::endl;
    }
    std::cout << std::endl;

    // 3. 启动策略
    std::cout << "[3] async 启动策略:\n";
    {
        // std::launch::async - 保证在新线程中执行
        auto fut1 = std::async(std::launch::async, []() {
            return std::string("async: 新线程执行");
        });

        // std::launch::deferred - 延迟到 get()/wait() 时在当前线程执行
        auto fut2 = std::async(std::launch::deferred, []() {
            return std::string("deferred: 延迟执行");
        });

        // 默认策略 - 由实现决定是 async 还是 deferred
        auto fut3 = std::async([]() {
            return std::string("默认策略");
        });

        std::cout << "  " << fut1.get() << std::endl;
        std::cout << "  " << fut2.get() << " (调用get时才执行)" << std::endl;
        std::cout << "  " << fut3.get() << std::endl;
    }
    std::cout << std::endl;

    // 4. promise-future 通道
    std::cout << "[4] promise-future 通道:\n";
    {
        std::promise<int> prom;
        std::future<int> fut = prom.get_future();

        // 在另一个线程中设置结果
        std::thread t([&prom]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            prom.set_value(100);  // 设置结果
        });

        std::cout << "  等待 promise 的值...\n";
        int value = fut.get();  // 阻塞等待
        std::cout << "  收到 promise 的值: " << value << std::endl;

        t.join();
    }
    std::cout << std::endl;

    // 5. packaged_task
    std::cout << "[5] packaged_task 包装可调用对象:\n";
    {
        // 创建 packaged_task
        std::packaged_task<int(int, int)> task([](int a, int b) {
            return a + b;
        });

        // 获取与之关联的 future
        std::future<int> fut = task.get_future();

        // 在线程中执行任务
        std::thread t(std::move(task), 10, 20);

        std::cout << "  packaged_task 结果: " << fut.get() << std::endl;
        t.join();
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. 异常传递
    std::cout << "[1] 通过 future 传递异常:\n";
    {
        // async 中的异常会被捕获并传递给 future
        auto fut = std::async(std::launch::async, []() -> int {
            throw std::runtime_error("异步计算出错！");
            return 0;
        });

        try {
            int result = fut.get();  // 重新抛出异常
            (void)result;
        } catch (const std::runtime_error& e) {
            std::cout << "  捕获异步异常: " << e.what() << std::endl;
        }

        // promise 也可以设置异常
        std::promise<int> prom;
        auto fut2 = prom.get_future();
        prom.set_exception(std::make_exception_ptr(
            std::logic_error("promise设置的异常")));

        try {
            fut2.get();
        } catch (const std::logic_error& e) {
            std::cout << "  捕获promise异常: " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;

    // 2. future 状态查询
    std::cout << "[2] future 状态查询:\n";
    {
        auto fut = std::async(std::launch::async, []() {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return 42;
        });

        // wait_for 检查是否完成
        auto status = fut.wait_for(std::chrono::milliseconds(50));
        if (status == std::future_status::timeout) {
            std::cout << "  50ms后: 还在计算中...\n";
        } else if (status == std::future_status::ready) {
            std::cout << "  50ms后: 已完成\n";
        } else if (status == std::future_status::deferred) {
            std::cout << "  延迟执行\n";
        }

        // 等待完成
        fut.wait();
        std::cout << "  结果: " << fut.get() << std::endl;
    }
    std::cout << std::endl;

    // 3. shared_future
    std::cout << "[3] shared_future (多次获取结果):\n";
    {
        auto fut = std::async(std::launch::async, []() {
            return 42;
        });

        // 转换为 shared_future
        std::shared_future<int> shared = fut.share();

        // 多个线程都可以 get()
        std::thread t1([shared]() {
            std::lock_guard<std::mutex> lock(g_cout_mtx);
            std::cout << "  线程1 获取: " << shared.get() << std::endl;
        });

        std::thread t2([shared]() {
            std::lock_guard<std::mutex> lock(g_cout_mtx);
            std::cout << "  线程2 获取: " << shared.get() << std::endl;
        });

        // 主线程也可以获取
        std::cout << "  主线程获取: " << shared.get() << std::endl;

        t1.join();
        t2.join();
    }
    std::cout << std::endl;

    // 4. 并行计算示例
    std::cout << "[4] 并行计算 - 分治求和:\n";
    {
        std::vector<int> data(1000);
        std::iota(data.begin(), data.end(), 1);  // 1, 2, 3, ..., 1000

        auto partial_sum = [](std::vector<int>::iterator begin,
                              std::vector<int>::iterator end) -> long long {
            return std::accumulate(begin, end, 0LL);
        };

        auto mid = data.begin() + data.size() / 2;

        // 两个异步任务并行计算
        auto fut1 = std::async(std::launch::async, partial_sum, data.begin(), mid);
        auto fut2 = std::async(std::launch::async, partial_sum, mid, data.end());

        long long total = fut1.get() + fut2.get();
        std::cout << "  1+2+...+1000 = " << total << std::endl;

        // 验证
        long long expected = 1000LL * 1001 / 2;
        std::cout << "  期望结果: " << expected << std::endl;
        std::cout << "  正确: " << (total == expected ? "是" : "否") << std::endl;
    }
    std::cout << std::endl;

    // 5. packaged_task 在任务队列中使用
    std::cout << "[5] packaged_task 任务队列:\n";
    {
        std::vector<std::packaged_task<int()>> tasks;
        std::vector<std::future<int>> futures;

        // 添加任务到队列
        for (int i = 0; i < 5; ++i) {
            std::packaged_task<int()> task([i]() {
                return i * i;
            });
            futures.push_back(task.get_future());
            tasks.push_back(std::move(task));
        }

        // 执行所有任务（可以在不同线程中）
        std::vector<std::thread> threads;
        for (auto& task : tasks) {
            threads.emplace_back(std::move(task));
        }

        // 收集结果
        std::cout << "  任务队列结果: ";
        for (auto& f : futures) {
            std::cout << f.get() << " ";
        }
        std::cout << std::endl;

        for (auto& t : threads) t.join();
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. future::get() 只能调用一次
    std::cout << "[1] future::get() 只能调用一次:\n";
    std::cout << "  // auto fut = std::async(...);\n";
    std::cout << "  // fut.get();  // 正确\n";
    std::cout << "  // fut.get();  // 未定义行为！future已无效\n";
    std::cout << "  // 需要多次获取结果 -> 使用 shared_future\n\n";

    // 2. async 返回的 future 析构时会阻塞
    std::cout << "[2] async 返回的 future 析构时会阻塞等待:\n";
    {
        auto start = std::chrono::steady_clock::now();
        {
            // 不保存 future，临时对象立即析构
            std::async(std::launch::async, []() {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            });
            // 注意: 析构时会阻塞等待！
        }
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start).count();
        std::cout << "  未保存future, 析构阻塞了: " << elapsed << " ms\n";
        std::cout << "  教训: 始终保存 async 返回的 future\n";
    }
    std::cout << std::endl;

    // 3. promise 必须设置值
    std::cout << "[3] promise 必须设置值或异常:\n";
    std::cout << "  如果 promise 析构时既没有 set_value 也没有 set_exception，\n";
    std::cout << "  future::get() 会抛出 std::future_error\n";
    {
        try {
            std::future<int> fut;
            {
                std::promise<int> prom;
                fut = prom.get_future();
            } // promise 析构，未设置值

            fut.get();
        } catch (const std::future_error& e) {
            std::cout << "  future_error: " << e.what() << std::endl;
        }
    }
    std::cout << std::endl;

    // 4. deferred 策略的陷阱
    std::cout << "[4] deferred 策略不是真正的异步:\n";
    std::cout << "  // auto f = std::async(std::launch::deferred, fn);\n";
    std::cout << "  // // fn 不会在新线程中执行！\n";
    std::cout << "  // f.get();  // 在当前线程执行 fn\n";
    std::cout << "  // wait_for 总是返回 deferred，无法判断是否完成\n";
    std::cout << "  // 需要真正异步时，明确指定 std::launch::async\n\n";

    // 5. packaged_task 不可拷贝
    std::cout << "[5] packaged_task 不可拷贝，只能移动:\n";
    std::cout << "  // std::packaged_task<int()> t1(fn);\n";
    std::cout << "  // auto t2 = t1;           // 编译错误！\n";
    std::cout << "  // auto t2 = std::move(t1); // 正确\n";
    std::cout << "  // std::thread th(std::move(t1));  // 必须 move 进 thread\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== future / promise / async / packaged_task - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: future、promise、async、packaged_task](https://en.cppreference.com/w/cpp/thread/future)
