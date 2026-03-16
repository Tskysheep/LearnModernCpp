---
sidebar_position: 72
slug: 71-barrier-latch
title: "barrier、latch 和 counting_semaphore"
description: "C++20 barrier、latch 和 counting_semaphore 教学示例"
tags: [cpp20, barrier_latch]
---

# barrier、latch 和 counting_semaphore

C++20 标准引入的 barrier、latch 和 counting_semaphore 特性完整教学示例。

## 完整源码

```cpp title="cpp20/71_barrier_latch.cpp"
/**
 * ============================================================
 * 特性名称: barrier、latch 和 counting_semaphore
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: C++20 引入了三种新的同步原语：
 *          - std::latch: 一次性同步屏障（倒计时器）
 *          - std::barrier: 可重用的同步屏障
 *          - std::counting_semaphore: 计数信号量
 *
 * [为什么引入]: 传统的 mutex + condition_variable 实现这些模式
 *             既复杂又容易出错。标准库提供现成的高层同步原语，
 *             更安全、更高效。
 *
 * [关键要点]:
 *   - latch: 一次性，线程等待计数器归零，不可重用
 *   - barrier: 多次使用，所有线程到达后才能继续，自动重置
 *   - semaphore: 控制并发访问数量，P(acquire)/V(release) 操作
 * ============================================================
 */

#include <barrier>
#include <chrono>
#include <iostream>
#include <latch>
#include <mutex>
#include <semaphore>
#include <string>
#include <thread>
#include <vector>

std::mutex cout_mutex;

void safe_print(const std::string& msg) {
    std::lock_guard lock(cout_mutex);
    std::cout << msg << "\n";
}

// ===================== basic_usage() =====================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // ---- std::latch: 一次性同步 ----
    std::cout << "-- std::latch (一次性同步) --\n";
    {
        constexpr int num_workers = 4;
        std::latch work_done(num_workers);  // 计数器初始化为 4

        std::vector<std::jthread> workers;
        for (int i = 0; i < num_workers; ++i) {
            workers.emplace_back([i, &work_done] {
                // 模拟工作
                std::this_thread::sleep_for(std::chrono::milliseconds(50 * (i + 1)));
                safe_print("  [工人" + std::to_string(i) + "] 工作完成");
                work_done.count_down();  // 计数器减 1
            });
        }

        std::cout << "  [主线程] 等待所有工人完成...\n";
        work_done.wait();  // 阻塞直到计数器归零
        std::cout << "  [主线程] 所有工人已完成!\n";
    }

    // latch 的 arrive_and_wait: 计数并等待
    std::cout << "\n-- latch::arrive_and_wait --\n";
    {
        constexpr int num_threads = 3;
        std::latch sync_point(num_threads);

        std::vector<std::jthread> threads;
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([i, &sync_point] {
                safe_print("  [线程" + std::to_string(i) + "] 准备就绪");
                sync_point.arrive_and_wait();  // 到达并等待所有线程
                safe_print("  [线程" + std::to_string(i) + "] 同时开始执行!");
            });
        }
    }

    // ---- std::counting_semaphore: 计数信号量 ----
    std::cout << "\n-- std::counting_semaphore --\n";
    {
        // 限制最多 2 个线程同时访问资源
        std::counting_semaphore<2> semaphore(2);

        auto worker = [&semaphore](int id) {
            safe_print("  [线程" + std::to_string(id) + "] 等待进入...");
            semaphore.acquire();  // P 操作：获取信号量
            safe_print("  [线程" + std::to_string(id) + "] 进入临界区 (正在使用资源)");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            safe_print("  [线程" + std::to_string(id) + "] 离开临界区");
            semaphore.release();  // V 操作：释放信号量
        };

        std::vector<std::jthread> threads;
        for (int i = 0; i < 5; ++i) {
            threads.emplace_back(worker, i);
        }
    }
}

// ===================== advanced_usage() =====================
void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // ---- std::barrier: 可重用屏障 ----
    std::cout << "-- std::barrier (可重用屏障) --\n";
    {
        constexpr int num_threads = 3;
        int phase = 0;

        // barrier 可以带完成回调：所有线程到达后执行
        std::barrier sync_point(num_threads, [&phase]() noexcept {
            ++phase;
            std::lock_guard lock(cout_mutex);
            std::cout << "  === 第 " << phase << " 阶段完成，所有线程到齐 ===\n";
        });

        auto worker = [&sync_point](int id) {
            for (int round = 0; round < 3; ++round) {
                // 阶段工作
                safe_print("  [线程" + std::to_string(id)
                          + "] 执行第 " + std::to_string(round + 1) + " 轮工作");
                std::this_thread::sleep_for(std::chrono::milliseconds(30 * (id + 1)));

                // 到达屏障并等待所有线程
                sync_point.arrive_and_wait();
                // 所有线程都到达后，继续下一轮
            }
        };

        std::vector<std::jthread> threads;
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(worker, i);
        }
    }

    // ---- binary_semaphore: 二元信号量（类似互斥锁） ----
    std::cout << "\n-- std::binary_semaphore --\n";
    {
        // binary_semaphore 是 counting_semaphore<1> 的别名
        std::binary_semaphore sem(1);  // 初始值 1，类似 mutex

        int shared_data = 0;

        auto increment = [&sem, &shared_data](int id) {
            for (int i = 0; i < 5; ++i) {
                sem.acquire();  // 类似 lock
                ++shared_data;
                sem.release();  // 类似 unlock
            }
            safe_print("  [线程" + std::to_string(id) + "] 完成增量操作");
        };

        {
            std::jthread t1(increment, 1);
            std::jthread t2(increment, 2);
        }

        std::cout << "  共享数据最终值: " << shared_data << " (预期: 10)\n";
    }

    // ---- 生产者-消费者模式 ----
    std::cout << "\n-- 信号量实现生产者-消费者 --\n";
    {
        constexpr int buffer_size = 3;
        std::counting_semaphore<buffer_size> empty_slots(buffer_size);  // 空位数
        std::counting_semaphore<buffer_size> full_slots(0);  // 已填充数

        std::vector<int> buffer;
        std::mutex buffer_mutex;
        bool done = false;

        // 生产者
        std::jthread producer([&](std::stop_token stoken) {
            for (int i = 1; i <= 6 && !stoken.stop_requested(); ++i) {
                empty_slots.acquire();  // 等待空位
                {
                    std::lock_guard lock(buffer_mutex);
                    buffer.push_back(i);
                    safe_print("  [生产者] 生产: " + std::to_string(i)
                              + " (缓冲区大小: " + std::to_string(buffer.size()) + ")");
                }
                full_slots.release();  // 通知有新数据
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
            done = true;
            full_slots.release();  // 额外释放，唤醒消费者退出
        });

        // 消费者
        std::jthread consumer([&] {
            while (true) {
                full_slots.acquire();  // 等待数据
                int value = 0;
                {
                    std::lock_guard lock(buffer_mutex);
                    if (buffer.empty() && done) break;
                    if (!buffer.empty()) {
                        value = buffer.front();
                        buffer.erase(buffer.begin());
                    }
                }
                if (value > 0) {
                    safe_print("  [消费者] 消费: " + std::to_string(value));
                    empty_slots.release();  // 释放空位
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(60));
            }
        });
    }

    // ---- latch 用于启动门 (start gate) ----
    std::cout << "\n-- latch 作为启动门 --\n";
    {
        std::latch start_gate(1);  // 计数器为1，由主线程控制

        auto racer = [&start_gate](int id) {
            safe_print("  [选手" + std::to_string(id) + "] 就位");
            start_gate.wait();  // 等待起跑信号
            safe_print("  [选手" + std::to_string(id) + "] 开跑!");
        };

        std::vector<std::jthread> racers;
        for (int i = 0; i < 4; ++i) {
            racers.emplace_back(racer, i);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "  [裁判] 预备...跑!\n";
        start_gate.count_down();  // 发出起跑信号
    }
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：latch 不可重用
    std::cout << "1. latch 是一次性的，不可重用:\n";
    std::cout << "   计数器归零后就不能再使用\n";
    std::cout << "   如果需要重复同步，使用 barrier\n\n";

    // 陷阱2：barrier 参与线程数固定
    std::cout << "2. barrier 的参与线程数在构造时固定:\n";
    std::cout << "   可以通过 arrive_and_drop() 减少参与者\n";
    std::cout << "   但不能增加参与者\n\n";

    // 陷阱3：信号量不与线程绑定
    std::cout << "3. semaphore 不绑定到特定线程:\n";
    std::cout << "   任何线程都可以 acquire/release\n";
    std::cout << "   与 mutex 不同（mutex 必须由锁定它的线程解锁）\n";
    std::cout << "   这既是优势也是风险（更灵活但更容易出错）\n\n";

    // 陷阱4：死锁风险
    std::cout << "4. 信号量仍然有死锁风险:\n";
    std::cout << "   如果 acquire 和 release 不匹配\n";
    std::cout << "   例如：忘记 release 或 release 次数超过 acquire\n\n";

    // 陷阱5：barrier 完成回调中不能抛异常
    std::cout << "5. barrier 的完成回调必须是 noexcept:\n";
    std::cout << "   std::barrier sync(n, []() noexcept { ... });\n";
    std::cout << "   如果回调抛出异常，程序会终止\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== barrier、latch 和 semaphore - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: barrier、latch 和 counting_semaphore](https://en.cppreference.com/w/cpp/thread/barrier)
