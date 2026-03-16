---
sidebar_position: 3
slug: 02-multidim-subscript
title: "多维下标运算符 operator[]"
description: "C++23 多维下标运算符 operator[] 教学示例"
tags: [cpp23, multidim_subscript]
---

# 多维下标运算符 operator[]

C++23 标准引入的 多维下标运算符 operator[] 特性完整教学示例。

## 完整源码

```cpp title="cpp23/02_multidim_subscript.cpp"
/**
 * ============================================================
 * 特性名称: 多维下标运算符 operator[]
 * 所属标准: C++23
 * ============================================================
 *
 * [是什么]: C++23 允许 operator[] 接受多个参数，从而可以用
 *          m[i, j] 的语法直接访问多维数组元素，而不再需要
 *          m[i][j] 或 m(i, j) 的写法。
 *
 * [为什么引入]: 传统 C++ 中 operator[] 只能接受一个参数，
 *              导致多维容器需要使用 proxy 对象（m[i][j]）或
 *              改用 operator()（m(i,j)）。多参数 operator[]
 *              让语法更自然，与数学表示更一致。
 *
 * [关键要点]:
 *   - operator[] 现在可以接受零个或多个参数
 *   - 可以用于自定义矩阵类、张量类等多维数据结构
 *   - 旧的单参数 operator[] 仍然完全兼容
 *   - 逗号表达式在 [] 中已被废弃（C++20 起），C++23 中
 *     逗号分隔的参数被视为多个参数
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <array>
#include <cassert>
#include <cstddef>

// ============================================================
// 特性检测
// ============================================================
#ifdef __cpp_multidimensional_subscript
#define HAS_MULTIDIM_SUBSCRIPT 1
#else
#define HAS_MULTIDIM_SUBSCRIPT 0
#endif

#if HAS_MULTIDIM_SUBSCRIPT

// ============================================================
// 基础用法
// ============================================================
namespace basic_usage {

// 简单的二维矩阵类
template <typename T>
class Matrix {
public:
    Matrix(std::size_t rows, std::size_t cols)
        : rows_(rows), cols_(cols), data_(rows * cols) {}

    // C++23 多维下标运算符
    T& operator[](std::size_t row, std::size_t col) {
        return data_[row * cols_ + col];
    }

    const T& operator[](std::size_t row, std::size_t col) const {
        return data_[row * cols_ + col];
    }

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

    // 打印矩阵
    void print(const char* name) const {
        std::cout << "  " << name << " (" << rows_ << "x" << cols_ << "):" << std::endl;
        for (std::size_t i = 0; i < rows_; ++i) {
            std::cout << "    [";
            for (std::size_t j = 0; j < cols_; ++j) {
                if (j > 0) std::cout << ", ";
                std::cout << data_[i * cols_ + j];
            }
            std::cout << "]" << std::endl;
        }
    }

private:
    std::size_t rows_, cols_;
    std::vector<T> data_;
};

void demonstrate() {
    std::cout << "[基础用法]" << std::endl;

    // 创建 3x3 矩阵
    Matrix<int> mat(3, 3);

    // 使用 m[i, j] 语法赋值 —— 这是 C++23 新语法！
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            mat[i, j] = static_cast<int>(i * 10 + j);
        }
    }
    mat.print("mat");

    // 直接用 m[i, j] 读取
    std::cout << "  mat[1, 2] = " << mat[1, 2] << std::endl;
    std::cout << "  mat[2, 0] = " << mat[2, 0] << std::endl;

    std::cout << std::endl;
}

} // namespace basic_usage

// ============================================================
// 进阶用法
// ============================================================
namespace advanced_usage {

// 三维张量类
template <typename T>
class Tensor3D {
public:
    Tensor3D(std::size_t d1, std::size_t d2, std::size_t d3)
        : d1_(d1), d2_(d2), d3_(d3), data_(d1 * d2 * d3, T{}) {}

    // 三参数下标运算符
    T& operator[](std::size_t i, std::size_t j, std::size_t k) {
        return data_[(i * d2_ + j) * d3_ + k];
    }

    const T& operator[](std::size_t i, std::size_t j, std::size_t k) const {
        return data_[(i * d2_ + j) * d3_ + k];
    }

private:
    std::size_t d1_, d2_, d3_;
    std::vector<T> data_;
};

// 带边界检查的安全矩阵
template <typename T>
class SafeMatrix {
public:
    SafeMatrix(std::size_t rows, std::size_t cols, T default_val = T{})
        : rows_(rows), cols_(cols), data_(rows * cols, default_val) {}

    // 带边界检查的下标运算符
    T& operator[](std::size_t row, std::size_t col) {
        if (row >= rows_ || col >= cols_) {
            std::cerr << "  [错误] 越界访问 [" << row << ", " << col
                      << "]，矩阵大小为 " << rows_ << "x" << cols_ << std::endl;
            // 返回第一个元素作为安全值（生产代码应抛异常）
            return data_[0];
        }
        return data_[row * cols_ + col];
    }

    const T& operator[](std::size_t row, std::size_t col) const {
        if (row >= rows_ || col >= cols_) {
            static T dummy{};
            return dummy;
        }
        return data_[row * cols_ + col];
    }

private:
    std::size_t rows_, cols_;
    std::vector<T> data_;
};

// 与 operator() 对比的类
template <typename T>
class MatrixCompare {
public:
    MatrixCompare(std::size_t rows, std::size_t cols)
        : rows_(rows), cols_(cols), data_(rows * cols, T{}) {}

    // C++23 风格：operator[]
    T& operator[](std::size_t row, std::size_t col) {
        return data_[row * cols_ + col];
    }

    // 传统风格：operator()
    T& operator()(std::size_t row, std::size_t col) {
        return data_[row * cols_ + col];
    }

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }

private:
    std::size_t rows_, cols_;
    std::vector<T> data_;
};

void demonstrate() {
    std::cout << "[进阶用法]" << std::endl;

    // 三维张量
    std::cout << "  -- 三维张量 Tensor3D --" << std::endl;
    Tensor3D<double> tensor(2, 3, 4);
    tensor[1, 2, 3] = 3.14;
    tensor[0, 1, 2] = 2.71;
    std::cout << "  tensor[1, 2, 3] = " << tensor[1, 2, 3] << std::endl;
    std::cout << "  tensor[0, 1, 2] = " << tensor[0, 1, 2] << std::endl;

    // 安全矩阵（带边界检查）
    std::cout << "  -- 安全矩阵 SafeMatrix --" << std::endl;
    SafeMatrix<int> safe(3, 3, 0);
    safe[1, 1] = 99;
    std::cout << "  safe[1, 1] = " << safe[1, 1] << std::endl;
    safe[5, 5] = 42;  // 越界访问，会输出错误信息

    // 对比 operator[] 和 operator()
    std::cout << "  -- operator[] vs operator() 对比 --" << std::endl;
    MatrixCompare<int> mc(3, 3);
    mc[0, 1] = 10;       // C++23 新语法
    mc(1, 0) = 20;       // 传统 operator() 语法
    std::cout << "  mc[0, 1] = " << mc[0, 1] << " （使用 operator[]）" << std::endl;
    std::cout << "  mc(1, 0) = " << mc(1, 0) << " （使用 operator()）" << std::endl;
    std::cout << "  两种方式等价，但 operator[] 更符合数组直觉" << std::endl;

    std::cout << std::endl;
}

} // namespace advanced_usage

// ============================================================
// 易错点与陷阱
// ============================================================
namespace pitfalls {

void demonstrate() {
    std::cout << "[易错点与陷阱]" << std::endl;

    // 陷阱1：与 C++20 之前的逗号表达式混淆
    std::cout << "  陷阱1: C++20 之前 a[i, j] 中的逗号是逗号表达式" << std::endl;
    std::cout << "    - C++17: a[i, j] 等同于 a[(i, j)] 即 a[j]" << std::endl;
    std::cout << "    - C++20: 废弃了 [] 中的逗号表达式" << std::endl;
    std::cout << "    - C++23: a[i, j] 变成多参数下标运算符调用" << std::endl;
    std::cout << "    - 这意味着旧代码行为可能改变！" << std::endl;

    // 陷阱2：不能混用不同参数数量
    std::cout << "  陷阱2: 可以重载不同参数数量的 operator[]" << std::endl;
    std::cout << "    - 可以同时有 operator[](size_t) 和 operator[](size_t, size_t)" << std::endl;
    std::cout << "    - 编译器通过参数数量区分调用" << std::endl;

    // 陷阱3：静态数组不受影响
    std::cout << "  陷阱3: 内置数组 int a[3][4] 不受影响" << std::endl;
    std::cout << "    - 内置多维数组仍然用 a[i][j]" << std::endl;
    std::cout << "    - 多维 operator[] 仅用于自定义类" << std::endl;

    // 陷阱4：零参数 operator[]
    std::cout << "  陷阱4: C++23 也允许零参数 operator[]" << std::endl;
    std::cout << "    - 可以写 obj[] 来调用无参数版本" << std::endl;
    std::cout << "    - 应用场景较少，但语法是合法的" << std::endl;

    std::cout << std::endl;
}

} // namespace pitfalls

#endif // HAS_MULTIDIM_SUBSCRIPT

// ============================================================
// 主函数
// ============================================================
int main() {
    std::cout << "===== 多维下标运算符 operator[] - C++23 =====\n" << std::endl;

#if HAS_MULTIDIM_SUBSCRIPT
    basic_usage::demonstrate();
    advanced_usage::demonstrate();
    pitfalls::demonstrate();
#else
    std::cout << "[提示] 当前编译器不支持多维下标运算符特性。" << std::endl;
    std::cout << "请使用支持 C++23 的编译器（如 GCC 14+ 或 Clang 18+）。" << std::endl;
    std::cout << std::endl;
    std::cout << "多维下标运算符语法示例（供参考）：" << std::endl;
    std::cout << R"(
    template <typename T>
    class Matrix {
    public:
        // C++23: operator[] 可以接受多个参数
        T& operator[](std::size_t row, std::size_t col) {
            return data_[row * cols_ + col];
        }
    };

    Matrix<int> mat(3, 3);
    mat[1, 2] = 42;        // C++23 新语法！
    int val = mat[1, 2];   // 直接用 [i, j] 访问

    // 与传统方式对比：
    // mat[1][2]   -> 需要 proxy 对象
    // mat(1, 2)   -> 使用 operator()
    // mat[1, 2]   -> C++23 最自然的方式
)" << std::endl;
#endif

    return 0;
}
```

## 参考链接

- [cppreference: 多维下标运算符 operator[]](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_subscript_operator)
