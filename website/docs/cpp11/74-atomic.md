---
sidebar_position: 75
title: "原子操作 (std::atomic) 与内存序"
description: "C++11 原子操作 (std::atomic) 与内存序 教学示例"
tags: [cpp11, atomic]
---

# 原子操作 (std::atomic) 与内存序

C++11 标准引入的 原子操作 (std::atomic) 与内存序 特性完整教学示例。

## 完整源码

```cpp title="cpp11/74_atomic.cpp"
/**
 * ============================================================
 * 特性名称: 原子操作 (std::atomic) 与内存序
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: std::atomic 提供原子操作，保证对变量的读写操作在多线程中是不可分割的，
 *           不需要额外的锁保护。内存序(memory order)控制原子操作的可见性和排序。
 *
 * [为什么引入]: 对于简单的标志位和计数器等场景，使用mutex开销太大。
 *               原子操作在硬件层面保证操作的原子性，是无锁编程的基础。
 *
 * [关键要点]:
 *   - std::atomic<T> 对类型T的操作是原子的（不可被中断）
 *   - 支持 load、store、exchange、compare_exchange 等操作
 *   - 内存序控制操作的可见性：relaxed、acquire、release、seq_cst等
 *   - 对整数类型还支持 fetch_add、fetch_sub 等原子算术操作
 *   - 默认内存序为 memory_order_seq_cst（最严格，最安全）
 * ============================================================
 */

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include <mutex>

std::mutex g_cout_mtx;

void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(g_cout_mtx);
    std::cout << msg << std::endl;
}

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. 原子计数器
    std::cout << "[1] 原子计数器 (对比非原子):\n";
    {
        const int N = 100000;

        // 非原子：可能出错
        int normal_counter = 0;
        auto normal_inc = [&normal_counter]() {
            for (int i = 0; i < N; ++i) ++normal_counter;
        };

        std::thread t1(normal_inc);
        std::thread t2(normal_inc);
        t1.join(); t2.join();
        std::cout << "  非原子计数器 (期望 " << N * 2 << "): "
                  << normal_counter << std::endl;

        // 原子：保证正确
        std::atomic<int> atomic_counter(0);
        auto atomic_inc = [&atomic_counter]() {
            for (int i = 0; i < N; ++i) ++atomic_counter;
        };

        std::thread t3(atomic_inc);
        std::thread t4(atomic_inc);
        t3.join(); t4.join();
        std::cout << "  原子计数器   (期望 " << N * 2 << "): "
                  << atomic_counter.load() << std::endl;
    }
    std::cout << std::endl;

    // 2. load 和 store
    std::cout << "[2] load() 和 store():\n";
    {
        std::atomic<int> value(42);

        // 读取
        int v = value.load();
        std::cout << "  load(): " << v << std::endl;

        // 写入
        value.store(100);
        std::cout << "  store(100), load(): " << value.load() << std::endl;

        // 隐式转换也可以（使用默认内存序）
        int v2 = value;  // 等价于 value.load()
        value = 200;      // 等价于 value.store(200)
        std::cout << "  隐式赋值200, 读取: " << v2 << " -> " << value.load() << std::endl;
    }
    std::cout << std::endl;

    // 3. exchange
    std::cout << "[3] exchange (交换):\n";
    {
        std::atomic<int> value(10);

        // exchange: 设置新值，返回旧值
        int old = value.exchange(20);
        std::cout << "  exchange(20): 旧值=" << old
                  << ", 新值=" << value.load() << std::endl;
    }
    std::cout << std::endl;

    // 4. 原子算术操作
    std::cout << "[4] 原子算术操作:\n";
    {
        std::atomic<int> counter(100);

        // fetch_add: 原子加，返回旧值
        int old_val = counter.fetch_add(10);
        std::cout << "  fetch_add(10): 旧值=" << old_val
                  << ", 新值=" << counter.load() << std::endl;

        // fetch_sub: 原子减，返回旧值
        old_val = counter.fetch_sub(5);
        std::cout << "  fetch_sub(5):  旧值=" << old_val
                  << ", 新值=" << counter.load() << std::endl;

        // 运算符也是原子的
        counter += 3;
        counter -= 1;
        ++counter;
        --counter;
        std::cout << "  +=3, -=1, ++, --: " << counter.load() << std::endl;

        // 位操作
        std::atomic<int> flags(0xFF);
        flags.fetch_and(0x0F);
        std::cout << "  0xFF fetch_and(0x0F): 0x"
                  << std::hex << flags.load() << std::dec << std::endl;

        flags.fetch_or(0xF0);
        std::cout << "  fetch_or(0xF0): 0x"
                  << std::hex << flags.load() << std::dec << std::endl;
    }
    std::cout << std::endl;

    // 5. atomic_flag（最简单的原子类型）
    std::cout << "[5] atomic_flag (自旋锁基础):\n";
    {
        std::atomic_flag flag = ATOMIC_FLAG_INIT;

        // test_and_set: 设置标志并返回之前的值
        bool was_set = flag.test_and_set();
        std::cout << "  第一次 test_and_set: " << (was_set ? "已设置" : "未设置") << std::endl;

        was_set = flag.test_and_set();
        std::cout << "  第二次 test_and_set: " << (was_set ? "已设置" : "未设置") << std::endl;

        flag.clear();
        was_set = flag.test_and_set();
        std::cout << "  clear后 test_and_set: " << (was_set ? "已设置" : "未设置") << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. compare_exchange (CAS操作)
    std::cout << "[1] compare_exchange_strong (CAS):\n";
    {
        std::atomic<int> value(10);

        int expected = 10;
        bool success = value.compare_exchange_strong(expected, 20);
        // 如果 value == expected，则 value = 20，返回 true
        // 否则 expected = value 的当前值，返回 false

        std::cout << "  CAS(期望10, 设为20): "
                  << (success ? "成功" : "失败")
                  << ", value=" << value.load() << std::endl;

        expected = 10;  // 故意设错
        success = value.compare_exchange_strong(expected, 30);
        std::cout << "  CAS(期望10, 设为30): "
                  << (success ? "成功" : "失败")
                  << ", value=" << value.load()
                  << ", expected被更新为=" << expected << std::endl;
    }
    std::cout << std::endl;

    // 2. 用CAS实现无锁栈的push操作思路
    std::cout << "[2] CAS 实现无锁操作 (原子最大值更新):\n";
    {
        std::atomic<int> max_val(0);

        auto update_max = [&max_val](int new_val) {
            int current = max_val.load();
            while (new_val > current &&
                   !max_val.compare_exchange_weak(current, new_val)) {
                // compare_exchange_weak 可能虚假失败
                // 循环重试，current 会被自动更新为最新值
            }
        };

        // 多线程并发更新最大值
        std::vector<std::thread> threads;
        for (int i = 0; i < 10; ++i) {
            threads.emplace_back(update_max, i * 7 % 50);
        }
        for (auto& t : threads) t.join();

        std::cout << "  无锁最大值: " << max_val.load() << std::endl;
    }
    std::cout << std::endl;

    // 3. 自旋锁实现
    std::cout << "[3] 基于 atomic_flag 的自旋锁:\n";
    {
        class SpinLock {
            std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
        public:
            void lock() {
                while (flag_.test_and_set(std::memory_order_acquire)) {
                    // 自旋等待
                    std::this_thread::yield();  // 让出CPU，减少浪费
                }
            }
            void unlock() {
                flag_.clear(std::memory_order_release);
            }
        };

        SpinLock spin;
        int counter = 0;
        const int N = 10000;

        auto worker = [&]() {
            for (int i = 0; i < N; ++i) {
                spin.lock();
                ++counter;
                spin.unlock();
            }
        };

        std::thread t1(worker);
        std::thread t2(worker);
        t1.join(); t2.join();

        std::cout << "  自旋锁计数器 (期望 " << N * 2 << "): " << counter << std::endl;
    }
    std::cout << std::endl;

    // 4. 内存序简要说明
    std::cout << "[4] 内存序 (Memory Order) 简要说明:\n";
    {
        std::cout << "  memory_order_relaxed:\n";
        std::cout << "    最弱的保证，只保证原子性，不保证顺序\n";
        std::cout << "    适用于：纯计数器，不依赖于其他操作的顺序\n\n";

        std::cout << "  memory_order_acquire (用于load):\n";
        std::cout << "    当前线程中，此操作之后的读写不会被重排到此操作之前\n";
        std::cout << "    \"获取\"其他线程release之前的修改可见性\n\n";

        std::cout << "  memory_order_release (用于store):\n";
        std::cout << "    当前线程中，此操作之前的读写不会被重排到此操作之后\n";
        std::cout << "    \"发布\"此操作之前的修改，使acquire方可见\n\n";

        std::cout << "  memory_order_seq_cst (默认):\n";
        std::cout << "    最强的保证，全局一致顺序\n";
        std::cout << "    所有线程看到一致的操作顺序\n";

        // 演示 relaxed
        std::atomic<int> relaxed_counter(0);
        const int N = 50000;

        auto inc = [&]() {
            for (int i = 0; i < N; ++i) {
                // relaxed 足够用于计数器
                relaxed_counter.fetch_add(1, std::memory_order_relaxed);
            }
        };

        std::thread t1(inc);
        std::thread t2(inc);
        t1.join(); t2.join();

        std::cout << "\n  relaxed 计数器 (期望 " << N * 2 << "): "
                  << relaxed_counter.load(std::memory_order_relaxed) << std::endl;
    }
    std::cout << std::endl;

    // 5. acquire-release 配对示例
    std::cout << "[5] acquire-release 同步示例:\n";
    {
        std::atomic<bool> data_ready(false);
        int data = 0;

        // 生产者线程
        std::thread producer([&]() {
            data = 42;  // 普通写入
            data_ready.store(true, std::memory_order_release);
            // release 保证 data=42 在 store 之前完成
        });

        // 消费者线程
        std::thread consumer([&]() {
            while (!data_ready.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }
            // acquire 保证能看到 release 之前的所有写入
            safe_print("  consumer 读到 data = " + std::to_string(data));
        });

        producer.join();
        consumer.join();
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. atomic 不保证复合操作的原子性
    std::cout << "[1] atomic 不保证复合操作的原子性:\n";
    std::cout << "  // std::atomic<int> x(0);\n";
    std::cout << "  // if (x == 0) { x = 1; }  // 不是原子的！\n";
    std::cout << "  // 读和写是两个独立的原子操作，中间可能被插入\n";
    std::cout << "  // 正确做法: 使用 compare_exchange\n";
    {
        std::atomic<int> x(0);
        int expected = 0;
        x.compare_exchange_strong(expected, 1);  // 原子的 if-then-set
        std::cout << "  CAS正确实现: x = " << x.load() << std::endl;
    }
    std::cout << std::endl;

    // 2. compare_exchange_weak vs strong
    std::cout << "[2] compare_exchange_weak vs strong:\n";
    std::cout << "  weak: 可能虚假失败，必须在循环中使用\n";
    std::cout << "  strong: 不会虚假失败\n";
    std::cout << "  weak 在循环中使用时可能更高效（某些平台）\n";
    std::cout << "  单次比较用 strong，循环中用 weak\n\n";

    // 3. atomic<T> 的 T 必须满足条件
    std::cout << "[3] atomic<T> 的类型要求:\n";
    std::cout << "  T 必须是 TriviallyCopyable 类型\n";
    std::cout << "  可以: int, long, double, 指针, POD struct\n";
    std::cout << "  不可以: std::string, std::vector 等\n\n";

    // 4. is_lock_free 检查
    std::cout << "[4] 并非所有 atomic 都是无锁的:\n";
    {
        std::cout << "  atomic<int> 无锁: "
                  << (std::atomic<int>().is_lock_free() ? "是" : "否") << std::endl;
        std::cout << "  atomic<long long> 无锁: "
                  << (std::atomic<long long>().is_lock_free() ? "是" : "否") << std::endl;

        struct BigStruct { int data[100]; };
        std::cout << "  atomic<BigStruct> 无锁: "
                  << (std::atomic<BigStruct>().is_lock_free() ? "是" : "否")
                  << " (大结构体通常用锁实现)" << std::endl;
    }
    std::cout << std::endl;

    // 5. 不要滥用 relaxed
    std::cout << "[5] 谨慎使用 memory_order_relaxed:\n";
    std::cout << "  relaxed 只保证原子性，不保证可见性和顺序\n";
    std::cout << "  仅适用于不需要同步的场景（如纯计数器）\n";
    std::cout << "  如果不确定，使用默认的 seq_cst（最安全）\n";
    std::cout << "  内存序选择错误可能导致极难调试的并发bug\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 原子操作 (std::atomic) 与内存序 - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: 原子操作 (std::atomic) 与内存序](https://en.cppreference.com/w/cpp/atomic/atomic)
