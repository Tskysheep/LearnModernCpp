---
sidebar_position: 81
title: "std::chrono 时间库"
description: "C++11 std::chrono 时间库 教学示例"
tags: [cpp11, chrono]
---

# std::chrono 时间库

C++11 标准引入的 std::chrono 时间库 特性完整教学示例。

## 完整源码

```cpp title="cpp11/80_chrono.cpp"
/**
 * ============================================================
 * 特性名称: std::chrono 时间库
 * 所属标准: C++11
 * ============================================================
 *
 * [是什么]: std::chrono 是C++11引入的时间处理库，提供了类型安全的时间段
 *           (duration)、时间点(time_point)和时钟(clock)的抽象。
 *
 * [为什么引入]: C语言的 time()/clock() 精度低、类型不安全、易混淆单位。
 *               chrono 通过模板和类型系统在编译期防止时间单位的误用。
 *
 * [关键要点]:
 *   - duration: 表示时间段（如秒、毫秒、微秒等）
 *   - time_point: 表示某个时钟上的时间点
 *   - system_clock: 系统时钟，可与日历时间互转
 *   - steady_clock: 单调递增时钟，适合计时
 *   - high_resolution_clock: 最高精度时钟（通常是steady_clock的别名）
 * ============================================================
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <ctime>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. duration（时间段）
    std::cout << "[1] duration (时间段):\n";
    {
        // 预定义的 duration 类型
        std::chrono::hours        h(1);       // 1小时
        std::chrono::minutes      m(30);      // 30分钟
        std::chrono::seconds      s(45);      // 45秒
        std::chrono::milliseconds ms(500);    // 500毫秒
        std::chrono::microseconds us(1000);   // 1000微秒
        std::chrono::nanoseconds  ns(500000); // 500000纳秒

        std::cout << "  1小时 = " << h.count() << " 小时\n";
        std::cout << "  30分钟 = " << m.count() << " 分钟\n";
        std::cout << "  45秒 = " << s.count() << " 秒\n";
        std::cout << "  500毫秒 = " << ms.count() << " 毫秒\n";
        std::cout << "  1000微秒 = " << us.count() << " 微秒\n";
        std::cout << "  500000纳秒 = " << ns.count() << " 纳秒\n";
    }
    std::cout << std::endl;

    // 2. duration 转换
    std::cout << "[2] duration 转换:\n";
    {
        std::chrono::hours h(2);

        // 从大单位到小单位：隐式转换
        std::chrono::minutes m = h;
        std::chrono::seconds s = h;

        std::cout << "  2小时 = " << m.count() << " 分钟\n";
        std::cout << "  2小时 = " << s.count() << " 秒\n";

        // 从小单位到大单位：需要显式 duration_cast
        std::chrono::seconds s2(3661);  // 1小时1分1秒
        auto h2 = std::chrono::duration_cast<std::chrono::hours>(s2);
        auto m2 = std::chrono::duration_cast<std::chrono::minutes>(s2);

        std::cout << "  3661秒 = " << h2.count() << " 小时 (截断)\n";
        std::cout << "  3661秒 = " << m2.count() << " 分钟 (截断)\n";
    }
    std::cout << std::endl;

    // 3. duration 算术
    std::cout << "[3] duration 算术运算:\n";
    {
        auto d1 = std::chrono::seconds(30);
        auto d2 = std::chrono::minutes(1);

        auto sum = d1 + d2;  // 结果自动转为共同类型（秒）
        std::cout << "  30秒 + 1分钟 = " << sum.count() << " 秒\n";

        auto diff = d2 - d1;
        std::cout << "  1分钟 - 30秒 = " << diff.count() << " 秒\n";

        auto doubled = d1 * 2;
        std::cout << "  30秒 * 2 = " << doubled.count() << " 秒\n";

        // 比较
        std::cout << "  30秒 < 1分钟: "
                  << (d1 < d2 ? "是" : "否") << std::endl;
    }
    std::cout << std::endl;

    // 4. 时钟和时间点
    std::cout << "[4] 时钟 (clock) 和时间点 (time_point):\n";
    {
        // system_clock: 系统时钟
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::cout << "  系统时间: " << std::ctime(&t);

        // steady_clock: 单调时钟（不受系统时间调整影响）
        auto steady_now = std::chrono::steady_clock::now();
        std::cout << "  steady_clock 纪元以来: "
                  << std::chrono::duration_cast<std::chrono::seconds>(
                         steady_now.time_since_epoch()).count()
                  << " 秒\n";

        // high_resolution_clock
        std::cout << "  high_resolution_clock 精度: "
                  << std::chrono::high_resolution_clock::period::num << "/"
                  << std::chrono::high_resolution_clock::period::den << " 秒\n";
    }
    std::cout << std::endl;

    // 5. 基本计时
    std::cout << "[5] 基本计时:\n";
    {
        auto start = std::chrono::steady_clock::now();

        // 模拟耗时操作
        std::this_thread::sleep_for(std::chrono::milliseconds(150));

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "  sleep(150ms) 实际耗时: " << elapsed.count() << " ms\n";
    }
    std::cout << std::endl;
}

// ============================================================
// 高级用法
// ============================================================
void advanced_usage() {
    std::cout << "--- 高级用法 ---\n\n";

    // 1. 高精度计时器类
    std::cout << "[1] 高精度计时器类:\n";
    {
        class Timer {
            using Clock = std::chrono::steady_clock;
            using TimePoint = Clock::time_point;

            TimePoint start_;
            std::string name_;

        public:
            Timer(const std::string& name = "计时器") : name_(name) {
                start_ = Clock::now();
            }

            ~Timer() {
                auto end = Clock::now();
                auto us = std::chrono::duration_cast<std::chrono::microseconds>(
                    end - start_).count();
                std::cout << "  [" << name_ << "] 耗时: " << us << " 微秒\n";
            }

            double elapsed_ms() const {
                auto now = Clock::now();
                return std::chrono::duration<double, std::milli>(now - start_).count();
            }
        };

        // 使用 RAII 计时器
        {
            Timer t("向量排序");
            std::vector<int> data(100000);
            std::iota(data.begin(), data.end(), 0);
            std::reverse(data.begin(), data.end());
            std::sort(data.begin(), data.end());
        }

        {
            Timer t("休眠100ms");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    std::cout << std::endl;

    // 2. 自定义 duration 类型
    std::cout << "[2] 自定义 duration 类型:\n";
    {
        // 用 ratio 定义自定义时间单位
        using Days = std::chrono::duration<int, std::ratio<86400>>;        // 天
        using Weeks = std::chrono::duration<int, std::ratio<604800>>;      // 周
        using FPS30 = std::chrono::duration<double, std::ratio<1, 30>>;    // 1/30秒（30fps）

        Days one_day(1);
        Weeks one_week(1);
        FPS30 one_frame(1);

        auto day_in_seconds = std::chrono::duration_cast<std::chrono::seconds>(one_day);
        auto week_in_days = std::chrono::duration_cast<Days>(one_week);
        auto frame_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(one_frame);

        std::cout << "  1天 = " << day_in_seconds.count() << " 秒\n";
        std::cout << "  1周 = " << week_in_days.count() << " 天\n";
        std::cout << "  1帧(30fps) = " << frame_in_ms.count() << " 毫秒\n";
    }
    std::cout << std::endl;

    // 3. time_point 运算
    std::cout << "[3] time_point 运算:\n";
    {
        auto now = std::chrono::system_clock::now();

        // 时间点加减时间段
        auto future = now + std::chrono::hours(24);
        auto past = now - std::chrono::hours(1);

        std::time_t t_now = std::chrono::system_clock::to_time_t(now);
        std::time_t t_future = std::chrono::system_clock::to_time_t(future);
        std::time_t t_past = std::chrono::system_clock::to_time_t(past);

        std::cout << "  现在:     " << std::ctime(&t_now);
        std::cout << "  24小时后: " << std::ctime(&t_future);
        std::cout << "  1小时前:  " << std::ctime(&t_past);

        // 两个时间点相减得到时间段
        auto diff = future - now;
        std::cout << "  差值: "
                  << std::chrono::duration_cast<std::chrono::hours>(diff).count()
                  << " 小时\n";
    }
    std::cout << std::endl;

    // 4. 与线程结合
    std::cout << "[4] 与线程计时结合:\n";
    {
        // sleep_for
        auto start = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start).count();
        std::cout << "  sleep_for(50ms): " << elapsed_ms << " ms\n";

        // sleep_until
        auto wake_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(50);
        start = std::chrono::steady_clock::now();
        std::this_thread::sleep_until(wake_time);
        elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start).count();
        std::cout << "  sleep_until(+50ms): " << elapsed_ms << " ms\n";
    }
    std::cout << std::endl;

    // 5. 浮点数 duration
    std::cout << "[5] 浮点数 duration (高精度):\n";
    {
        // 使用 double 存储，避免截断
        std::chrono::duration<double> seconds(2.5);
        std::chrono::duration<double, std::milli> ms(2500.5);
        std::chrono::duration<double, std::micro> us(2500500.0);

        std::cout << "  2.5秒 = " << seconds.count() << " 秒\n";
        std::cout << "  2500.5毫秒 = " << ms.count() << " 毫秒\n";

        // 精确计时
        auto start = std::chrono::steady_clock::now();
        volatile int sum = 0;
        for (int i = 0; i < 1000000; ++i) sum += i;
        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::cout << "  百万次累加耗时: " << elapsed.count() << " ms\n";
        (void)sum;
    }
    std::cout << std::endl;
}

// ============================================================
// 易错点
// ============================================================
void pitfalls() {
    std::cout << "--- 易错点 ---\n\n";

    // 1. 不要用 system_clock 计时
    std::cout << "[1] 计时应用 steady_clock，不要用 system_clock:\n";
    std::cout << "  system_clock 可能被NTP调整或用户修改\n";
    std::cout << "  steady_clock 保证单调递增，适合计时\n";
    std::cout << "  // 错误: auto start = system_clock::now();\n";
    std::cout << "  // 正确: auto start = steady_clock::now();\n\n";

    // 2. duration_cast 的截断
    std::cout << "[2] duration_cast 会截断（向零取整）:\n";
    {
        std::chrono::milliseconds ms(1500);
        auto s = std::chrono::duration_cast<std::chrono::seconds>(ms);
        std::cout << "  1500毫秒 -> " << s.count() << " 秒 (截断了500ms)\n";
        std::cout << "  如需四舍五入，需要手动处理\n";
    }
    std::cout << std::endl;

    // 3. count() 的误用
    std::cout << "[3] 尽量避免过早调用 count():\n";
    std::cout << "  count() 丢失类型信息，无法防止单位错误\n";
    std::cout << "  应尽可能使用 duration 类型进行运算\n";
    std::cout << "  只在最终输出时才调用 count()\n\n";

    // 4. 时钟精度
    std::cout << "[4] 时钟精度不等于实际可测量精度:\n";
    {
        std::cout << "  system_clock 精度: "
                  << std::chrono::system_clock::period::num << "/"
                  << std::chrono::system_clock::period::den << " 秒\n";
        std::cout << "  steady_clock 精度: "
                  << std::chrono::steady_clock::period::num << "/"
                  << std::chrono::steady_clock::period::den << " 秒\n";
        std::cout << "  实际精度受操作系统调度等因素影响\n";
    }
    std::cout << std::endl;

    // 5. 隐式转换规则
    std::cout << "[5] 隐式转换只允许无精度损失的方向:\n";
    std::cout << "  // std::chrono::seconds s(1);\n";
    std::cout << "  // std::chrono::milliseconds ms = s;  // OK: 秒->毫秒\n";
    std::cout << "  // std::chrono::seconds s2 = ms;      // 编译错误！\n";
    std::cout << "  // auto s2 = duration_cast<seconds>(ms);  // 显式转换\n\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::chrono 时间库 - C++11 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::chrono 时间库](https://en.cppreference.com/w/cpp/chrono)
