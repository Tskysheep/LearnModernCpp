---
sidebar_position: 51
slug: 50-filesystem
title: "std::filesystem (文件系统库)"
description: "C++17 std::filesystem (文件系统库) 教学示例"
tags: [cpp17, filesystem]
---

# std::filesystem (文件系统库)

C++17 标准引入的 std::filesystem (文件系统库) 特性完整教学示例。

## 完整源码

```cpp title="cpp17/50_filesystem.cpp"
/**
 * ============================================================
 * 特性名称: std::filesystem (文件系统库)
 * 所属标准: C++17
 * ============================================================
 *
 * [是什么]: 标准库文件系统库，提供路径操作、文件/目录遍历、
 *          文件状态查询、创建/删除/拷贝等功能的跨平台 API。
 *
 * [为什么引入]: C++14 及以前没有标准的文件系统操作方式，
 *              只能依赖 POSIX API 或 Boost.Filesystem。
 *              C++17 将 Boost.Filesystem 纳入标准库，提供
 *              了跨平台的统一接口。
 *
 * [关键要点]:
 *   - #include <filesystem>，namespace fs = std::filesystem
 *   - fs::path 是核心类，表示文件路径
 *   - fs::directory_iterator / fs::recursive_directory_iterator 遍历目录
 *   - fs::exists / fs::is_regular_file / fs::is_directory 查询文件状态
 *   - fs::create_directories / fs::remove / fs::copy 管理文件
 *   - 大部分操作有两个版本：抛异常版和 error_code 版
 * ============================================================
 */

#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

// ============================================================
// 辅助函数
// ============================================================
std::string format_size(uintmax_t size) {
    const char* units[] = {"B", "KB", "MB", "GB"};
    int unit = 0;
    double dsize = static_cast<double>(size);
    while (dsize >= 1024.0 && unit < 3) {
        dsize /= 1024.0;
        ++unit;
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << dsize << " " << units[unit];
    return oss.str();
}

// ============================================================
// 基础用法
// ============================================================
void basic_usage() {
    std::cout << "--- basic_usage ---\n";

    // 1. 路径操作
    fs::path p1 = "/home/user/documents/file.txt";
    fs::path p2 = "C:/Users/test/file.cpp";
    fs::path p3 = fs::current_path();

    std::cout << "路径: " << p1 << "\n";
    std::cout << "文件名: " << p1.filename() << "\n";
    std::cout << "主干名: " << p1.stem() << "\n";
    std::cout << "扩展名: " << p1.extension() << "\n";
    std::cout << "父路径: " << p1.parent_path() << "\n";
    std::cout << "根路径: " << p1.root_path() << "\n";

    // 2. 路径拼接
    fs::path base = "/home/user";
    fs::path full = base / "projects" / "demo" / "main.cpp";
    std::cout << "\n拼接路径: " << full << "\n";

    // 3. 路径替换
    fs::path src = "project/main.cpp";
    fs::path header = src;
    header.replace_extension(".h");
    std::cout << "替换扩展名: " << header << "\n";

    // 4. 当前工作目录
    std::cout << "\n当前目录: " << fs::current_path() << "\n";

    // 5. 检查文件/目录是否存在
    std::cout << "当前目录存在: " << fs::exists(fs::current_path()) << "\n";
    std::cout << "/不存在的路径 存在: " << fs::exists("/不存在的路径") << "\n";
}

// ============================================================
// 进阶用法
// ============================================================
void advanced_usage() {
    std::cout << "\n--- advanced_usage ---\n";

    // 创建临时测试目录
    fs::path test_dir = fs::current_path() / "fs_test_demo";

    // 1. 创建目录（递归创建）
    fs::create_directories(test_dir / "sub1" / "deep");
    fs::create_directories(test_dir / "sub2");
    std::cout << "创建目录: " << test_dir << "\n";

    // 2. 创建测试文件
    {
        std::ofstream(test_dir / "readme.txt") << "这是测试文件";
        std::ofstream(test_dir / "main.cpp") << "#include <iostream>";
        std::ofstream(test_dir / "sub1" / "data.csv") << "a,b,c\n1,2,3";
        std::ofstream(test_dir / "sub1" / "deep" / "config.json") << "{}";
        std::ofstream(test_dir / "sub2" / "notes.txt") << "笔记";
    }
    std::cout << "创建了测试文件\n";

    // 3. 遍历目录（非递归）
    std::cout << "\n目录内容 (非递归):\n";
    for (const auto& entry : fs::directory_iterator(test_dir)) {
        auto status = entry.status();
        std::string type = fs::is_directory(status) ? "[目录]" : "[文件]";
        std::cout << "  " << type << " " << entry.path().filename();
        if (fs::is_regular_file(status)) {
            std::cout << " (" << format_size(entry.file_size()) << ")";
        }
        std::cout << "\n";
    }

    // 4. 递归遍历目录
    std::cout << "\n目录内容 (递归):\n";
    for (const auto& entry : fs::recursive_directory_iterator(test_dir)) {
        // 计算相对路径的深度用于缩进
        auto rel = fs::relative(entry.path(), test_dir);
        int depth = 0;
        for (auto it = rel.begin(); it != rel.end(); ++it) ++depth;

        std::string indent(depth * 2, ' ');
        std::string type = fs::is_directory(entry) ? "[目录]" : "[文件]";
        std::cout << "  " << indent << type << " " << entry.path().filename() << "\n";
    }

    // 5. 文件状态查询
    fs::path file = test_dir / "readme.txt";
    std::cout << "\n文件状态查询: " << file.filename() << "\n";
    std::cout << "  存在: " << fs::exists(file) << "\n";
    std::cout << "  是普通文件: " << fs::is_regular_file(file) << "\n";
    std::cout << "  是目录: " << fs::is_directory(file) << "\n";
    std::cout << "  文件大小: " << fs::file_size(file) << " 字节\n";

    // 6. 拷贝文件
    fs::path copied = test_dir / "readme_copy.txt";
    fs::copy_file(file, copied, fs::copy_options::overwrite_existing);
    std::cout << "\n拷贝文件: " << file.filename() << " -> " << copied.filename() << "\n";

    // 7. 重命名/移动文件
    fs::path renamed = test_dir / "readme_renamed.txt";
    fs::rename(copied, renamed);
    std::cout << "重命名: readme_copy.txt -> readme_renamed.txt\n";

    // 8. 使用 error_code 版本（不抛异常）
    std::error_code ec;
    auto size = fs::file_size(test_dir / "不存在的文件.txt", ec);
    if (ec) {
        std::cout << "\nerror_code 版本: " << ec.message() << "\n";
    } else {
        std::cout << "文件大小: " << size << "\n";
    }

    // 9. 遍历并收集特定类型文件
    std::vector<fs::path> cpp_files;
    for (const auto& entry : fs::recursive_directory_iterator(test_dir)) {
        if (entry.path().extension() == ".cpp") {
            cpp_files.push_back(entry.path());
        }
    }
    std::cout << "\n找到 " << cpp_files.size() << " 个 .cpp 文件\n";
    for (const auto& f : cpp_files) {
        std::cout << "  " << fs::relative(f, test_dir) << "\n";
    }

    // 10. 获取磁盘空间信息
    auto space = fs::space(fs::current_path());
    std::cout << "\n磁盘空间:\n";
    std::cout << "  总容量: " << format_size(space.capacity) << "\n";
    std::cout << "  可用: " << format_size(space.available) << "\n";
    std::cout << "  空闲: " << format_size(space.free) << "\n";

    // 清理测试目录
    auto removed = fs::remove_all(test_dir);
    std::cout << "\n清理: 删除了 " << removed << " 个文件/目录\n";
}

// ============================================================
// 易错点 / 陷阱
// ============================================================
void pitfalls() {
    std::cout << "\n--- pitfalls ---\n";

    // 陷阱 1: 路径分隔符
    //   Windows 用 '\'，Linux/macOS 用 '/'
    //   fs::path 的 / 运算符会自动处理
    fs::path p = fs::path("dir") / "subdir" / "file.txt";
    std::cout << "陷阱1 - 跨平台路径: " << p << "\n";
    std::cout << "  通用格式: " << p.generic_string() << "\n";

    // 陷阱 2: 异常 vs error_code
    //   默认版本出错抛 filesystem_error 异常
    //   error_code 版本不抛异常，适合性能敏感场景
    try {
        fs::file_size("/绝对不存在的文件");
    } catch (const fs::filesystem_error& e) {
        std::cout << "陷阱2 - 异常版本: " << e.what() << "\n";
    }

    std::error_code ec;
    fs::file_size("/绝对不存在的文件", ec);
    std::cout << "  error_code 版本: " << ec.message() << "\n";

    // 陷阱 3: 符号链接
    //   directory_iterator 默认跟随符号链接
    //   递归遍历时可能陷入循环
    std::cout << "陷阱3 - 递归遍历要注意符号链接循环\n";
    std::cout << "  使用 directory_options::follow_directory_symlink 控制\n";

    // 陷阱 4: 路径比较是字符串比较，不考虑规范化
    fs::path a = "dir/./file.txt";
    fs::path b = "dir/file.txt";
    std::cout << "陷阱4 - 路径比较: \"" << a.generic_string()
              << "\" == \"" << b.generic_string()
              << "\": " << (a == b) << "\n";
    // 使用 canonical() 或 weakly_canonical() 进行规范化比较
    // 但要求路径存在

    // 陷阱 5: file_size 对目录和特殊文件行为未定义
    std::cout << "陷阱5 - file_size() 只对普通文件有意义\n";

    // 陷阱 6: 线程安全
    //   文件系统操作不是原子的，多线程/多进程可能竞争
    //   "检查然后操作" 模式（TOCTOU）是常见 bug
    std::cout << "陷阱6 - 文件系统操作不是原子的 (TOCTOU 风险)\n";
    std::cout << "  if (exists(f)) read(f);  // f 可能在检查后被删除\n";
}

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== std::filesystem (文件系统库) - C++17 =====\n\n";

    basic_usage();
    advanced_usage();
    pitfalls();

    return 0;
}
```

## 参考链接

- [cppreference: std::filesystem (文件系统库)](https://en.cppreference.com/w/cpp/filesystem)
