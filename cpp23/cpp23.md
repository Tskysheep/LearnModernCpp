# C++23 特性详解

> C++23 继续完善现代 C++，引入了 std::expected、deducing this、std::print、std::mdspan 等重要特性。部分特性编译器支持仍在进行中。

---

## 可编译示例

### 01. if consteval (`01_if_consteval.cpp`)
- 在 `constexpr` 函数中检测当前是否处于编译期求值
- `if consteval { /* 编译期路径 */ } else { /* 运行期路径 */ }`
- 替代 `std::is_constant_evaluated()`（C++20），语法更清晰
- 参考: [if consteval](https://en.cppreference.com/w/cpp/language/if#Consteval_if)

### 02. 多维下标运算符 (`02_multidim_subscript.cpp`)
- `operator[](size_t i, size_t j)` 支持多个参数
- `matrix[i, j]` 替代 `matrix(i, j)` 或 `matrix[i][j]`
- 更直观的多维数组访问
- 参考: [operator[]](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_subscript_operator)

### 03. size_t 字面量 (`03_size_t_literal.cpp`)
- `42uz` 或 `42zu` 后缀产生 `size_t` 类型
- 消除有符号/无符号比较的警告
- `for (auto i = 0uz; i < v.size(); ++i)`
- 参考: [integer literal](https://en.cppreference.com/w/cpp/language/integer_literal)

### 05. Lambda 改进 (`05_lambda_improvements.cpp`)
- 带属性时可以省略空参数列表的 `()`
- `static` Lambda：无状态 Lambda 可标记为 static
- 参考: [lambda](https://en.cppreference.com/w/cpp/language/lambda)

### 40. std::expected ⭐ (`40_expected.cpp`)
- `std::expected<T, E>` 表示"值或错误"
- 替代异常的错误处理方式
- 支持 monadic 操作：`and_then`、`transform`、`or_else`
- 比返回错误码更类型安全，比异常更高效
- 参考: [std::expected](https://en.cppreference.com/w/cpp/utility/expected)

### 41. optional monadic (`41_optional_monadic.cpp`)
- `optional::and_then(f)`：有值时调用 f（f 返回 optional）
- `optional::transform(f)`：有值时调用 f（f 返回值）
- `optional::or_else(f)`：无值时调用 f
- 函数式风格的链式操作
- 参考: [optional::and_then](https://en.cppreference.com/w/cpp/utility/optional/and_then) | [optional::transform](https://en.cppreference.com/w/cpp/utility/optional/transform) | [optional::or_else](https://en.cppreference.com/w/cpp/utility/optional/or_else)

### 42. move_only_function (`42_move_only_function.cpp`)
- `std::move_only_function` 可存储仅移动的可调用对象
- `std::function` 要求可调用对象可拷贝
- 适合存储捕获了 `unique_ptr` 的 Lambda
- 参考: [std::move_only_function](https://en.cppreference.com/w/cpp/utility/functional/move_only_function)

### 43. to_underlying (`43_to_underlying.cpp`)
- `std::to_underlying(enum_value)` 转换枚举到底层类型
- 替代 `static_cast<std::underlying_type_t<E>>(e)`
- 更简洁、更安全
- 参考: [std::to_underlying](https://en.cppreference.com/w/cpp/utility/to_underlying)

### 44. std::unreachable (`44_unreachable.cpp`)
- 标记不可达的代码路径
- 允许编译器进行更积极的优化
- 未定义行为（如果实际到达则是 bug）
- 参考: [std::unreachable](https://en.cppreference.com/w/cpp/utility/unreachable)

### 61. stacktrace (`61_stacktrace.cpp`)
- 运行时获取调用栈信息
- 用于错误诊断和调试
- 编译器支持程度不一
- 参考: [std::stacktrace](https://en.cppreference.com/w/cpp/utility/basic_stacktrace)

### 70. byteswap (`70_byteswap.cpp`)
- `std::byteswap(value)` 反转字节序
- 用于网络编程（大小端转换）
- constexpr 友好
- 参考: [std::byteswap](https://en.cppreference.com/w/cpp/numeric/byteswap)

### 90. [[assume]] (`90_attributes.cpp`)
- `[[assume(expr)]]` 告诉编译器某条件一定为真
- 编译器可以据此进行优化
- 如果假设不成立是未定义行为
- 参考: [[[assume]]](https://en.cppreference.com/w/cpp/language/attributes/assume)

---

## 仅文档说明的特性

以下特性在当前主流编译器（GCC 14、Clang 18、MSVC 2022）中支持不完整，仅在此文档中说明。

### Deducing this（显式对象参数）

消除 const/非const 成员函数重载的代码重复。

参考: [deducing this](https://en.cppreference.com/w/cpp/language/member_functions#Explicit_object_parameter)

```cpp
// C++23 之前需要写两个版本：
struct Widget {
    int& get() { return data; }
    const int& get() const { return data; }
    int data;
};

// C++23 用 deducing this 一个版本搞定：
struct Widget {
    // this auto&& self 是显式对象参数
    auto&& get(this auto&& self) {
        return std::forward_like<decltype(self)>(self.data);
    }
    int data;
};
```

**关键点：**
- `this auto&& self` 作为第一个参数
- 编译器根据调用者的 const 性自动推导 self 的类型
- 也可用于 CRTP 简化

### std::print / std::println

替代 `std::cout` 的现代输出方式。

参考: [std::print](https://en.cppreference.com/w/cpp/io/print) | [std::println](https://en.cppreference.com/w/cpp/io/println)

```cpp
#include <print>

int main() {
    std::println("Hello, {}!", "World");      // 自动换行
    std::print("pi = {:.4f}\n", 3.14159);     // 手动换行
    std::println("{:>10}", 42);                // 右对齐
}
```

**关键点：**
- 基于 `std::format` 的格式化语法
- `println` 自动追加换行符
- 比 `cout` 更高效（减少了虚函数调用和 locale 处理）
- 可以输出到任意输出流

### std::mdspan（多维数组视图）

非拥有的多维数组视图，替代手动的多维索引计算。

参考: [std::mdspan](https://en.cppreference.com/w/cpp/container/mdspan)

```cpp
#include <mdspan>

void process_matrix(std::mdspan<float, std::dextents<size_t, 2>> mat) {
    for (size_t i = 0; i < mat.extent(0); ++i)
        for (size_t j = 0; j < mat.extent(1); ++j)
            mat[i, j] *= 2.0f;
}

int main() {
    std::vector<float> data(12);
    // 将一维数据解释为 3x4 矩阵
    auto mat = std::mdspan(data.data(), 3, 4);
    process_matrix(mat);
}
```

**关键点：**
- 支持任意维度
- 支持多种布局策略（行优先、列优先、自定义步幅）
- 与 `std::span` 类似但支持多维
- 科学计算和图像处理的利器

### std::flat_map / std::flat_set

缓存友好的有序关联容器。

参考: [std::flat_map](https://en.cppreference.com/w/cpp/container/flat_map) | [std::flat_set](https://en.cppreference.com/w/cpp/container/flat_set)

```cpp
#include <flat_map>
#include <flat_set>

int main() {
    std::flat_map<std::string, int> scores;
    scores["Alice"] = 95;
    scores["Bob"] = 87;

    std::flat_set<int> s = {3, 1, 4, 1, 5};
    // 底层使用排序的 vector，迭代更快
}
```

**关键点：**
- 底层使用排序的连续容器（通常是 `vector`）
- 查找 O(log n)（二分搜索），但缓存更友好
- 插入/删除 O(n)，适合读多写少的场景
- 内存占用比 `std::map`（红黑树）更小

### std::generator

标准的惰性生成器协程类型。

参考: [std::generator](https://en.cppreference.com/w/cpp/coroutine/generator)

```cpp
#include <generator>

std::generator<int> fibonacci() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;
        auto next = a + b;
        a = b;
        b = next;
    }
}

int main() {
    for (auto n : fibonacci()) {
        if (n > 100) break;
        std::println("{}", n);
    }
}
```

**关键点：**
- C++20 协程缺少标准的 Generator 类型，需要手写 promise_type
- C++23 提供了 `std::generator<T>` 开箱即用
- 支持 `ranges` 接口
- 可以用于递归生成器（`std::generator<T>::yielded`）

### static operator()

无状态仿函数的零开销调用。

参考: [static operator()](https://en.cppreference.com/w/cpp/language/operators#Function_call_operator)

```cpp
struct Hasher {
    static size_t operator()(int x) noexcept {
        return x * 2654435761u;
    }
};

// Lambda 也可以是 static
auto add = [](int a, int b) static { return a + b; };
```

**关键点：**
- 无状态可调用对象不需要 this 指针
- `static operator()` 可以被直接调用，无需对象实例
- 编译器可以更好地优化

### std::ranges 新增视图

C++23 大幅扩展了 ranges 视图库。

参考: [ranges](https://en.cppreference.com/w/cpp/ranges) | [views::chunk](https://en.cppreference.com/w/cpp/ranges/chunk_view) | [views::zip](https://en.cppreference.com/w/cpp/ranges/zip_view) | [views::enumerate](https://en.cppreference.com/w/cpp/ranges/enumerate_view)

```cpp
#include <ranges>

// chunk: 分块
auto chunks = v | std::views::chunk(3);
// [1,2,3], [4,5,6], [7,8,9]

// slide: 滑动窗口
auto windows = v | std::views::slide(3);
// [1,2,3], [2,3,4], [3,4,5], ...

// zip: 多个范围并行组合
auto zipped = std::views::zip(names, scores);

// enumerate: 带索引
for (auto [i, val] : v | std::views::enumerate) {
    std::println("[{}] = {}", i, val);
}

// chunk_by: 按条件分组
auto groups = v | std::views::chunk_by(std::less{});

// join_with: 用分隔符连接
auto joined = strs | std::views::join_with(',');

// cartesian_product: 笛卡尔积
for (auto [x, y] : std::views::cartesian_product(xs, ys)) {
    // ...
}
```

**新增视图列表：**
| 视图 | 功能 | 参考 |
|------|------|------|
| `chunk(n)` | 按固定大小 n 分块 | [chunk_view](https://en.cppreference.com/w/cpp/ranges/chunk_view) |
| `slide(n)` | 大小为 n 的滑动窗口 | [slide_view](https://en.cppreference.com/w/cpp/ranges/slide_view) |
| `zip` | 并行组合多个范围 | [zip_view](https://en.cppreference.com/w/cpp/ranges/zip_view) |
| `zip_transform` | 组合并转换 | [zip_transform_view](https://en.cppreference.com/w/cpp/ranges/zip_transform_view) |
| `enumerate` | 带索引遍历 | [enumerate_view](https://en.cppreference.com/w/cpp/ranges/enumerate_view) |
| `chunk_by(pred)` | 按谓词分组 | [chunk_by_view](https://en.cppreference.com/w/cpp/ranges/chunk_by_view) |
| `join_with(sep)` | 用分隔符连接 | [join_with_view](https://en.cppreference.com/w/cpp/ranges/join_with_view) |
| `stride(n)` | 每隔 n 个取一个 | [stride_view](https://en.cppreference.com/w/cpp/ranges/stride_view) |
| `cartesian_product` | 笛卡尔积 | [cartesian_product_view](https://en.cppreference.com/w/cpp/ranges/cartesian_product_view) |
| `as_rvalue` | 转为右值视图 | [as_rvalue_view](https://en.cppreference.com/w/cpp/ranges/as_rvalue_view) |
| `as_const` | 转为 const 视图 | [as_const_view](https://en.cppreference.com/w/cpp/ranges/as_const_view) |
| `repeat(val)` | 无限重复 | [repeat_view](https://en.cppreference.com/w/cpp/ranges/repeat_view) |
| `repeat(val, n)` | 有限重复 | [repeat_view](https://en.cppreference.com/w/cpp/ranges/repeat_view) |
