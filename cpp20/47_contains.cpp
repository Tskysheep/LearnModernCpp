/**
 * ============================================================
 * 特性名称: 关联容器 contains()
 * 所属标准: C++20
 * ============================================================
 *
 * [是什么]: C++20 为所有关联容器（map、set、unordered_map、unordered_set）
 *          添加了 contains() 成员函数，直接返回 bool，
 *          表示容器中是否存在指定的键。
 *
 * [为什么引入]: 之前判断元素是否存在需要 find() != end()，
 *             语法冗长且容易写错。
 *             contains() 语义更直观、代码更简洁。
 *
 * [关键要点]:
 *   - 返回 bool，比 find() != end() 更简洁
 *   - 适用于所有关联容器和无序关联容器
 *   - 不返回迭代器（如果需要迭代器，仍用 find）
 *   - 与 count() != 0 等价，但语义更清晰
 * ============================================================
 */

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// ===================== basic_usage() =====================
void basic_usage() {
    std::cout << "--- 基础用法 ---\n\n";

    // 1. std::set
    std::set<int> numbers = {1, 3, 5, 7, 9};

    std::cout << "-- std::set --\n";
    std::cout << "set 内容: {1, 3, 5, 7, 9}\n";
    std::cout << "contains(5): " << std::boolalpha << numbers.contains(5) << "\n";
    std::cout << "contains(4): " << numbers.contains(4) << "\n";

    // 2. std::map
    std::map<std::string, int> ages = {
        {"张三", 25},
        {"李四", 30},
        {"王五", 28}
    };

    std::cout << "\n-- std::map --\n";
    std::cout << "contains(\"张三\"): " << ages.contains("张三") << "\n";
    std::cout << "contains(\"赵六\"): " << ages.contains("赵六") << "\n";

    // 3. std::unordered_set
    std::unordered_set<std::string> fruits = {"苹果", "香蕉", "橙子"};

    std::cout << "\n-- std::unordered_set --\n";
    std::cout << "contains(\"苹果\"): " << fruits.contains("苹果") << "\n";
    std::cout << "contains(\"西瓜\"): " << fruits.contains("西瓜") << "\n";

    // 4. std::unordered_map
    std::unordered_map<int, std::string> codes = {
        {200, "OK"},
        {404, "Not Found"},
        {500, "Internal Server Error"}
    };

    std::cout << "\n-- std::unordered_map --\n";
    std::cout << "contains(200): " << codes.contains(200) << "\n";
    std::cout << "contains(301): " << codes.contains(301) << "\n";

    // 5. std::multiset / std::multimap
    std::multiset<int> multi_nums = {1, 1, 2, 3, 3, 3};
    std::cout << "\n-- std::multiset --\n";
    std::cout << "contains(1): " << multi_nums.contains(1) << "\n";
    std::cout << "contains(4): " << multi_nums.contains(4) << "\n";
}

