---
sidebar_position: 9
title: "shared_timed_mutex与shared_lock（读写锁）"
description: "C++14 shared_timed_mutex与shared_lock（读写锁） 教学示例"
tags: [cpp14, shared_timed_mutex]
---

# shared_timed_mutex与shared_lock（读写锁）

C++14 标准引入的 shared_timed_mutex与shared_lock（读写锁） 特性完整教学示例。

## 完整源码

```cpp title="cpp14/08_shared_timed_mutex.cpp"
/**
 * ============================================================
 * 特性名称: shared_timed_mutex与shared_lock（读写锁）
 * 所属标准: C++14
 * ============================================================
 *
 * [是什么]: C++14引入了std::shared_timed_mutex和std::shared_lock，
 *          实现了读写锁（共享-独占锁）模式。多个线程可以同时持有共享锁
 *          （读锁），但独占锁（写锁）会排斥所有其他锁。
 *
 * [为什么引入]: C++11只有mutex和recursive_mutex，不支持读写锁。在读多写少
 *              的场景中，所有读操作都互斥效率低下。shared_timed_mutex允许
 *              多个读者并发访问，只在写入时才独占，大幅提升并发性能。
 *
 * [关键要点]:
 *   - shared_lock<>: 以共享模式锁定，多个线程可同时持有（读锁）
 *   - unique_lock<>/lock_guard<>: 以独占模式锁定（写锁）
 *   - shared_timed_mutex支持超时操作（try_lock_for/try_lock_until）
 *   - 适用于读多写少的场景，如缓存、配置读取等
 * ============================================================
 */

#include <iostream>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include <map>
#include <sstream>
#include <atomic>

// 线程安全的输出
std::mutex cout_mutex;

template<typename... Args>
void safe_print(Args&&... args) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    // 使用initializer_list展开参数包
    using expander = int[];
    (void)expander{0, (void(std::cout << std::forward<Args>(args)), 0)...};
    std::cout << "\n";
}

// ============================================================
// 基础用法
// ============================================================

// 线程安全的配置存储（读多写少的典型场景）
class ThreadSafeConfig {
public:
    // 读操作——使用shared_lock（共享锁）
    std::string get(const std::string& key) const {
        std::shared_lock<std::shared_timed_mutex> lock(mutex_);
        auto it = data_.find(key);
        return it != data_.end() ? it->second : "";
    }

    // 写操作——使用unique_lock（独占锁）
    void set(const std::string& key, const std::string& value) {
        std::unique_lock<std::shared_timed_mutex> lock(mutex_);
        data_[key] = value;
    }

    // 读取所有键值对
    std::map<std::string, std::string> get_all() const {
        std::shared_lock<std::shared_timed_mutex> lock(mutex_);
        return data_;
    }

    size_t size() const {
        std::shared_lock<std::shared_timed_mutex> lock(mutex_);
        return data_.size();
    }

private:
    mutable std::shared_timed_mutex mutex_;
    std::map<std::string, std::string> data_;
};

void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    ThreadSafeConfig config;

    // 先写入一些配置
    config.set("host", "localhost");
    config.set("port", "8080");
    config.set("mode", "production");

    std::cout << "  初始配置:\n";
    std::cout << "    host = " << config.get("host") << "\n";
    std::cout << "    port = " << config.get("port") << "\n";
    std::cout << "    mode = " << config.get("mode") << "\n";

    // 多个读线程并发读取
    std::cout << "\n  启动多个读线程并发读取...\n";

    std::vector<std::thread> readers;
    for (int i = 0; i < 4; ++i) {
        readers.emplace_back([&config, i]() {
            // 多个读线程可以同时执行，因为它们使用shared_lock
            auto host = config.get("host");
            auto port = config.get("port");
            safe_print("    读线程", i, ": host=", host, ", port=", port);
        });
    }

    for (auto& t : readers) t.join();

    // 写线程会独占锁
    std::cout << "\n  写线程更新配置...\n";
    std::thread writer([&config]() {
        config.set("mode", "debug");
        safe_print("    写线程: mode 已更新为 debug");
    });
    writer.join();

    std::cout << "  更新后 mode = " << config.get("mode") << "\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 读写锁 vs 普通mutex的性能对比
class RWCounter {
public:
    void increment() {
        std::unique_lock<std::shared_timed_mutex> lock(mutex_);
        ++value_;
    }

    int get() const {
        std::shared_lock<std::shared_timed_mutex> lock(mutex_);
        return value_;
    }

private:
    mutable std::shared_timed_mutex mutex_;
    int value_ = 0;
};

class MutexCounter {
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++value_;
    }

    int get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }

private:
    mutable std::mutex mutex_;
    int value_ = 0;
};

// 2. 带超时的锁操作
class TimedResource {
public:
    bool try_read(std::chrono::milliseconds timeout) {
        // 尝试在指定时间内获取共享锁
        std::shared_lock<std::shared_timed_mutex> lock(mutex_, timeout);
        if (lock.owns_lock()) {
            // 成功获取锁
            return true;
        }
        return false;  // 超时
    }

    bool try_write(std::chrono::milliseconds timeout) {
        // 尝试在指定时间内获取独占锁
        std::unique_lock<std::shared_timed_mutex> lock(mutex_, timeout);
        if (lock.owns_lock()) {
            // 成功获取锁
            return true;
        }
        return false;  // 超时
    }

private:
    std::shared_timed_mutex mutex_;
};

// 3. 多读单写的缓存
class Cache {
public:
    // 多个线程可以同时查询缓存
    bool lookup(int key, std::string& value) const {
        std::shared_lock<std::shared_timed_mutex> lock(mutex_);
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            value = it->second;
            return true;
        }
        return false;
    }

    // 只有一个线程可以更新缓存
    void update(int key, const std::string& value) {
        std::unique_lock<std::shared_timed_mutex> lock(mutex_);
        cache_[key] = value;
    }

    void clear() {
        std::unique_lock<std::shared_timed_mutex> lock(mutex_);
        cache_.clear();
    }

private:
    mutable std::shared_timed_mutex mutex_;
    std::map<int, std::string> cache_;
};

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. 性能对比：读多写少场景
    std::cout << "  [读写锁 vs mutex性能对比]\n";

    const int NUM_READS = 100'000;
    const int NUM_READERS = 4;

    // shared_timed_mutex测试
    {
        RWCounter rw_counter;
        rw_counter.increment();  // 设置初始值

        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_READERS; ++i) {
            threads.emplace_back([&rw_counter]() {
                for (int j = 0; j < NUM_READS; ++j) {
                    rw_counter.get();  // 并发读
                }
            });
        }
        for (auto& t : threads) t.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "    shared_timed_mutex: " << us << " 微秒\n";
    }

    // 普通mutex测试
    {
        MutexCounter m_counter;
        m_counter.increment();

        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        for (int i = 0; i < NUM_READERS; ++i) {
            threads.emplace_back([&m_counter]() {
                for (int j = 0; j < NUM_READS; ++j) {
                    m_counter.get();  // 互斥读
                }
            });
        }
        for (auto& t : threads) t.join();

        auto end = std::chrono::high_resolution_clock::now();
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "    普通mutex:          " << us << " 微秒\n";
    }

    std::cout << "    (读多场景下shared_timed_mutex通常更快)\n";

    // 2. 超时操作
    std::cout << "\n  [超时锁操作]\n";
    TimedResource resource;
    bool got_read = resource.try_read(std::chrono::milliseconds(100));
    bool got_write = resource.try_write(std::chrono::milliseconds(100));
    std::cout << "    try_read(100ms)  = " << (got_read ? "成功" : "超时") << "\n";
    std::cout << "    try_write(100ms) = " << (got_write ? "成功" : "超时") << "\n";

    // 3. 缓存场景
    std::cout << "\n  [多读单写缓存]\n";
    Cache cache;

    // 写入一些数据
    for (int i = 0; i < 10; ++i) {
        cache.update(i, "value_" + std::to_string(i));
    }

    // 多线程并发查询
    std::atomic<int> hits{0};
    std::atomic<int> misses{0};

    std::vector<std::thread> readers;
    for (int t = 0; t < 4; ++t) {
        readers.emplace_back([&cache, &hits, &misses]() {
            for (int i = 0; i < 15; ++i) {
                std::string val;
                if (cache.lookup(i, val)) {
                    hits.fetch_add(1);
                } else {
                    misses.fetch_add(1);
                }
            }
        });
    }
    for (auto& t : readers) t.join();

    std::cout << "    缓存命中: " << hits.load() << "\n";
    std::cout << "    缓存未中: " << misses.load() << "\n";
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱1：读写锁不防止写饥饿或读饥饿
    std::cout << "  陷阱1: 读写锁的饥饿问题\n";
    std::cout << "    如果读者一直持有锁，写者可能永远等不到机会（写饥饿）\n";
    std::cout << "    反之亦然。具体行为取决于实现\n";

    // 陷阱2：shared_timed_mutex比普通mutex开销更大
    std::cout << "\n  陷阱2: 不要在写多读少场景使用读写锁\n";
    std::cout << "    shared_timed_mutex的锁操作比mutex更重\n";
    std::cout << "    只有读远多于写时才有性能优势\n";

    // 陷阱3：不要在持有共享锁时尝试获取独占锁（死锁）
    std::cout << "\n  陷阱3: 锁升级会导致死锁\n";
    std::cout << "    // 死锁示例（不要这样做！）：\n";
    std::cout << "    // shared_lock<shared_timed_mutex> read_lock(mtx);\n";
    std::cout << "    // unique_lock<shared_timed_mutex> write_lock(mtx); // 死锁!\n";
    std::cout << "    正确做法：先释放共享锁，再获取独占锁\n";

    // 陷阱4：shared_lock只能用于shared_timed_mutex（或shared_mutex in C++17）
    std::cout << "\n  陷阱4: shared_lock不能用于普通mutex\n";
    std::cout << "    std::shared_lock<std::mutex> lock(m); // 编译错误!\n";
    std::cout << "    必须搭配shared_timed_mutex使用\n";

    // 陷阱5：C++14只有shared_timed_mutex，没有shared_mutex
    std::cout << "\n  陷阱5: C++14 vs C++17\n";
    std::cout << "    C++14: shared_timed_mutex (支持超时)\n";
    std::cout << "    C++17: 增加了 shared_mutex (不支持超时，但更轻量)\n";
    std::cout << "    如果不需要超时功能，C++17的shared_mutex更合适\n";

    // 陷阱6：读写锁不能替代细粒度的并发数据结构
    std::cout << "\n  陷阱6: 读写锁不是万能的\n";
    std::cout << "    高并发场景可能需要无锁数据结构或分段锁\n";
    std::cout << "    读写锁适合中低并发的读多写少场景\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== shared_timed_mutex与shared_lock - C++14 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: shared_timed_mutex与shared_lock（读写锁）](https://en.cppreference.com/w/cpp/thread/mutex)
