/**
 * ============================================================
 * 特性名称: 条件变量 (condition_variable)
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: std::condition_variable 是线程同步原语，允许一个或多个线程
 *           等待某个条件成立后再继续执行，通常配合 mutex 和 unique_lock 使用。
 *
 * [为什么引入]: 仅靠互斥锁无法高效实现"等待某个条件"的模式。
 *               忙等待（轮询）浪费CPU资源，条件变量提供了高效的等待/通知机制。
 *
 * [关键要点]:
 *   - wait(): 释放锁并阻塞，被通知后重新获取锁
 *   - notify_one(): 唤醒一个等待线程
 *   - notify_all(): 唤醒所有等待线程
 *   - 必须配合 std::unique_lock<std::mutex> 使用
 *   - 需要处理虚假唤醒（spurious wakeup），使用带谓词的 wait
 * ============================================================
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <string>
#include <chrono>
#include <functional>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. 基本的等待/通知
    std::cout << "[1] 基本的 wait / notify_one:\n";
    {
        std::mutex mtx;
        std::condition_variable cv;
        bool ready = false;

        // 等待线程
        std::thread waiter([&]() {
            std::unique_lock<std::mutex> lock(mtx);
            std::cout << "  等待线程: 等待通知...\n";

            // 带谓词的 wait，防止虚假唤醒
            cv.wait(lock, [&ready]() { return ready; });

            std::cout << "  等待线程: 收到通知，继续执行！\n";
        });

        // 通知线程
        std::thread notifier([&]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            {
                std::lock_guard<std::mutex> lock(mtx);
                ready = true;
                std::cout << "  通知线程: 设置条件并发送通知\n";
            }
            cv.notify_one();
        });

        waiter.join();
        notifier.join();
    }
    std::cout << std::endl;

    // 2. 虚假唤醒处理
    std::cout << "[2] 虚假唤醒 (Spurious Wakeup) 处理:\n";
    {
        std::cout << "  虚假唤醒: 线程可能在没有notify时被唤醒\n";
        std::cout << "  错误写法:\n";
        std::cout << "    cv.wait(lock);  // 可能虚假唤醒！\n";
        std::cout << "  正确写法 (带谓词):\n";
        std::cout << "    cv.wait(lock, []{ return condition; });\n";
        std::cout << "  等价于:\n";
        std::cout << "    while (!condition) { cv.wait(lock); }\n";
    }
    std::cout << std::endl;

    // 3. notify_all 唤醒所有线程
    std::cout << "[3] notify_all (唤醒所有等待线程):\n";
    {
        std::mutex mtx;
        std::condition_variable cv;
        bool start = false;

        auto worker = [&](int id) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&start]() { return start; });
            std::cout << "  工作线程 " << id << " 开始工作\n";
        };

        // 创建多个等待线程
        std::vector<std::thread> threads;
        for (int i = 0; i < 4; ++i) {
            threads.emplace_back(worker, i);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        {
            std::lock_guard<std::mutex> lock(mtx);
            start = true;
            std::cout << "  主线程: 发送 notify_all\n";
        }
        cv.notify_all();  // 唤醒所有线程

        for (auto& t : threads) t.join();
    }
    std::cout << std::endl;

    // 4. wait_for 超时等待
    std::cout << "[4] wait_for (超时等待):\n";
    {
        std::mutex mtx;
        std::condition_variable cv;
        bool data_ready = false;

        std::thread t([&]() {
            std::unique_lock<std::mutex> lock(mtx);

            // 等待最多200ms
            bool success = cv.wait_for(lock, std::chrono::milliseconds(200),
                                        [&]() { return data_ready; });

            if (success) {
                std::cout << "  等待线程: 在超时前收到通知\n";
            } else {
                std::cout << "  等待线程: 超时！条件未满足\n";
            }
        });

        // 不发送通知，让其超时
        t.join();
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. 生产者-消费者模式
    std::cout << "[1] 生产者-消费者模式:\n";
    {
        std::queue<int> buffer;
        const int MAX_BUFFER = 5;
        std::mutex mtx;
        std::condition_variable cv_producer;
        std::condition_variable cv_consumer;
        bool done = false;

        // 生产者
        std::thread producer([&]() {
            for (int i = 1; i <= 10; ++i) {
                std::unique_lock<std::mutex> lock(mtx);

                // 等待缓冲区有空间
                cv_producer.wait(lock, [&]() {
                    return buffer.size() < MAX_BUFFER;
                });

                buffer.push(i);
                std::cout << "  [生产] 产品 " << i
                          << " (缓冲区: " << buffer.size() << "/" << MAX_BUFFER << ")\n";

                cv_consumer.notify_one();  // 通知消费者

                lock.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }

            std::lock_guard<std::mutex> lock(mtx);
            done = true;
            cv_consumer.notify_all();
        });

        // 消费者
        std::thread consumer([&]() {
            while (true) {
                std::unique_lock<std::mutex> lock(mtx);

                cv_consumer.wait(lock, [&]() {
                    return !buffer.empty() || done;
                });

                while (!buffer.empty()) {
                    int item = buffer.front();
                    buffer.pop();
                    std::cout << "  [消费] 产品 " << item
                              << " (缓冲区: " << buffer.size() << "/" << MAX_BUFFER << ")\n";
                    cv_producer.notify_one();  // 通知生产者有空间了
                }

                if (done && buffer.empty()) break;
            }
        });

        producer.join();
        consumer.join();
        std::cout << "  生产者-消费者完成\n";
    }
    std::cout << std::endl;

    // 2. 多生产者-多消费者
    std::cout << "[2] 多生产者-多消费者队列:\n";
    {
        class ThreadSafeQueue {
            std::queue<int> queue_;
            std::mutex mtx_;
            std::condition_variable cv_;

        public:
            void push(int value) {
                {
                    std::lock_guard<std::mutex> lock(mtx_);
                    queue_.push(value);
                }
                cv_.notify_one();
            }

            bool try_pop(int& value, std::chrono::milliseconds timeout) {
                std::unique_lock<std::mutex> lock(mtx_);
                if (!cv_.wait_for(lock, timeout, [this]() { return !queue_.empty(); })) {
                    return false;
                }
                value = queue_.front();
                queue_.pop();
                return true;
            }
        };

        ThreadSafeQueue queue;
        std::mutex cout_mtx;

        // 2个生产者
        auto producer = [&](int id) {
            for (int i = 0; i < 5; ++i) {
                int item = id * 100 + i;
                queue.push(item);
                std::lock_guard<std::mutex> lock(cout_mtx);
                std::cout << "  P" << id << " 生产: " << item << "\n";
            }
        };

        // 3个消费者
        auto consumer = [&](int id) {
            int value;
            int count = 0;
            while (queue.try_pop(value, std::chrono::milliseconds(200))) {
                std::lock_guard<std::mutex> lock(cout_mtx);
                std::cout << "  C" << id << " 消费: " << value << "\n";
                ++count;
            }
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << "  C" << id << " 共消费 " << count << " 个\n";
        };

        std::thread p1(producer, 1);
        std::thread p2(producer, 2);

        // 等生产者先开始生产
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        std::thread c1(consumer, 1);
        std::thread c2(consumer, 2);

        p1.join();
        p2.join();
        c1.join();
        c2.join();
    }
    std::cout << std::endl;

    // 3. 事件通知（一次性）
    std::cout << "[3] 一次性事件通知:\n";
    {
        std::mutex mtx;
        std::condition_variable cv;
        std::string result;
        bool has_result = false;

        // 异步计算线程
        std::thread worker([&]() {
            // 模拟耗时计算
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            {
                std::lock_guard<std::mutex> lock(mtx);
                result = "计算完成：42";
                has_result = true;
            }
            cv.notify_one();
        });

        // 主线程等待结果
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&]() { return has_result; });
            std::cout << "  收到结果: " << result << std::endl;
        }

        worker.join();
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. 必须用 unique_lock
    std::cout << "[1] condition_variable 必须配合 unique_lock:\n";
    std::cout << "  // std::lock_guard<std::mutex> lock(mtx);\n";
    std::cout << "  // cv.wait(lock);  // 编译错误！需要 unique_lock\n";
    std::cout << "  // 原因: wait() 需要释放和重新获取锁，lock_guard不支持\n\n";

    // 2. notify 前不需要持锁（但修改条件需要）
    std::cout << "[2] notify 时是否需要持锁:\n";
    std::cout << "  // 修改共享条件变量 -> 必须持锁\n";
    std::cout << "  // 调用 notify_one/all -> 可以不持锁（甚至推荐不持锁）\n";
    std::cout << "  // 推荐模式:\n";
    std::cout << "  // {\n";
    std::cout << "  //   std::lock_guard<std::mutex> lock(mtx);\n";
    std::cout << "  //   condition = true;  // 持锁修改条件\n";
    std::cout << "  // }  // 先解锁\n";
    std::cout << "  // cv.notify_one();  // 再通知（减少无谓的锁竞争）\n\n";

    // 3. 先通知后等待
    std::cout << "[3] 通知可能在等待之前到达（丢失通知）:\n";
    std::cout << "  如果 notify 发生在 wait 之前，通知会丢失！\n";
    std::cout << "  解决方案: 使用条件变量（bool flag）记录状态\n";
    {
        std::mutex mtx;
        std::condition_variable cv;
        bool notified = false;

        // 先通知
        {
            std::lock_guard<std::mutex> lock(mtx);
            notified = true;
        }
        cv.notify_one();

        // 后等待 —— 由于有 notified 标志，不会永远阻塞
        std::thread t([&]() {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&]() { return notified; });  // 立即返回
            std::cout << "  即使通知先到达，带谓词的wait也能正确处理\n";
        });
        t.join();
    }
    std::cout << std::endl;

    // 4. 虚假唤醒再强调
    std::cout << "[4] 始终使用带谓词的 wait (防虚假唤醒):\n";
    std::cout << "  // 错误:\n";
    std::cout << "  // cv.wait(lock);\n";
    std::cout << "  // if (condition) { ... }  // 可能虚假唤醒时条件不满足\n";
    std::cout << "  //\n";
    std::cout << "  // 正确:\n";
    std::cout << "  // cv.wait(lock, []{ return condition; });\n";
    std::cout << "  // 内部等价于: while (!condition) cv.wait(lock);\n\n";

    // 5. condition_variable 只能用 std::mutex
    std::cout << "[5] condition_variable 只能配合 std::mutex:\n";
    std::cout << "  // std::recursive_mutex rmtx;\n";
    std::cout << "  // std::condition_variable cv;\n";
    std::cout << "  // std::unique_lock<std::recursive_mutex> lock(rmtx);\n";
    std::cout << "  // cv.wait(lock);  // 编译错误！\n";
    std::cout << "  // 需要用 condition_variable_any 才能配合其他锁类型\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 条件变量 (condition_variable) - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
