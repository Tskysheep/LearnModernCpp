/**
 * ============================================================
 * 特性名称: 互斥锁与锁 (mutex、lock_guard、unique_lock)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: C++11提供了 std::mutex 互斥锁及其RAII封装 lock_guard 和
 *           unique_lock，用于保护共享数据避免数据竞争。
 *
 * [为什么引入]: 多线程并发访问共享数据时，如果不加同步，会导致数据竞争
 *               （Data Race），产生未定义行为。互斥锁提供了互斥访问的机制。
 *
 * [关键要点]:
 *   - std::mutex: 基本互斥锁，提供 lock/unlock/try_lock
 *   - std::lock_guard: RAII风格锁，构造时加锁，析构时解锁，不可转移
 *   - std::unique_lock: 更灵活的RAII锁，支持延迟锁定、手动解锁、转移所有权
 *   - std::lock(): 同时锁定多个互斥量，避免死锁
 *   - std::recursive_mutex: 允许同一线程多次锁定
 * ============================================================
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. 数据竞争问题演示
    std::cout << "[1] 无锁保护的数据竞争:\n";
    {
        int counter = 0;
        const int ITERATIONS = 100000;

        auto increment = [&counter]() {
            for (int i = 0; i < ITERATIONS; ++i) {
                ++counter;  // 数据竞争！
            }
        };

        std::thread t1(increment);
        std::thread t2(increment);
        t1.join();
        t2.join();

        std::cout << "  无锁 counter (期望 " << ITERATIONS * 2 << "): "
                  << counter << std::endl;
        std::cout << "  (由于数据竞争，结果可能不正确)\n";
    }
    std::cout << std::endl;

    // 2. mutex 解决数据竞争
    std::cout << "[2] 用 mutex 保护共享数据:\n";
    {
        int counter = 0;
        std::mutex mtx;
        const int ITERATIONS = 100000;

        auto increment = [&]() {
            for (int i = 0; i < ITERATIONS; ++i) {
                mtx.lock();
                ++counter;
                mtx.unlock();
            }
        };

        std::thread t1(increment);
        std::thread t2(increment);
        t1.join();
        t2.join();

        std::cout << "  有锁 counter (期望 " << ITERATIONS * 2 << "): "
                  << counter << std::endl;
    }
    std::cout << std::endl;

    // 3. lock_guard: RAII 自动管理锁
    std::cout << "[3] lock_guard (RAII自动管理):\n";
    {
        int counter = 0;
        std::mutex mtx;
        const int ITERATIONS = 100000;

        auto increment = [&]() {
            for (int i = 0; i < ITERATIONS; ++i) {
                std::lock_guard<std::mutex> lock(mtx);  // 构造时加锁
                ++counter;
            } // 析构时自动解锁
        };

        std::thread t1(increment);
        std::thread t2(increment);
        t1.join();
        t2.join();

        std::cout << "  lock_guard counter (期望 " << ITERATIONS * 2 << "): "
                  << counter << std::endl;
        std::cout << "  lock_guard 好处: 异常安全，不会忘记解锁\n";
    }
    std::cout << std::endl;

    // 4. unique_lock: 更灵活的锁
    std::cout << "[4] unique_lock (灵活锁定/解锁):\n";
    {
        std::mutex mtx;
        int shared_data = 0;

        auto worker = [&](int id) {
            // unique_lock 允许手动控制
            std::unique_lock<std::mutex> lock(mtx);
            shared_data += id;
            std::cout << "  线程" << id << " 持锁修改: " << shared_data << std::endl;

            lock.unlock();  // 可以提前解锁
            // 执行不需要锁的操作...
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

            lock.lock();    // 可以重新加锁
            shared_data += id * 10;
            std::cout << "  线程" << id << " 重新加锁修改: " << shared_data << std::endl;
        }; // 析构时自动解锁（如果还持有锁）

        std::thread t1(worker, 1);
        std::thread t2(worker, 2);
        t1.join();
        t2.join();
    }
    std::cout << std::endl;

    // 5. try_lock 非阻塞尝试
    std::cout << "[5] try_lock (非阻塞尝试):\n";
    {
        std::mutex mtx;

        auto worker = [&](int id) {
            for (int i = 0; i < 5; ++i) {
                if (mtx.try_lock()) {
                    std::cout << "  线程" << id << " 获得锁 (第" << i << "次)\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    mtx.unlock();
                } else {
                    std::cout << "  线程" << id << " 未获得锁，跳过 (第" << i << "次)\n";
                }
            }
        };

        std::thread t1(worker, 1);
        std::thread t2(worker, 2);
        t1.join();
        t2.join();
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. std::lock 避免死锁
    std::cout << "[1] std::lock 避免死锁:\n";
    {
        struct Account {
            std::string name;
            int balance;
            std::mutex mtx;
            Account(const std::string& n, int b) : name(n), balance(b) {}
        };

        Account alice("Alice", 1000);
        Account bob("Bob", 500);

        auto transfer = [](Account& from, Account& to, int amount) {
            // std::lock 同时锁定两个互斥量，避免死锁
            std::lock(from.mtx, to.mtx);

            // adopt_lock: 告诉 lock_guard 互斥量已经锁定，只需接管
            std::lock_guard<std::mutex> lock1(from.mtx, std::adopt_lock);
            std::lock_guard<std::mutex> lock2(to.mtx, std::adopt_lock);

            if (from.balance >= amount) {
                from.balance -= amount;
                to.balance += amount;
                std::cout << "  转账 " << amount << ": "
                          << from.name << "(" << from.balance << ") -> "
                          << to.name << "(" << to.balance << ")" << std::endl;
            }
        };

        // 两个线程同时转账（方向相反），使用 std::lock 不会死锁
        std::thread t1(transfer, std::ref(alice), std::ref(bob), 100);
        std::thread t2(transfer, std::ref(bob), std::ref(alice), 50);
        t1.join();
        t2.join();

        std::cout << "  最终: " << alice.name << "=" << alice.balance
                  << ", " << bob.name << "=" << bob.balance << std::endl;
    }
    std::cout << std::endl;

    // 2. unique_lock 与 defer_lock
    std::cout << "[2] unique_lock 与 defer_lock:\n";
    {
        std::mutex mtx1, mtx2;

        auto worker = [&]() {
            // defer_lock: 只创建 lock 对象，不立即加锁
            std::unique_lock<std::mutex> lock1(mtx1, std::defer_lock);
            std::unique_lock<std::mutex> lock2(mtx2, std::defer_lock);

            // 使用 std::lock 同时加锁
            std::lock(lock1, lock2);

            std::cout << "  同时持有两个锁" << std::endl;
        }; // 析构时自动解锁

        std::thread t(worker);
        t.join();
    }
    std::cout << std::endl;

    // 3. recursive_mutex
    std::cout << "[3] recursive_mutex (递归互斥量):\n";
    {
        std::recursive_mutex rmtx;
        int value = 0;

        // 普通 mutex 同一线程重复 lock 会死锁
        // recursive_mutex 允许同一线程多次加锁
        auto recursive_fn = [&](int depth) {
            std::lock_guard<std::recursive_mutex> lock(rmtx);
            value += depth;
            if (depth > 0) {
                // 递归调用，再次加锁（同一线程）
                std::lock_guard<std::recursive_mutex> lock2(rmtx);
                value += depth;
            }
        };

        std::thread t(recursive_fn, 3);
        t.join();
        std::cout << "  recursive_mutex: value = " << value << std::endl;
        std::cout << "  同一线程可以多次锁定 recursive_mutex\n";
    }
    std::cout << std::endl;

    // 4. 线程安全容器包装
    std::cout << "[4] 线程安全容器包装:\n";
    {
        class ThreadSafeVector {
            std::vector<int> data_;
            mutable std::mutex mtx_;

        public:
            void push_back(int val) {
                std::lock_guard<std::mutex> lock(mtx_);
                data_.push_back(val);
            }

            int size() const {
                std::lock_guard<std::mutex> lock(mtx_);
                return static_cast<int>(data_.size());
            }

            std::string to_string() const {
                std::lock_guard<std::mutex> lock(mtx_);
                std::string result;
                for (int v : data_) {
                    if (!result.empty()) result += " ";
                    result += std::to_string(v);
                }
                return result;
            }
        };

        ThreadSafeVector vec;

        auto writer = [&vec](int start) {
            for (int i = start; i < start + 5; ++i) {
                vec.push_back(i);
            }
        };

        std::thread t1(writer, 0);
        std::thread t2(writer, 100);
        t1.join();
        t2.join();

        std::cout << "  线程安全vector大小: " << vec.size() << std::endl;
        std::cout << "  内容: " << vec.to_string() << std::endl;
    }
    std::cout << std::endl;

    // 5. unique_lock 的所有权转移
    std::cout << "[5] unique_lock 的所有权转移:\n";
    {
        std::mutex mtx;

        auto get_lock = [&mtx]() -> std::unique_lock<std::mutex> {
            std::unique_lock<std::mutex> lock(mtx);
            // 准备共享数据...
            return lock;  // 移动返回锁
        };

        // 接收函数返回的锁（所有权转移）
        std::unique_lock<std::mutex> my_lock = get_lock();
        std::cout << "  锁的所有权已转移\n";
        std::cout << "  owns_lock(): " << (my_lock.owns_lock() ? "是" : "否") << std::endl;
    } // 这里自动解锁
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. 忘记解锁
    std::cout << "[1] 直接使用 mutex 容易忘记解锁:\n";
    std::cout << "  // mtx.lock();\n";
    std::cout << "  // // 如果这里抛出异常，mutex永远不会解锁！\n";
    std::cout << "  // do_something_that_might_throw();\n";
    std::cout << "  // mtx.unlock();\n";
    std::cout << "  解决: 始终使用 lock_guard 或 unique_lock\n\n";

    // 2. 死锁
    std::cout << "[2] 死锁的常见原因:\n";
    std::cout << "  - 两个线程以不同顺序锁定两个互斥量\n";
    std::cout << "  - 同一线程对非递归mutex重复加锁\n";
    std::cout << "  避免方法:\n";
    std::cout << "    1) 始终以相同顺序加锁\n";
    std::cout << "    2) 使用 std::lock() 一次性锁定多个\n";
    std::cout << "    3) 使用 recursive_mutex（如需递归）\n\n";

    // 3. 锁的粒度
    std::cout << "[3] 锁的粒度要适当:\n";
    std::cout << "  - 粒度太粗: 持锁时间长，降低并发性\n";
    std::cout << "  - 粒度太细: 频繁加锁解锁，增加开销\n";
    std::cout << "  - 原则: 只在访问共享数据时持有锁\n";
    {
        std::mutex mtx;
        auto good_practice = [&mtx]() {
            // 不需要锁的计算
            int local_result = 42 * 100;

            {
                std::lock_guard<std::mutex> lock(mtx);
                // 只在这里访问共享数据
                (void)local_result;  // 假装写入共享数据
            } // 立即解锁

            // 不需要锁的后续处理
        };
        std::thread t(good_practice);
        t.join();
        std::cout << "  正确: 最小化持锁区域\n";
    }
    std::cout << std::endl;

    // 4. lock_guard 不能手动解锁
    std::cout << "[4] lock_guard vs unique_lock:\n";
    std::cout << "  lock_guard:\n";
    std::cout << "    + 简单、轻量\n";
    std::cout << "    - 不能手动 unlock/lock\n";
    std::cout << "    - 不能移动\n";
    std::cout << "  unique_lock:\n";
    std::cout << "    + 可以手动 unlock/lock\n";
    std::cout << "    + 支持 defer_lock、try_to_lock\n";
    std::cout << "    + 可以移动所有权\n";
    std::cout << "    + 配合 condition_variable 使用\n";
    std::cout << "    - 略有额外开销\n\n";

    // 5. mutable 与 const 方法
    std::cout << "[5] const 方法中需要 mutable mutex:\n";
    std::cout << "  // class Foo {\n";
    std::cout << "  //   mutable std::mutex mtx_;  // mutable!\n";
    std::cout << "  //   int data_;\n";
    std::cout << "  //   int getData() const {\n";
    std::cout << "  //     std::lock_guard<std::mutex> lock(mtx_);  // const中也能锁\n";
    std::cout << "  //     return data_;\n";
    std::cout << "  //   }\n";
    std::cout << "  // };\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 互斥锁与锁 (mutex/lock_guard/unique_lock) - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
