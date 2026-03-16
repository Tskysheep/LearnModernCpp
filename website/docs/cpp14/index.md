---
sidebar_position: 1
title: "C++14 新特性概览"
description: "C++14 是对 C++11 的小幅改进和修补，放松了 constexpr 限制，增加了泛型 Lambda 和一些实用工具"
tags: [C++14, 现代C++, 泛型Lambda, constexpr, make_unique]
---

# C++14 新特性概览

> C++14 是对 C++11 的小幅改进和修补，放松了 constexpr 限制，增加了泛型 Lambda 和一些实用工具。

:::tip C++14 定位
C++14 被称为 C++11 的 "bug fix release"。它没有引入颠覆性的新特性，而是对 C++11 进行打磨和补充。本章包含 **11 个独立可编译示例**。
:::

---

## 特性总览

| 编号 | 文件 | 特性 | 分类 |
|:----:|------|------|------|
| 01 | `01_generic_lambdas.cpp` | 泛型 Lambda | 语言核心 |
| 02 | `02_return_type_deduction.cpp` | 返回类型推导 | 语言核心 |
| 03 | `03_variable_templates.cpp` | 变量模板 | 模板 |
| 04 | `04_binary_literals_separators.cpp` | 二进制字面量、数字分隔符 | 字面量 |
| 05 | `05_relaxed_constexpr.cpp` | 放松的 constexpr | 编译期计算 |
| 06 | `06_make_unique.cpp` | make_unique | 智能指针 |
| 07 | `07_deprecated_attribute.cpp` | [[deprecated]] | 属性 |
| 08 | `08_shared_timed_mutex.cpp` | 读写锁 | 并发 |
| 09 | `09_integer_sequence.cpp` | integer_sequence | 元编程 |
| 10 | `10_exchange.cpp` | std::exchange | 工具 |
| 11 | `11_quoted.cpp` | std::quoted | I/O |

---

## 特性详解

### 01. 泛型 Lambda (`01_generic_lambdas.cpp`)

- Lambda 参数使用 `auto`，相当于模板函数
- `[](auto x, auto y) { return x + y; }` 可处理任意类型
- 使 Lambda 成为真正的多态可调用对象

:::tip 关键改进
C++11 的 Lambda 参数必须指定具体类型，C++14 的泛型 Lambda 让 Lambda 具备了与模板函数同等的泛型能力。
:::

:::info 参考链接
[lambda](https://en.cppreference.com/w/cpp/language/lambda)
:::

### 02. 函数返回类型推导 (`02_return_type_deduction.cpp`)

- 函数可以用 `auto` 作为返回类型，编译器自动推导
- `decltype(auto)` 保留引用和 cv 限定符
- 递归函数需要至少一个 return 在递归调用前

:::warning 注意
`auto` 返回类型推导会剥离引用和 const，如果需要保留这些限定符，请使用 `decltype(auto)`。
:::

:::info 参考链接
[auto](https://en.cppreference.com/w/cpp/language/auto) | [decltype(auto)](https://en.cppreference.com/w/cpp/language/auto#Return_type_deduction)
:::

### 03. 变量模板 (`03_variable_templates.cpp`)

- `template<typename T> constexpr T pi = T(3.14159265358979);`
- 类型参数化的常量值
- 简化 type_traits：`is_same_v<T, U>` 替代 `is_same<T, U>::value`

:::info 参考链接
[variable template](https://en.cppreference.com/w/cpp/language/variable_template)
:::

### 04. 二进制字面量与数字分隔符 (`04_binary_literals_separators.cpp`)

- `0b1010` 表示二进制数
- `1'000'000` 用单引号分隔提升可读性
- 分隔符可用于任何进制

:::tip 可读性提升
数字分隔符对大数值特别有用，例如 `0xFF'FF'FF'FF` 或 `3.141'592'653'589`。
:::

:::info 参考链接
[integer literal](https://en.cppreference.com/w/cpp/language/integer_literal)
:::

### 05. 放松的 constexpr (`05_relaxed_constexpr.cpp`)

- C++14 允许 constexpr 函数包含：
  - 局部变量声明
  - `if`/`switch` 条件语句
  - 循环（`for`、`while`）
  - 多条语句
- C++11 的 constexpr 函数只能有一条 return 语句

:::tip 重大改进
C++14 的 constexpr 放松使得编译期计算能力大幅增强，不再需要用递归模拟循环。
:::

:::info 参考链接
[constexpr](https://en.cppreference.com/w/cpp/language/constexpr)
:::

### 06. std::make_unique (`06_make_unique.cpp`)

- `auto p = std::make_unique<T>(args...)`
- 填补 C++11 的遗漏（当时只有 `make_shared`）
- 异常安全：避免 `new` 和 `unique_ptr` 构造之间的内存泄漏

:::warning 为什么不直接用 new？
`std::make_unique` 不仅更简洁，还提供了异常安全保证。在 `f(std::unique_ptr<A>(new A), g())` 中，如果 `g()` 在 `new A` 和 `unique_ptr` 构造之间抛出异常，会导致内存泄漏。`make_unique` 避免了这个问题。
:::

:::info 参考链接
[std::make_unique](https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique)
:::

### 07. [[deprecated]] (`07_deprecated_attribute.cpp`)

- `[[deprecated("使用新API")]]` 标记弃用的接口
- 编译器产生警告但不阻止编译
- 适用于函数、类、变量、枚举等

:::info 参考链接
[[[deprecated]]](https://en.cppreference.com/w/cpp/language/attributes/deprecated)
:::

### 08. 读写锁 (`08_shared_timed_mutex.cpp`)

- `std::shared_timed_mutex` + `std::shared_lock`
- 多个读者可以同时持有共享锁
- 写者需要独占锁
- 适合读多写少的场景

:::tip 使用场景
读写锁非常适合缓存系统、配置管理等读多写少的场景。C++17 进一步引入了 `std::shared_mutex`（不带超时功能但更轻量）。
:::

:::info 参考链接
[std::shared_timed_mutex](https://en.cppreference.com/w/cpp/thread/shared_timed_mutex) | [std::shared_lock](https://en.cppreference.com/w/cpp/thread/shared_lock)
:::

### 09. integer_sequence (`09_integer_sequence.cpp`)

- `std::index_sequence<0, 1, 2, ...>` 编译期整数序列
- `std::make_index_sequence<N>` 生成 0 到 N-1 的序列
- 用于 tuple 元素展开等元编程场景

:::info 参考链接
[std::integer_sequence](https://en.cppreference.com/w/cpp/utility/integer_sequence)
:::

### 10. std::exchange (`10_exchange.cpp`)

- `auto old = std::exchange(obj, new_value);` 替换并返回旧值
- 简化移动构造函数和移动赋值运算符
- 原子操作：先保存旧值，再设置新值

:::tip 实用技巧
`std::exchange` 在移动构造函数中特别有用，可以简洁地将源对象的成员置为默认值。
:::

:::info 参考链接
[std::exchange](https://en.cppreference.com/w/cpp/utility/exchange)
:::

### 11. std::quoted (`11_quoted.cpp`)

- `std::quoted(str)` 自动添加引号和转义
- 支持 I/O 流的往返（输出再输入不丢失信息）
- 可自定义引号字符和转义字符

:::info 参考链接
[std::quoted](https://en.cppreference.com/w/cpp/io/manip/quoted)
:::
