/**
 * ============================================================
 * 特性名称: Ranges 基础 (std::ranges 算法)
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: std::ranges 命名空间下提供了一套新的算法，
 *          可以直接接受容器（而非迭代器对），
 *          并支持投影（projection）功能。
 *
 * [为什么引入]: 传统 STL 算法需要传递 begin/end 迭代器对，
 *             代码冗长且容易出错。Ranges 算法更简洁、更安全。
 *
 * [关键要点]:
 *   - 直接传容器，不需要 begin()/end()
 *   - 投影（projection）可以在算法中变换元素
 *   - 返回更丰富的信息（如 ranges::find 返回迭代器）
 *   - 约束更严格，错误信息更清晰
 * ============================================================
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

// ===================== basic_usage() =====================

struct Student {
    std::string name;
    int age;
    double score;
};

void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. ranges::sort — 直接传容器
    std::vector<int> nums = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    // 传统写法
    // std::sort(nums.begin(), nums.end());

    // C++20 ranges 写法
    std::ranges::sort(nums);
    std::cout << "排序后: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";

    // 降序排列
    std::ranges::sort(nums, std::greater{});
    std::cout << "降序: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";

    // 2. ranges::find
    auto it = std::ranges::find(nums, 5);
    if (it != nums.end()) {
        std::cout << "\n找到 5，位置: " << std::distance(nums.begin(), it) << "\n";
    }

    // 3. ranges::count
    std::vector<int> data = {1, 2, 3, 2, 4, 2, 5};
    auto cnt = std::ranges::count(data, 2);
    std::cout << "2 出现了 " << cnt << " 次\n";

    // 4. ranges::min / ranges::max
    auto min_val = std::ranges::min(nums);
    auto max_val = std::ranges::max(nums);
    std::cout << "最小值: " << min_val << ", 最大值: " << max_val << "\n";

    // 5. ranges::for_each
    std::cout << "\nfor_each 输出: ";
    std::ranges::for_each(nums, [](int n) { std::cout << n * 2 << " "; });
    std::cout << "\n";
}

// ===================== advanced_usage() =====================
void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // ---- 投影 (Projection) ----
    std::cout << "-- 投影 (Projection) --\n";

    std::vector<Student> students = {
        {"张三", 20, 85.5},
        {"李四", 22, 92.0},
        {"王五", 19, 78.3},
        {"赵六", 21, 95.5},
        {"孙七", 20, 88.0}
    };

    // 按分数排序（使用投影）
    std::ranges::sort(students, std::less{}, &Student::score);
    std::cout << "按分数升序:\n";
    for (const auto& s : students) {
        std::cout << "  " << s.name << " - 年龄:" << s.age
                  << " 分数:" << s.score << "\n";
    }

    // 按年龄排序
    std::ranges::sort(students, {}, &Student::age);  // {} 表示默认比较器 (less)
    std::cout << "\n按年龄升序:\n";
    for (const auto& s : students) {
        std::cout << "  " << s.name << " - 年龄:" << s.age << "\n";
    }

    // 用投影查找
    auto found = std::ranges::find(students, "赵六", &Student::name);
    if (found != students.end()) {
        std::cout << "\n找到赵六，分数: " << found->score << "\n";
    }

    // 用投影查找最高分
    auto best = std::ranges::max_element(students, {}, &Student::score);
    std::cout << "最高分: " << best->name << " (" << best->score << ")\n";

    // 投影可以是任意可调用对象（Lambda）
    std::ranges::sort(students, {}, [](const Student& s) {
        return s.name.size();  // 按名字长度排序
    });
    std::cout << "\n按名字长度排序:\n";
    for (const auto& s : students) {
        std::cout << "  " << s.name << " (长度:" << s.name.size() << ")\n";
    }

    // ---- 与传统算法对比 ----
    std::cout << "\n-- 与传统算法对比 --\n";
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

    // 传统：需要两个迭代器
    // std::sort(v.begin(), v.end());
    // auto it = std::find(v.begin(), v.end(), 5);

    // Ranges：直接传容器
    std::ranges::sort(v);
    auto it = std::ranges::find(v, 5);
    std::cout << "传统算法需要 v.begin(), v.end()\n";
    std::cout << "Ranges 算法直接传 v\n";

    // ranges::contains (C++23) 类似功能用 find 实现
    bool has_five = std::ranges::find(v, 5) != v.end();
    std::cout << "是否包含 5: " << std::boolalpha << has_five << "\n";

    // ---- 其他常用 ranges 算法 ----
    std::cout << "\n-- 其他常用算法 --\n";

    // ranges::all_of / any_of / none_of
    std::vector<int> scores = {85, 92, 78, 95, 88};
    bool all_pass = std::ranges::all_of(scores, [](int s) { return s >= 60; });
    std::cout << "全部及格: " << all_pass << "\n";

    bool any_excellent = std::ranges::any_of(scores, [](int s) { return s >= 90; });
    std::cout << "有人优秀: " << any_excellent << "\n";

    // ranges::copy
    std::vector<int> dest;
    dest.resize(scores.size());
    std::ranges::copy(scores, dest.begin());
    std::cout << "复制: ";
    for (int s : dest) std::cout << s << " ";
    std::cout << "\n";

    // ranges::reverse
    std::ranges::reverse(dest);
    std::cout << "反转: ";
    for (int s : dest) std::cout << s << " ";
    std::cout << "\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：ranges 算法在 std::ranges 命名空间下
    std::cout << "1. ranges 算法在 std::ranges 命名空间下:\n";
    std::cout << "   std::sort(v.begin(), v.end());   // 传统算法\n";
    std::cout << "   std::ranges::sort(v);            // C++20 ranges 算法\n";
    std::cout << "   两者可以共存，不会冲突\n\n";

    // 陷阱2：投影不改变原始数据
    std::cout << "2. 投影只是在算法内部变换，不修改原始数据:\n";
    std::vector<int> nums = {-3, -1, 4, -1, 5};
    std::ranges::sort(nums, {}, [](int n) { return std::abs(n); });
    std::cout << "   按绝对值排序: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n   原始值保持不变（仍然有负数）\n\n";

    // 陷阱3：返回值不同
    std::cout << "3. ranges 算法返回值更丰富:\n";
    std::cout << "   std::sort 返回 void\n";
    std::cout << "   std::ranges::sort 返回迭代器\n";
    std::cout << "   std::ranges::find 返回迭代器(借用范围时)\n\n";

    // 陷阱4：悬垂迭代器保护
    std::cout << "4. ranges 算法防止悬垂迭代器:\n";
    std::cout << "   auto it = std::ranges::find(get_temp_vec(), 42);\n";
    std::cout << "   // 返回 std::ranges::dangling 而非迭代器!\n";
    std::cout << "   这是一个编译期安全检查\n\n";

    // 陷阱5：ranges 算法不接受所有容器
    std::cout << "5. ranges 算法对容器有 concept 约束:\n";
    std::cout << "   ranges::sort 要求 random_access_range + sortable\n";
    std::cout << "   std::list 不满足 random_access，不能用 ranges::sort\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== Ranges 基础算法 - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
