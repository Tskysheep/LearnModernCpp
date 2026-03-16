/**
 * ============================================================
 * 特性名称: std::exchange
 * 所属标准: C++14
 * ============================================================
 *
 * [是什么]: std::exchange(obj, new_value) 将obj的值替换为new_value，
 *          同时返回obj的旧值。它是一个简单但非常实用的工具函数，
 *          定义在<utility>头文件中。
 *
 * [为什么引入]: 在移动语义的实现中，经常需要"取出旧值并替换为默认值"的操作。
 *              没有exchange时，需要临时变量来保存旧值，代码冗长且容易出错。
 *              exchange将这个常见模式封装为一行代码。
 *
 * [关键要点]:
 *   - exchange(obj, new_val) 等价于 { auto old = move(obj); obj = new_val; return old; }
 *   - 最常用于简化移动构造函数和移动赋值运算符
 *   - 也适用于需要"读取并重置"的场景
 *   - 不是原子操作，不用于多线程同步
 * ============================================================
 */

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <algorithm>

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 最基本的用法：替换并获取旧值
    int x = 42;
    int old_x = std::exchange(x, 100);
    std::cout << "  旧值: " << old_x << "\n";   // 42
    std::cout << "  新值: " << x << "\n";         // 100

    // 2. 字符串
    std::string s = "Hello";
    std::string old_s = std::exchange(s, "World");
    std::cout << "  旧字符串: \"" << old_s << "\"\n";   // Hello
    std::cout << "  新字符串: \"" << s << "\"\n";         // World

    // 3. 与手动写法对比
    std::cout << "\n  [与手动写法对比]\n";

    // 手动写法
    int a = 10;
    int temp = a;
    a = 20;
    std::cout << "  手动: old=" << temp << ", new=" << a << "\n";

    // exchange写法
    int b = 10;
    int old_b = std::exchange(b, 20);
    std::cout << "  exchange: old=" << old_b << ", new=" << b << "\n";

    // 4. 用于"消费"一个值
    bool flag = true;
    if (std::exchange(flag, false)) {
        std::cout << "\n  flag曾经是true，现在已重置为false\n";
    }
    std::cout << "  flag = " << std::boolalpha << flag << "\n";
}

// ============================================================
// 进阶用法
// ============================================================

// 1. 简化移动构造函数和移动赋值运算符
class Buffer {
public:
    Buffer() : data_(nullptr), size_(0), name_("empty") {
        std::cout << "    Buffer() 默认构造\n";
    }

    explicit Buffer(std::size_t size, const std::string& name = "unnamed")
        : data_(new int[size]()), size_(size), name_(name) {
        std::cout << "    Buffer(\"" << name_ << "\", " << size_ << ") 构造\n";
    }

    ~Buffer() {
        if (data_) {
            std::cout << "    ~Buffer(\"" << name_ << "\") 释放 " << size_ << " 个int\n";
        }
        delete[] data_;
    }

    // 移动构造函数——使用exchange简化
    Buffer(Buffer&& other) noexcept
        : data_(std::exchange(other.data_, nullptr))    // 取走指针，置空源
        , size_(std::exchange(other.size_, 0))           // 取走大小，归零
        , name_(std::exchange(other.name_, "moved-from")) // 取走名字，标记
    {
        std::cout << "    Buffer(\"" << name_ << "\") 移动构造\n";
    }

    // 移动赋值运算符——使用exchange简化
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;  // 释放当前资源

            data_ = std::exchange(other.data_, nullptr);
            size_ = std::exchange(other.size_, 0);
            name_ = std::exchange(other.name_, "moved-from");
            std::cout << "    Buffer(\"" << name_ << "\") 移动赋值\n";
        }
        return *this;
    }

    // 禁用拷贝
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    void info() const {
        std::cout << "    [" << name_ << "] size=" << size_
                  << ", data=" << (data_ ? "有效" : "空") << "\n";
    }

    std::size_t size() const { return size_; }
    const std::string& name() const { return name_; }

private:
    int* data_;
    std::size_t size_;
    std::string name_;
};

// 不使用exchange的移动构造函数对比
class BufferOld {
public:
    BufferOld() : data_(nullptr), size_(0) {}
    explicit BufferOld(std::size_t size) : data_(new int[size]()), size_(size) {}
    ~BufferOld() { delete[] data_; }

    // 不用exchange——需要更多代码
    BufferOld(BufferOld&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;  // 必须手动置空
        other.size_ = 0;        // 必须手动归零
    }

    BufferOld(const BufferOld&) = delete;
    BufferOld& operator=(const BufferOld&) = delete;
    BufferOld& operator=(BufferOld&&) = delete;

private:
    int* data_;
    std::size_t size_;
};

// 2. 链表节点的所有权转移
struct ListNode {
    int value;
    std::unique_ptr<ListNode> next;

    ListNode(int v, std::unique_ptr<ListNode> n = nullptr)
        : value(v), next(std::move(n)) {}
};

// 3. 状态机中的状态转换
class StateMachine {
public:
    enum class State { Idle, Running, Paused, Stopped };

