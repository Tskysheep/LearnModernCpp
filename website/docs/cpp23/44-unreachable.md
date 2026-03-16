---
sidebar_position: 45
title: "std::unreachable（不可达标记）"
description: "C++23 std::unreachable（不可达标记） 教学示例"
tags: [cpp23, unreachable]
---

# std::unreachable（不可达标记）

C++23 标准引入的 std::unreachable（不可达标记） 特性完整教学示例。

## 完整源码

```cpp title="cpp23/44_unreachable.cpp"
/**
 * ============================================================
 * 特性名称: std::unreachable（不可达标记）
 * 所属标准: C++23
 * ============================================================
 *
 * [是什么]: std::unreachable() 是一个标记函数，告诉编译器某段代码
 *          在正常执行流程中永远不会到达。如果执行确实到达了这里，
 *          行为是未定义的（UB）。编译器可以利用这个提示进行优化。
 *
 * [为什么引入]: 在 C++23 之前，不同编译器有各自的扩展来标记不可达
 *              代码：GCC/Clang 的 __builtin_unreachable()、MSVC
 *              的 __assume(false)。std::unreachable 提供了跨平台的
 *              标准方式。
 *
 * [关键要点]:
 *   - 调用 std::unreachable() 表示此处不可能执行到
 *   - 如果真的执行到，是未定义行为
 *   - 编译器可利用此信息消除死代码、优化分支
 *   - 定义在 <utility> 头文件中
 *   - 常用于 switch 的 default 分支、完整枚举匹配后等场景
 * ============================================================
 */

#include <iostream>
#include <cstdint>
#include <string>

// ============================================================
// 特性检测
// ============================================================
#ifdef __cpp_lib_unreachable
#include <utility>
#define HAS_UNREACHABLE 1
#else
#define HAS_UNREACHABLE 0

// 提供跨平台的替代实现
namespace fallback {
    [[noreturn]] inline void unreachable() {
#if defined(__GNUC__) || defined(__clang__)
        __builtin_unreachable();
#elif defined(_MSC_VER)
        __assume(false);
#else
        // 最后的手段：无限循环（虽然不是真正的 unreachable）
        while (true) {}
#endif
    }
}
#endif

// 统一的调用方式
inline void mark_unreachable() {
#if HAS_UNREACHABLE
    std::unreachable();
#else
    fallback::unreachable();
#endif
}

// ============================================================
// 基础用法
// ============================================================
namespace basic_usage {

enum class Direction { North, South, East, West };

// 使用 unreachable 标记不可达的 default 分支
std::string direction_name(Direction dir) {
    switch (dir) {
        case Direction::North: return "北";
        case Direction::South: return "南";
        case Direction::East:  return "东";
        case Direction::West:  return "西";
    }
    // 所有枚举值都已覆盖，这里不可能到达
    // 但没有这行，某些编译器会警告"不是所有路径都有返回值"
    mark_unreachable();
}

// 简单的除以 2 的幂（假设 n 一定是 2 的幂）
int fast_log2(unsigned n) {
    // 前提条件：n 是 2 的幂
    switch (n) {
        case 1:   return 0;
        case 2:   return 1;
        case 4:   return 2;
        case 8:   return 3;
        case 16:  return 4;
        case 32:  return 5;
        case 64:  return 6;
        case 128: return 7;
        case 256: return 8;
        default:
            // 如果调用者遵守了前提条件（n<=256 且为 2 的幂），这里不可达
            mark_unreachable();
    }
}

void demonstrate() {
    std::cout << "[基础用法]" << std::endl;

    // 枚举 switch 示例
    std::cout << "  -- 枚举 switch 中使用 --" << std::endl;
    std::cout << "  North = " << direction_name(Direction::North) << std::endl;
    std::cout << "  South = " << direction_name(Direction::South) << std::endl;
    std::cout << "  East  = " << direction_name(Direction::East) << std::endl;
    std::cout << "  West  = " << direction_name(Direction::West) << std::endl;

    // log2 示例
    std::cout << "  -- 优化 switch 查找 --" << std::endl;
    std::cout << "  fast_log2(1)   = " << fast_log2(1) << std::endl;
    std::cout << "  fast_log2(16)  = " << fast_log2(16) << std::endl;
    std::cout << "  fast_log2(256) = " << fast_log2(256) << std::endl;

    std::cout << std::endl;
}

} // namespace basic_usage

// ============================================================
// 进阶用法
// ============================================================
namespace advanced_usage {

// 场景1：状态机中的不可达状态
enum class State : uint8_t {
    Init = 0,
    Running = 1,
    Paused = 2,
    Stopped = 3
};

class StateMachine {
public:
    explicit StateMachine() : state_(State::Init) {}

    void start() {
        if (state_ == State::Init || state_ == State::Paused) {
            state_ = State::Running;
        }
    }

    void pause() {
        if (state_ == State::Running) {
            state_ = State::Paused;
        }
    }

    void stop() {
        state_ = State::Stopped;
    }

    // 只在 Running 状态调用
    int process_step() {
        switch (state_) {
            case State::Running:
                return 42;  // 正常处理
            case State::Init:
            case State::Paused:
            case State::Stopped:
                // 调用者保证只在 Running 状态调用此方法
                // 如果到这里，说明有 bug
                mark_unreachable();
        }
        mark_unreachable();
    }

    const char* state_name() const {
        switch (state_) {
            case State::Init:    return "初始化";
            case State::Running: return "运行中";
            case State::Paused:  return "已暂停";
            case State::Stopped: return "已停止";
        }
        mark_unreachable();
    }

private:
    State state_;
};

// 场景2：百分比范围映射（利用 unreachable 消除多余检查）
const char* grade(int score) {
    if (score >= 90) return "优秀";
    if (score >= 80) return "良好";
    if (score >= 70) return "中等";
    if (score >= 60) return "及格";
    if (score >= 0)  return "不及格";
    // score 不应为负数（前提条件）
    mark_unreachable();
}

// 场景3：与 [[noreturn]] 的关系
[[noreturn]] void fatal_error(const std::string& msg) {
    std::cerr << "  [致命错误] " << msg << std::endl;
    std::abort();
}

int get_positive_value(int x) {
    if (x > 0) return x;
    if (x == 0) return 1;
    // x < 0 的情况不应发生（由调用者保证）
    fatal_error("不应为负值");
    // fatal_error 标记了 [[noreturn]]，编译器知道不会到这里
    // 但如果没有 [[noreturn]]，就需要：
    // mark_unreachable();
}

void demonstrate() {
    std::cout << "[进阶用法]" << std::endl;

    // 状态机
    std::cout << "  -- 状态机 --" << std::endl;
    StateMachine sm;
    std::cout << "  当前状态: " << sm.state_name() << std::endl;
    sm.start();
    std::cout << "  启动后: " << sm.state_name() << std::endl;
    std::cout << "  处理步骤结果: " << sm.process_step() << std::endl;
    sm.pause();
    std::cout << "  暂停后: " << sm.state_name() << std::endl;

    // 分数等级
    std::cout << "  -- 分数等级 --" << std::endl;
    int scores[] = {95, 85, 75, 65, 50};
    for (int s : scores) {
        std::cout << "  " << s << " 分 -> " << grade(s) << std::endl;
    }

    // 正值获取
    std::cout << "  -- 正值保证 --" << std::endl;
    std::cout << "  get_positive_value(5) = " << get_positive_value(5) << std::endl;
    std::cout << "  get_positive_value(0) = " << get_positive_value(0) << std::endl;

    // 与各平台内建函数对比
    std::cout << "  -- 跨平台对比 --" << std::endl;
    std::cout << "  GCC/Clang:  __builtin_unreachable()" << std::endl;
    std::cout << "  MSVC:       __assume(false)" << std::endl;
    std::cout << "  C++23:      std::unreachable()     <- 标准统一方式" << std::endl;

    std::cout << std::endl;
}

} // namespace advanced_usage

// ============================================================
// 易错点与陷阱
// ============================================================
namespace pitfalls {

void demonstrate() {
    std::cout << "[易错点与陷阱]" << std::endl;

    // 陷阱1：如果执行到了 unreachable，是未定义行为
    std::cout << "  陷阱1: unreachable 不是 assert！" << std::endl;
    std::cout << "    - assert: 调试时检测错误，发布时移除" << std::endl;
    std::cout << "    - unreachable: 告诉编译器这里永远不会到达" << std::endl;
    std::cout << "    - 如果执行到了 unreachable，任何事都可能发生（UB）" << std::endl;
    std::cout << "    - 在调试阶段，建议先用 assert(false) 再用 unreachable" << std::endl;

    // 陷阱2：不要用 unreachable 替代错误处理
    std::cout << "  陷阱2: 不要滥用 unreachable" << std::endl;
    std::cout << "    - 错误: 用 unreachable 掩盖未处理的错误路径" << std::endl;
    std::cout << "    - 正确: 只在逻辑上确实不可能到达时使用" << std::endl;
    std::cout << "    - 建议模式:" << std::endl;
    std::cout << R"(      #ifdef NDEBUG
          std::unreachable();  // 发布版：优化
      #else
          assert(false && "不应到达此处");  // 调试版：检测
      #endif)" << std::endl;

    // 陷阱3：编译器可能利用 unreachable 做"激进"优化
    std::cout << "  陷阱3: 编译器会基于 unreachable 做优化" << std::endl;
    std::cout << "    - 如果你错误地标记了可达代码为 unreachable" << std::endl;
    std::cout << "    - 编译器可能删除看似必要的代码" << std::endl;
    std::cout << "    - 导致难以调试的 bug" << std::endl;

    // 陷阱4：与 [[noreturn]] 的区别
    std::cout << "  陷阱4: unreachable vs [[noreturn]]" << std::endl;
    std::cout << "    - [[noreturn]]: 标记函数永远不会正常返回（如 exit、throw）" << std::endl;
    std::cout << "    - unreachable: 标记某个代码位置永远不会被执行" << std::endl;
    std::cout << "    - [[noreturn]] 函数内部不需要 unreachable" << std::endl;

    // 陷阱5：不是编译期检查
    std::cout << "  陷阱5: unreachable 不做编译期验证" << std::endl;
    std::cout << "    - 编译器不会验证该位置是否真的不可达" << std::endl;
    std::cout << "    - 你需要自己确保逻辑正确" << std::endl;

    std::cout << std::endl;
}

} // namespace pitfalls

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::unreachable（不可达标记） - C++23 =====\n" << std::endl;

#if HAS_UNREACHABLE
    std::cout << "[信息] 使用 std::unreachable (C++23 标准)" << std::endl;
#else
    std::cout << "[信息] std::unreachable 不可用，使用编译器内建替代" << std::endl;
    std::cout << "请使用支持 C++23 的编译器（如 GCC 14+ 或 Clang 18+）获得标准支持。" << std::endl;
#endif
    std::cout << std::endl;

    basic_usage::demonstrate();
    advanced_usage::demonstrate();
    pitfalls::demonstrate();

    return 0;
}
```

## 参考链接

- [cppreference: std::unreachable（不可达标记）](https://en.cppreference.com/w/cpp/utility/unreachable)
