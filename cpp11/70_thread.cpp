/**
 * ============================================================
 * 特性名称: std::thread 线程
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: C++11在标准库中引入了 <thread> 头文件，提供了跨平台的线程创建
 *           和管理功能，无需依赖平台特定的API（如pthread、Windows线程）。
 *
 * [为什么引入]: C++03没有标准的多线程支持，开发者必须使用平台特定的线程API。
 *               std::thread 提供了统一的、可移植的线程抽象。
 *
 * [关键要点]:
 *   - std::thread 创建新线程执行可调用对象
 *   - 必须调用 join()（等待完成）或 detach()（分离）
 *   - 支持传值、传引用(std::ref)、传移动语义参数
 *   - std::this_thread 提供当前线程的相关操作
 *   - hardware_concurrency() 查询硬件支持的并发线程数
 * ============================================================
 */

#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <functional>
#include <chrono>

// 用于保护控制台输出的互斥锁
std::mutex cout_mutex;

// 线程安全的打印函数
void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << msg << std::endl;
}

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. 创建线程（使用普通函数）
    std::cout << "[1] 创建线程 (普通函数):\n";
    {
        auto worker = []() {
            safe_print("  工作线程: 我在新线程中运行！");
        };

        std::thread t(worker);
        safe_print("  主线程: 已创建工作线程");
        t.join();  // 等待线程完成
        std::cout << "  主线程: 工作线程已完成\n";
    }
    std::cout << std::endl;

    // 2. 使用 lambda 创建线程
    std::cout << "[2] 使用 lambda 创建线程:\n";
    {
        std::thread t([]() {
            for (int i = 0; i < 3; ++i) {
                safe_print("  lambda线程: 计数 " + std::to_string(i));
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        });

        t.join();
        std::cout << "  lambda线程完成\n";
    }
    std::cout << std::endl;

    // 3. 传参：按值传递
    std::cout << "[3] 传参 - 按值传递:\n";
    {
        auto printInfo = [](int id, std::string name) {
            safe_print("  线程 " + std::to_string(id) + ": 名字=" + name);
        };

        std::thread t(printInfo, 1, std::string("工作者"));
        t.join();
    }
    std::cout << std::endl;

    // 4. 传参：按引用传递（使用 std::ref）
    std::cout << "[4] 传参 - 按引用传递 (std::ref):\n";
    {
        int result = 0;

        auto compute = [](int& out) {
            out = 42;
        };

        // 必须用 std::ref，否则 thread 会拷贝参数
        std::thread t(compute, std::ref(result));
        t.join();

        std::cout << "  线程计算结果: " << result << std::endl;
    }
    std::cout << std::endl;

    // 5. join 和 detach
    std::cout << "[5] join 与 detach:\n";
    {
        // join: 阻塞等待线程完成
        std::thread t1([]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            safe_print("  t1 (join): 完成工作");
        });
        t1.join();
        std::cout << "  t1 已 join\n";

        // detach: 分离线程，让其在后台运行
        // 注意：detach后线程独立运行，主线程不再管理
        std::thread t2([]() {
            // detach 的线程应该尽快完成，或者通过其他机制同步
            // 这里只是演示概念
        });
        t2.detach();
        std::cout << "  t2 已 detach (在后台运行)\n";

        // 给 detach 的线程一点时间完成
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. 线程 ID
    std::cout << "[1] 线程 ID:\n";
    {
        std::cout << "  主线程 ID: " << std::this_thread::get_id() << std::endl;

        std::thread t([]() {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "  工作线程 ID: " << std::this_thread::get_id() << std::endl;
        });

        std::cout << "  t.get_id(): " << t.get_id() << std::endl;
        t.join();
    }
    std::cout << std::endl;

    // 2. 硬件并发数
    std::cout << "[2] 硬件并发数:\n";
    {
        unsigned int n = std::thread::hardware_concurrency();
        std::cout << "  hardware_concurrency() = " << n << std::endl;
        std::cout << "  (返回0表示无法确定)\n";
    }
    std::cout << std::endl;

    // 3. 传参：移动语义
    std::cout << "[3] 传参 - 移动语义:\n";
    {
        auto processData = [](std::vector<int> data) {
            int sum = 0;
            for (int v : data) sum += v;
            safe_print("  移动后数据之和: " + std::to_string(sum));
        };

        std::vector<int> data = {1, 2, 3, 4, 5};
        std::cout << "  移动前 data.size() = " << data.size() << std::endl;

        std::thread t(processData, std::move(data));
        t.join();

        std::cout << "  移动后 data.size() = " << data.size()
                  << " (数据已移走)" << std::endl;
    }
    std::cout << std::endl;

    // 4. 多线程并行计算
    std::cout << "[4] 多线程并行计算:\n";
    {
        const int NUM_THREADS = 4;
        const int N = 1000000;
        std::vector<long long> partial_sums(NUM_THREADS, 0);

        auto compute_partial = [&](int thread_id) {
            int start = thread_id * (N / NUM_THREADS);
            int end = (thread_id + 1) * (N / NUM_THREADS);
            long long sum = 0;
            for (int i = start; i < end; ++i) {
                sum += i;
            }
            partial_sums[thread_id] = sum;
        };

        std::vector<std::thread> threads;
        auto start_time = std::chrono::steady_clock::now();

        for (int i = 0; i < NUM_THREADS; ++i) {
            threads.emplace_back(compute_partial, i);
        }
        for (auto& t : threads) {
            t.join();
        }

        auto end_time = std::chrono::steady_clock::now();

        long long total = 0;
        for (long long s : partial_sums) total += s;

        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time).count();

        std::cout << "  " << NUM_THREADS << " 个线程计算 0~"
                  << N-1 << " 的和: " << total << std::endl;
        std::cout << "  耗时: " << elapsed << " 微秒" << std::endl;
    }
    std::cout << std::endl;

    // 5. 线程移动（thread 不可拷贝但可移动）
    std::cout << "[5] 线程移动:\n";
    {
        std::thread t1([]() {
            safe_print("  t1 线程执行");
        });

        // thread 不可拷贝
        // std::thread t2 = t1;  // 编译错误！

        // 但可以移动
        std::thread t2 = std::move(t1);
        std::cout << "  移动后 t1.joinable(): "
                  << (t1.joinable() ? "是" : "否") << std::endl;
        std::cout << "  移动后 t2.joinable(): "
                  << (t2.joinable() ? "是" : "否") << std::endl;

        t2.join();
    }
    std::cout << std::endl;

    // 6. this_thread 工具
    std::cout << "[6] std::this_thread 工具:\n";
    {
        std::cout << "  sleep_for: 暂停指定时长" << std::endl;
        std::cout << "  sleep_until: 暂停到指定时间点" << std::endl;
        std::cout << "  yield: 让出CPU时间片" << std::endl;
        std::cout << "  get_id: 获取当前线程ID" << std::endl;

        auto start = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto end = std::chrono::steady_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "  sleep_for(100ms) 实际: " << ms << " ms" << std::endl;
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. 必须 join 或 detach
    std::cout << "[1] 线程必须 join 或 detach，否则程序终止:\n";
    std::cout << "  // std::thread t(fn);\n";
    std::cout << "  // // 忘记 join/detach\n";
    std::cout << "  // } // 析构时调用 std::terminate()!\n";
    std::cout << "  正确做法: 始终在作用域结束前 join() 或 detach()\n\n";

    // 2. joinable 检查
    std::cout << "[2] 对非 joinable 线程调用 join/detach 会崩溃:\n";
    {
        std::thread t([]() {});
        t.join();
        std::cout << "  join后 joinable(): "
                  << (t.joinable() ? "是" : "否") << std::endl;
        std::cout << "  // t.join();  // 再次调用会 std::terminate()!\n";
        std::cout << "  正确: if (t.joinable()) t.join();\n";
    }
    std::cout << std::endl;

    // 3. 线程参数默认按值拷贝
    std::cout << "[3] thread 参数默认按值拷贝:\n";
    {
        int x = 0;
        std::thread t([](int val) {
            val = 100;  // 修改的是副本
        }, x);
        t.join();
        std::cout << "  按值传递: x = " << x << " (未改变)" << std::endl;

        std::thread t2([](int& val) {
            val = 100;
        }, std::ref(x));
        t2.join();
        std::cout << "  std::ref传递: x = " << x << " (已改变)" << std::endl;
    }
    std::cout << std::endl;

    // 4. 异常安全
    std::cout << "[4] 异常安全（RAII包装线程）:\n";
    std::cout << "  // 如果 join 前抛出异常，线程不会被 join\n";
    std::cout << "  // 解决方案1: try-catch\n";
    std::cout << "  // 解决方案2: RAII 包装类\n";
    {
        // 简单的 RAII 线程守卫
        struct ThreadGuard {
            std::thread& t_;
            ThreadGuard(std::thread& t) : t_(t) {}
            ~ThreadGuard() {
                if (t_.joinable()) t_.join();
            }
            ThreadGuard(const ThreadGuard&) = delete;
            ThreadGuard& operator=(const ThreadGuard&) = delete;
        };

        std::thread t([]() {
            safe_print("  RAII守卫的线程");
        });
        ThreadGuard guard(t);
        // 即使这里抛出异常，ThreadGuard 析构时也会 join
        std::cout << "  ThreadGuard 确保线程被 join\n";
    }
    std::cout << std::endl;

    // 5. detach 后的悬空引用
    std::cout << "[5] detach 后注意局部变量生命周期:\n";
    std::cout << "  // void bad_function() {\n";
    std::cout << "  //     int local = 42;\n";
    std::cout << "  //     std::thread t([&local]() {\n";
    std::cout << "  //         // local 可能已被销毁！\n";
    std::cout << "  //         std::cout << local;  // 未定义行为\n";
    std::cout << "  //     });\n";
    std::cout << "  //     t.detach();\n";
    std::cout << "  // }  // local 在这里销毁\n";
    std::cout << "  detach的线程不要引用局部变量！应按值捕获或使用shared_ptr\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::thread 线程 - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