    State transition(State new_state) {
        return std::exchange(state_, new_state);
    }

    State current() const { return state_; }

    static const char* state_name(State s) {
        switch (s) {
            case State::Idle:    return "Idle";
            case State::Running: return "Running";
            case State::Paused:  return "Paused";
            case State::Stopped: return "Stopped";
            default:             return "Unknown";
        }
    }

private:
    State state_ = State::Idle;
};

void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 1. 移动构造函数演示
    std::cout << "  [移动构造函数]\n";
    Buffer buf1(1024, "original");
    buf1.info();

    Buffer buf2(std::move(buf1));
    std::cout << "  移动后:\n";
    buf1.info();  // moved-from
    buf2.info();  // original

    // 移动赋值
    std::cout << "\n  [移动赋值运算符]\n";
    Buffer buf3(512, "target");
    buf3 = std::move(buf2);
    std::cout << "  移动赋值后:\n";
    buf2.info();
    buf3.info();

    // 2. 使用exchange实现链表反转
    std::cout << "\n  [链表操作]\n";
    // 构建链表: 1 -> 2 -> 3 -> 4
    auto head = std::make_unique<ListNode>(1,
        std::make_unique<ListNode>(2,
            std::make_unique<ListNode>(3,
                std::make_unique<ListNode>(4))));

    // 用exchange反转链表
    std::unique_ptr<ListNode> prev = nullptr;
    auto current = std::move(head);
    while (current) {
        auto next = std::exchange(current->next, std::move(prev));
        prev = std::exchange(current, std::move(next));
    }
    head = std::move(prev);

    // 打印反转后的链表
    std::cout << "  反转后: ";
    for (auto* p = head.get(); p; p = p->next.get()) {
        std::cout << p->value << " ";
    }
    std::cout << "\n";

    // 3. 状态机
    std::cout << "\n  [状态机转换]\n";
    StateMachine sm;
    std::cout << "  当前: " << StateMachine::state_name(sm.current()) << "\n";

    auto old = sm.transition(StateMachine::State::Running);
    std::cout << "  转换: " << StateMachine::state_name(old)
              << " -> " << StateMachine::state_name(sm.current()) << "\n";

    old = sm.transition(StateMachine::State::Paused);
    std::cout << "  转换: " << StateMachine::state_name(old)
              << " -> " << StateMachine::state_name(sm.current()) << "\n";

    old = sm.transition(StateMachine::State::Stopped);
    std::cout << "  转换: " << StateMachine::state_name(old)
              << " -> " << StateMachine::state_name(sm.current()) << "\n";

    // 4. 消费队列——取出所有元素
    std::cout << "\n  [消费队列]\n";
    std::queue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);

    // 用exchange一次性取走整个队列
    auto consumed = std::exchange(q, std::queue<int>{});
    std::cout << "  原队列大小: " << q.size() << " (已清空)\n";
    std::cout << "  消费的元素: ";
    while (!consumed.empty()) {
        std::cout << consumed.front() << " ";
        consumed.pop();
    }
    std::cout << "\n";

    std::cout << "\n  --- 析构演示 ---\n";
}

// ============================================================
// 易错点与陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱1：exchange不是原子操作
    std::cout << "  陷阱1: std::exchange不是原子操作\n";
    std::cout << "    多线程环境中需要额外的同步机制\n";
    std::cout << "    原子交换请使用 std::atomic::exchange()\n";

    // 陷阱2：exchange的第二个参数会被移动/拷贝到目标
    std::cout << "\n  陷阱2: 注意new_value的生命周期\n";
    std::string original = "Hello";
    std::string replacement = "World";
    auto old = std::exchange(original, std::move(replacement));
    std::cout << "    old = \"" << old << "\"\n";
    std::cout << "    original = \"" << original << "\"\n";
    std::cout << "    replacement = \"" << replacement << "\" (已被移动)\n";

    // 陷阱3：自赋值场景
    std::cout << "\n  陷阱3: exchange不检查自赋值\n";
    int val = 42;
    int result = std::exchange(val, val);  // 虽然无意义但合法
    std::cout << "    exchange(42, 42): old=" << result << ", new=" << val << "\n";

    // 陷阱4：不要与std::swap混淆
    std::cout << "\n  陷阱4: exchange vs swap\n";
    int a = 10, b = 20;
    // exchange: 单向替换，返回旧值
    int old_a = std::exchange(a, 99);
    std::cout << "    exchange(a=10, 99): old_a=" << old_a << ", a=" << a << "\n";

    // swap: 双向交换
    a = 10; b = 20;
    std::swap(a, b);
    std::cout << "    swap(10, 20): a=" << a << ", b=" << b << "\n";

    // 陷阱5：返回值不使用时的警告
    std::cout << "\n  陷阱5: 如果不需要旧值，直接赋值更好\n";
    std::cout << "    std::exchange(x, 0);  // 不使用返回值——不如直接 x = 0;\n";
    std::cout << "    exchange的价值在于同时获取旧值\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::exchange - C++14 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
