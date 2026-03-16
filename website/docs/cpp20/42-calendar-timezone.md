---
sidebar_position: 43
title: "chrono 日历和时区 (Calendar and Time Zone)"
description: "C++20 chrono 日历和时区 (Calendar and Time Zone) 教学示例"
tags: [cpp20, calendar_timezone]
---

# chrono 日历和时区 (Calendar and Time Zone)

C++20 标准引入的 chrono 日历和时区 (Calendar and Time Zone) 特性完整教学示例。

## 完整源码

```cpp title="cpp20/42_calendar_timezone.cpp"
/**
 * ============================================================
 * 特性名称: chrono 日历和时区 (Calendar and Time Zone)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: C++20 为 <chrono> 库增加了日历类型（如 year_month_day）
 *          和时区支持（如 zoned_time），使日期和时间操作更加方便。
 *
 * [为什么引入]: C++17 的 chrono 只有 duration 和 time_point，
 *             没有日历和时区概念，日期操作需要依赖 C 库或第三方库。
 *
 * [关键要点]:
 *   - year_month_day: 表示日期
 *   - year/month/day 字面量
 *   - 日期算术运算
 *   - 时区转换（需要系统支持 IANA 时区数据库）
 * ============================================================
 */

#include <chrono>
#include <iostream>
#include <string>

// 检测时区支持
// 注意：时区功能需要 IANA 时区数据库支持
// GCC 需要链接 -lstdc++ 并有时区数据
// MSVC 通常开箱即用

// ===================== basic_usage() =====================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    using namespace std::chrono;

    // 1. 创建日期
    year_month_day date1{year{2024}, month{3}, day{15}};
    std::cout << "日期1: " << static_cast<int>(date1.year()) << "-"
              << static_cast<unsigned>(date1.month()) << "-"
              << static_cast<unsigned>(date1.day()) << "\n";

    // 使用运算符创建日期（更自然的语法）
    auto date2 = 2024y / March / 15d;
    std::cout << "日期2: " << static_cast<int>(date2.year()) << "-"
              << static_cast<unsigned>(date2.month()) << "-"
              << static_cast<unsigned>(date2.day()) << "\n";

    // 另一种创建方式
    auto date3 = March / 15 / 2024;
    std::cout << "日期3: " << static_cast<int>(date3.year()) << "-"
              << static_cast<unsigned>(date3.month()) << "-"
              << static_cast<unsigned>(date3.day()) << "\n";

    // 2. 日期有效性检查
    auto valid_date = 2024y / February / 29d;  // 2024 是闰年
    auto invalid_date = 2023y / February / 29d;  // 2023 不是闰年

    std::cout << "\n2024-02-29 有效: " << std::boolalpha << valid_date.ok() << "\n";
    std::cout << "2023-02-29 有效: " << invalid_date.ok() << "\n";

    // 3. 年份和月份操作
    auto y = year{2024};
    std::cout << "\n2024 是闰年: " << y.is_leap() << "\n";
    std::cout << "2023 是闰年: " << year{2023}.is_leap() << "\n";

    // 4. 星期几
    auto ymd = 2024y / March / 15d;
    year_month_day_last last_day_of_month{year{2024} / March / last};
    std::cout << "\n2024年3月最后一天: "
              << static_cast<unsigned>(last_day_of_month.day()) << "\n";

    weekday wd{sys_days{ymd}};
    std::cout << "2024-03-15 是星期: " << wd.c_encoding() << " (0=周日)\n";

    // 5. 当前时间
    auto now = system_clock::now();
    auto today = floor<days>(now);
    year_month_day today_ymd{today};
    std::cout << "\n今天: " << static_cast<int>(today_ymd.year()) << "-"
              << static_cast<unsigned>(today_ymd.month()) << "-"
              << static_cast<unsigned>(today_ymd.day()) << "\n";
}

// ===================== advanced_usage() =====================
void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    using namespace std::chrono;

    // ---- 日期算术 ----
    std::cout << "-- 日期算术 --\n";

    auto date = 2024y / March / 15d;

    // 加减月份
    auto next_month = date.year() / (date.month() + months{1}) / date.day();
    auto prev_month = date.year() / (date.month() - months{1}) / date.day();
    std::cout << "下个月: " << static_cast<int>(next_month.year()) << "-"
              << static_cast<unsigned>(next_month.month()) << "-"
              << static_cast<unsigned>(next_month.day()) << "\n";
    std::cout << "上个月: " << static_cast<int>(prev_month.year()) << "-"
              << static_cast<unsigned>(prev_month.month()) << "-"
              << static_cast<unsigned>(prev_month.day()) << "\n";

    // 加减年份
    auto next_year = (date.year() + years{1}) / date.month() / date.day();
    std::cout << "明年: " << static_cast<int>(next_year.year()) << "-"
              << static_cast<unsigned>(next_year.month()) << "-"
              << static_cast<unsigned>(next_year.day()) << "\n";

    // 使用 sys_days 计算天数差
    auto d1 = sys_days{2024y / January / 1d};
    auto d2 = sys_days{2024y / December / 31d};
    auto diff = d2 - d1;
    std::cout << "\n2024-01-01 到 2024-12-31: " << diff.count() << " 天\n";

    // 加减天数
    auto d3 = sys_days{2024y / March / 15d} + days{100};
    year_month_day result{d3};
    std::cout << "2024-03-15 + 100天: " << static_cast<int>(result.year()) << "-"
              << static_cast<unsigned>(result.month()) << "-"
              << static_cast<unsigned>(result.day()) << "\n";

    // ---- 特定日期查找 ----
    std::cout << "\n-- 特定日期 --\n";

    // 某月的第一个和最后一天
    year_month_day_last dec_last{2024y / December / last};
    std::cout << "2024年12月最后一天: "
              << static_cast<unsigned>(dec_last.day()) << "号\n";

    // 某月的第N个星期几
    // 例：2024年11月的第4个星期四（美国感恩节）
    auto thanksgiving = year_month_weekday{2024y / November / Thursday[4]};
    year_month_day thanksgiving_date{sys_days{thanksgiving}};
    std::cout << "2024年感恩节: " << static_cast<int>(thanksgiving_date.year()) << "-"
              << static_cast<unsigned>(thanksgiving_date.month()) << "-"
              << static_cast<unsigned>(thanksgiving_date.day()) << "\n";

    // ---- 时间操作 ----
    std::cout << "\n-- 时分秒操作 --\n";

    auto now = system_clock::now();
    auto today = floor<days>(now);
    auto time_since_midnight = now - today;
    hh_mm_ss hms{duration_cast<seconds>(time_since_midnight)};

    std::cout << "当前时间: "
              << hms.hours().count() << ":"
              << hms.minutes().count() << ":"
              << hms.seconds().count() << " (UTC)\n";

    // ---- 时区 (可能需要系统支持) ----
    std::cout << "\n-- 时区 --\n";

#if defined(__cpp_lib_chrono) && __cpp_lib_chrono >= 201907L
    try {
        // 获取当前时区的时间
        auto zt = zoned_time{current_zone(), system_clock::now()};
        auto local_time = zt.get_local_time();
        auto local_days_val = floor<days>(local_time);
        year_month_day local_date{local_days_val};
        hh_mm_ss local_hms{duration_cast<seconds>(local_time - local_days_val)};

        std::cout << "本地时区: " << zt.get_time_zone()->name() << "\n";
        std::cout << "本地日期: " << static_cast<int>(local_date.year()) << "-"
                  << static_cast<unsigned>(local_date.month()) << "-"
                  << static_cast<unsigned>(local_date.day()) << "\n";
        std::cout << "本地时间: "
                  << local_hms.hours().count() << ":"
                  << local_hms.minutes().count() << ":"
                  << local_hms.seconds().count() << "\n";

        // 时区转换
        auto ny_time = zoned_time{"America/New_York", system_clock::now()};
        auto tokyo_time = zoned_time{"Asia/Tokyo", system_clock::now()};

        auto ny_local = ny_time.get_local_time();
        auto ny_days = floor<days>(ny_local);
        hh_mm_ss ny_hms{duration_cast<seconds>(ny_local - ny_days)};

        auto tk_local = tokyo_time.get_local_time();
        auto tk_days = floor<days>(tk_local);
        hh_mm_ss tk_hms{duration_cast<seconds>(tk_local - tk_days)};

        std::cout << "纽约时间: " << ny_hms.hours().count() << ":"
                  << ny_hms.minutes().count() << ":"
                  << ny_hms.seconds().count() << "\n";
        std::cout << "东京时间: " << tk_hms.hours().count() << ":"
                  << tk_hms.minutes().count() << ":"
                  << tk_hms.seconds().count() << "\n";
    } catch (const std::exception& e) {
        std::cout << "时区功能异常: " << e.what() << "\n";
        std::cout << "（可能缺少 IANA 时区数据库）\n";
    }
#else
    std::cout << "时区功能不可用（需要 __cpp_lib_chrono >= 201907L）\n";
    std::cout << "GCC: 需要链接 -lstdc++ 并安装 tzdata\n";
    std::cout << "MSVC: 通常直接可用\n";
#endif
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    using namespace std::chrono;

    // 陷阱1：日期溢出
    std::cout << "1. 月份和日期操作可能产生无效日期:\n";
    auto jan31 = 2024y / January / 31d;
    auto next = jan31.year() / (jan31.month() + months{1}) / jan31.day();
    std::cout << "   2024-01-31 + 1月 = " << static_cast<int>(next.year()) << "-"
              << static_cast<unsigned>(next.month()) << "-"
              << static_cast<unsigned>(next.day())
              << " (有效: " << std::boolalpha << next.ok() << ")\n";
    std::cout << "   2月没有31号! 需要手动处理\n\n";

    // 陷阱2：时区数据库
    std::cout << "2. 时区功能依赖系统的 IANA 时区数据库:\n";
    std::cout << "   Linux/Mac: 通常预装 (tzdata 包)\n";
    std::cout << "   Windows MSVC: 使用 ICU 或系统注册表\n";
    std::cout << "   GCC on Windows: 可能需要手动配置\n\n";

    // 陷阱3：sys_days 和 local_days 的区别
    std::cout << "3. sys_days vs local_days:\n";
    std::cout << "   sys_days: UTC 时间的天数\n";
    std::cout << "   local_days: 本地时间的天数\n";
    std::cout << "   跨时区时两者可能不同（日期线两侧）\n\n";

    // 陷阱4：year_month_day 不是 time_point
    std::cout << "4. year_month_day 是日历类型，不是 time_point:\n";
    std::cout << "   需要通过 sys_days 转换:\n";
    std::cout << "   sys_days tp = sys_days{2024y/March/15d};\n";
    std::cout << "   year_month_day ymd{tp};  // 反向转换\n\n";

    // 陷阱5：月份枚举从1开始
    std::cout << "5. 月份从1开始（不是0）:\n";
    std::cout << "   January = 1, February = 2, ..., December = 12\n";
    std::cout << "   与 C 的 struct tm 不同（tm 中 tm_mon 从0开始）\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== chrono 日历和时区 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: chrono 日历和时区 (Calendar and Time Zone)](https://en.cppreference.com/w/cpp/chrono#Calendar)