// ===================== advanced_usage() =====================
void advanced_usage() {
    std::cout << "\n--- 高级用法 ---\n\n";

    // ---- 与传统方式对比 ----
    std::cout << "-- 与传统方式对比 --\n\n";

    std::map<std::string, int> scores = {
        {"数学", 95},
        {"语文", 88},
        {"英语", 92}
    };

    std::string subject = "数学";

    // C++20: contains()
    if (scores.contains(subject)) {
        std::cout << "[C++20 contains] " << subject << " 存在, 分数: " << scores[subject] << "\n";
    }

    // C++17: find() != end()
    if (auto it = scores.find(subject); it != scores.end()) {
        std::cout << "[C++17 find] " << subject << " 存在, 分数: " << it->second << "\n";
    }

    // 更早: count() != 0
    if (scores.count(subject) != 0) {
        std::cout << "[count] " << subject << " 存在\n";
    }

    // ---- 实际应用场景 ----
    std::cout << "\n-- 实际应用：权限检查 --\n";
    std::set<std::string> permissions = {"read", "write", "execute"};

    auto check_permission = [&permissions](const std::string& perm) {
        if (permissions.contains(perm)) {
            std::cout << "  [允许] " << perm << "\n";
        } else {
            std::cout << "  [拒绝] " << perm << "\n";
        }
    };

    check_permission("read");
    check_permission("write");
    check_permission("admin");
    check_permission("delete");

    // ---- 黑名单/白名单 ----
    std::cout << "\n-- 黑名单过滤 --\n";
    std::unordered_set<std::string> blacklist = {"spam", "virus", "malware"};
    std::vector<std::string> messages = {"hello", "spam", "你好", "virus", "C++20"};

    for (const auto& msg : messages) {
        if (blacklist.contains(msg)) {
            std::cout << "  [拦截] " << msg << "\n";
        } else {
            std::cout << "  [通过] " << msg << "\n";
        }
    }

    // ---- 配置查找 ----
    std::cout << "\n-- 配置查找 --\n";
    std::map<std::string, std::string> config = {
        {"host", "localhost"},
        {"port", "8080"},
        {"debug", "true"}
    };

    auto get_config = [&config](const std::string& key,
                                 const std::string& default_val = "") -> std::string {
        if (config.contains(key)) {
            return config[key];
        }
        return default_val;
    };

    std::cout << "  host: " << get_config("host") << "\n";
    std::cout << "  port: " << get_config("port") << "\n";
    std::cout << "  debug: " << get_config("debug") << "\n";
    std::cout << "  timeout: " << get_config("timeout", "30") << " (默认值)\n";

    // ---- 与 ranges 配合 ----
    std::cout << "\n-- 与其他 C++20 特性配合 --\n";
    std::set<int> valid_ids = {1, 3, 5, 7, 9};
    std::vector<int> request_ids = {1, 2, 3, 4, 5, 6, 7};

    std::cout << "有效请求: ";
    for (int id : request_ids) {
        if (valid_ids.contains(id)) {
            std::cout << id << " ";
        }
    }
    std::cout << "\n";
}

// ===================== pitfalls() =====================
void pitfalls() {
    std::cout << "\n--- 易错点 ---\n\n";

    // 陷阱1：contains 只判断存在性，不返回迭代器
    std::cout << "1. contains() 只返回 bool:\n";
    std::cout << "   如果需要获取值，仍需使用 find():\n";
    std::map<std::string, int> m = {{"key", 42}};
    if (m.contains("key")) {
        // 已知存在，但还需要再找一次才能获取值
        // 效率上有双重查找的开销
        std::cout << "   contains + []: " << m["key"] << "\n";
    }
    // 更好的方式（如果需要值）：
    if (auto it = m.find("key"); it != m.end()) {
        std::cout << "   find + ->second: " << it->second << " (只查找一次)\n";
    }
    std::cout << "\n";

    // 陷阱2：vector/array 没有 contains
    std::cout << "2. vector/array 没有 contains() 成员函数:\n";
    std::cout << "   只有关联容器有 contains()\n";
    std::cout << "   对于 vector，使用 std::find 或 std::ranges::find\n";
    std::vector<int> v = {1, 2, 3, 4, 5};
    bool found = std::find(v.begin(), v.end(), 3) != v.end();
    std::cout << "   vector 查找 3: " << std::boolalpha << found << "\n\n";

    // 陷阱3：multimap/multiset 的 contains 只判断是否存在
    std::cout << "3. multimap/multiset 的 contains 不告诉你有几个:\n";
    std::multiset<int> ms = {1, 1, 2, 2, 2, 3};
    std::cout << "   multiset{1,1,2,2,2,3}.contains(2) = " << ms.contains(2) << "\n";
    std::cout << "   multiset{1,1,2,2,2,3}.count(2) = " << ms.count(2) << "\n";
    std::cout << "   如果需要计数，仍使用 count()\n\n";

    // 陷阱4：与 count 的性能对比
    std::cout << "4. contains() vs count() 的性能:\n";
    std::cout << "   对于 set/map: 两者性能相同（O(log n)）\n";
    std::cout << "   对于 multiset/multimap: contains 可能更快\n";
    std::cout << "   因为 contains 找到一个就返回，count 要数完所有\n\n";

    // 陷阱5：透明比较器
    std::cout << "5. 使用透明比较器可以避免临时对象:\n";
    std::set<std::string, std::less<>> transparent_set = {"hello", "world"};
    // 使用 std::less<> 后，可以直接用 const char* 查找
    bool has_hello = transparent_set.contains("hello");  // 不创建临时 string
    std::cout << "   透明比较器查找: " << has_hello << "\n";
    std::cout << "   避免了从 const char* 构造临时 std::string\n";
}

// ===================== main() =====================
int main() {
    std::cout << "===== 关联容器 contains() - C++20 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
