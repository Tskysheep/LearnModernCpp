---
sidebar_position: 71
slug: 70-jthread
title: "jthread 和 stop_token (协作式线程取消)"
description: "C++20 jthread 和 stop_token (协作式线程取消) 教学示例"
tags: [cpp20, jthread]
---

# jthread 和 stop_token (协作式线程取消)

C++20 标准引入的 jthread 和 stop_token (协作式线程取消) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/70_jthread.cpp"
/**
 * ============================================================
 * 特性名称: jthread 和 stop_token (协作式线程取消)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: std::jthread 是 std::thread 的改进版本，
 *          支持 RAII 自动 join 和协作式取消机制（stop_token）。
 *
 * [为什么引入]: std::thread 有两个痛点：
 *             1) 析构时如果线程还在运行会调用 std::terminate
 *             2) 没有标准的线程取消机制
 *             jthread 同时解决了这两个问题。
 *
 * [关键要点]:
 *   - jthread 析构时自动请求停止并 join（RAII）
 *   - stop_token 提供协作式取消机制
 *   - stop_callback 在收到停止请求时执行回调
 *   - 线程函数可以接收 stop_token 作为第一个参数
 * ============================================================
 */

#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// 用于同步输出的互斥锁
std::mutex cout_mutex;

void safe_print(const std::string& msg) {
    std::lock_guard lock(cout_mutex);
    std::cout << msg << "\n";
}

// ===================== basic_usage() =====================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. jthread 自动 join（RAII）
    std::cout << "-- jthread 自动 join --\n";
    {
        std::jthread t([] {
            safe_print("  [jthread] 工作线程开始执行");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            safe_print("  [jthread] 工作线程执行完毕");
        });
        // 不需要显式调用 t.join()!
        // jthread 析构时会自动 join
        safe_print("  [主线程] 即将离开作用域...");
    }
    // 到这里 jthread 析构，自动等待线程结束
    std::cout << "  [主线程] 已安全退出作用域\n";

    // 对比：std::thread 必须显式 join
    std::cout << "\n-- 对比 std::thread --\n";
    {
        std::thread t([] {
            safe_print("  [thread] 工作线程执行");
        });
        t.join();  // 必须显式 join，否则析构时 terminate!
        std::cout << "  [主线程] std::thread 必须显式 join\n";
    }

    // 2. stop_token 取消机制
    std::cout << "\n-- stop_token 取消机制 --\n";
    {
        // 线程函数的第一个参数如果是 stop_token，jthread 会自动传入
        std::jthread worker([](std::stop_token stoken) {
            int count = 0;
            while (!stoken.stop_requested()) {
                safe_print("  [工作线程] 正在工作... (" + std::to_string(++count) + ")");
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            safe_print("  [工作线程] 收到停止请求，优雅退出");
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << "  [主线程] 发送停止请求\n";
        worker.request_stop();  // 请求线程停止
        // jthread 析构时也会自动 request_stop + join
    }
    std::cout << "  [主线程] 工作线程已停止\n";
}

// ===================== advanced_usage() =====================
void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // 1. stop_callback: 停止时的回调
    std::cout << "-- stop_callback --\n";
    {
        std::jthread worker([](std::stop_token stoken) {
            // 注册停止回调
            std::stop_callback callback(stoken, [] {
                safe_print("  [回调] 停止回调被触发!");
            });

            while (!stoken.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            safe_print("  [工作线程] 退出");
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "  [主线程] 请求停止...\n";
        worker.request_stop();
    }

    // 2. stop_source: 手动控制停止
    std::cout << "\n-- stop_source --\n";
    {
        std::stop_source ssource;
        std::stop_token stoken = ssource.get_token();

        std::jthread t1([stoken] {
            while (!stoken.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
            safe_print("  [线程1] 已停止");
        });

        std::jthread t2([stoken] {
            while (!stoken.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
            safe_print("  [线程2] 已停止");
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "  [主线程] 通过 stop_source 同时停止两个线程\n";
        ssource.request_stop();  // 一次请求停止所有持有 token 的线程
    }

    // 3. 获取 jthread 的 stop_source
    std::cout << "\n-- 获取 jthread 的 stop_source --\n";
    {
        std::jthread worker([](std::stop_token stoken) {
            int i = 0;
            while (!stoken.stop_requested()) {
                safe_print("  [工作] 迭代 " + std::to_string(++i));
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        });

        // 获取 jthread 内部的 stop_source
        auto source = worker.get_stop_source();
        auto token = worker.get_stop_token();

        std::cout << "  stop_possible: " << std::boolalpha << token.stop_possible() << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        source.request_stop();
        std::cout << "  stop_requested: " << token.stop_requested() << "\n";
    }

    // 4. 多个 stop_callback
    std::cout << "\n-- 多个 stop_callback --\n";
    {
        std::jthread worker([](std::stop_token stoken) {
            std::stop_callback cb1(stoken, [] {
                safe_print("  [回调1] 保存状态...");
            });
            std::stop_callback cb2(stoken, [] {
                safe_print("  [回调2] 释放资源...");
            });
            std::stop_callback cb3(stoken, [] {
                safe_print("  [回调3] 发送通知...");
            });

            while (!stoken.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            safe_print("  [工作线程] 退出");
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        worker.request_stop();
    }

    // 5. jthread 与 std::thread 的接口兼容
    std::cout << "\n-- jthread 其他接口 --\n";
    {
        std::jthread t([] {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        });
        std::cout << "  线程 ID: " << t.get_id() << "\n";
        std::cout << "  joinable: " << std::boolalpha << t.joinable() << "\n";
        std::cout << "  hardware_concurrency: " << std::jthread::hardware_concurrency() << "\n";
    }
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：stop_token 是协作式的
    std::cout << "1. stop_token 是协作式取消，线程必须主动检查:\n";
    std::cout << "   不会强制终止线程（不是 pthread_cancel）\n";
    std::cout << "   线程必须在循环中检查 stop_requested()\n";
    std::cout << "   如果线程在阻塞操作中，不会自动唤醒\n\n";

    // 陷阱2：jthread 析构顺序
    std::cout << "2. jthread 析构时先 request_stop()，再 join():\n";
    std::cout << "   如果线程不检查 stop_token，join 会一直等待\n";
    std::cout << "   相当于和 std::thread 不 join 同样的死锁风险\n\n";

    // 陷阱3：stop_callback 的生命周期
    std::cout << "3. stop_callback 在析构时自动注销:\n";
    std::cout << "   如果 callback 对象先于 stop 请求析构，回调不会执行\n";
    std::cout << "   确保 callback 对象的生命周期覆盖需要的范围\n\n";

    // 陷阱4：不能 detach 后再 request_stop
    std::cout << "4. detach 后不能再 request_stop:\n";
    std::cout << "   jthread 支持 detach()，但 detach 后 stop_source 失效\n";
    std::cout << "   通常不建议对 jthread 使用 detach\n\n";

    // 陷阱5：stop_callback 的线程安全
    std::cout << "5. stop_callback 执行的线程:\n";
    std::cout << "   如果注册时已经 stop_requested，回调在注册线程执行\n";
    std::cout << "   如果注册后才 stop_requested，回调在调用 request_stop 的线程执行\n";
    std::cout << "   回调中要注意线程安全\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== jthread 和 stop_token - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: jthread 和 stop_token (协作式线程取消)](https://en.cppreference.com/w/cpp/thread/thread)
