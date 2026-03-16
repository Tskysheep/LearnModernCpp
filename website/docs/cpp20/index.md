---
sidebar_position: 1
title: "C++20 新特性概览"
description: "C++20 是继 C++11 后最大的标准更新，引入了 Concepts、Ranges、Coroutines、Modules 四大核心特性"
tags: [C++20, 现代C++, Concepts, Ranges, Coroutines, format, span, jthread]
---

# C++20 新特性概览

> C++20 是继 C++11 后最大的标准更新，引入了 Concepts、Ranges、Coroutines、Modules 四大核心特性，以及大量标准库改进。

:::tip C++20 的重要性
C++20 是现代 C++ 的又一个里程碑，被称为 "C++11 以来最大的更新"。本章包含 **23 个独立可编译示例**，涵盖四大核心特性和丰富的标准库增强。
:::

---

## 特性总览

| 分类 | 特性数量 | 核心特性 |
|------|:--------:|----------|
| 语言核心 | 10 | Concepts、三路比较 `<=>` |
| Ranges | 3 | Ranges 基础、视图、自定义 Range |
| 协程 | 2 | 协程基础、Generator |
| 标准库 | 8 | span、format、日历时区 |
| 并发 | 2 | jthread、barrier/latch |
| 属性 | 1 | [[likely]]、[[no_unique_address]] |

---

## 语言核心特性

### 01. Concepts 基础 (`01_concepts_basics.cpp`)

> **[→ 查看完整代码](./01-concepts-basics)**

:::tip 核心特性
Concepts 是 C++20 最重要的语言特性，彻底改变了模板编程的方式。
:::

- `concept` 定义对模板参数的约束
- `requires` 子句替代 SFINAE，错误信息更清晰
- 标准库提供 `std::integral`、`std::floating_point` 等

:::info 参考链接
[concepts](https://en.cppreference.com/w/cpp/language/constraints) | [standard concepts](https://en.cppreference.com/w/cpp/concepts)
:::

### 02. Concepts 进阶 (`02_concepts_advanced.cpp`)

> **[→ 查看完整代码](./02-concepts-advanced)**

- 自定义 concept 组合（`&&`、`||`）
- concept 细化（refinement）
- subsumption 规则决定重载优先级

:::info 参考链接
[requires clause](https://en.cppreference.com/w/cpp/language/constraints#Requires_clauses)
:::

### 03. requires 表达式 (`03_requires_expressions.cpp`)

> **[→ 查看完整代码](./03-requires-expressions)**

- 简单需求：`requires { expr; }`
- 类型需求：`requires { typename T::type; }`
- 复合需求：`requires { { expr } -> concept; }`
- 嵌套需求：`requires { requires concept<T>; }`

:::info 参考链接
[requires expression](https://en.cppreference.com/w/cpp/language/requires)
:::

### 04. 简写函数模板 (`04_abbreviated_templates.cpp`)

> **[→ 查看完整代码](./04-abbreviated-templates)**

- `void f(auto x)` 等价于 `template<typename T> void f(T x)`
- `void f(std::integral auto x)` 带约束的简写
- 使简单模板的声明更简洁

:::info 参考链接
[abbreviated function template](https://en.cppreference.com/w/cpp/language/function_template#Abbreviated_function_template)
:::

### 05. 三路比较 `<=>` (`05_spaceship_operator.cpp`)

> **[→ 查看完整代码](./05-spaceship-operator)**

:::tip 核心特性
三路比较运算符 `<=>` 可以一行代码生成全部 6 个比较运算符。
:::

- `auto operator<=>(const T&) const = default;` 生成全部 6 个比较
- 三种排序类别：strong_ordering、weak_ordering、partial_ordering
- 大幅减少比较运算符的样板代码

:::info 参考链接
[three-way comparison](https://en.cppreference.com/w/cpp/language/operator_comparison#Three-way_comparison) | [std::strong_ordering](https://en.cppreference.com/w/cpp/utility/compare/strong_ordering)
:::

### 06. 指定初始化器 (`06_designated_initializers.cpp`)

> **[→ 查看完整代码](./06-designated-initializers)**

- `Point p{.x = 1, .y = 2};` 命名成员初始化
- 必须按成员声明顺序
- 自文档化的初始化方式

:::warning 注意
指定初始化器必须按照成员在结构体中的声明顺序使用，不能跳过或乱序。
:::

:::info 参考链接
[aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization#Designated_initializers)
:::

### 07. consteval (`07_consteval.cpp`)

> **[→ 查看完整代码](./07-consteval)**

- `consteval` 函数**保证**在编译期执行
- 与 `constexpr` 的区别：constexpr 是"可以"，consteval 是"必须"
- 用于编译期验证、代码生成

:::info 参考链接
[consteval](https://en.cppreference.com/w/cpp/language/consteval)
:::

### 08. constinit (`08_constinit.cpp`)

> **[→ 查看完整代码](./08-constinit)**

- 保证变量在编译期初始化
- 防止 Static Initialization Order Fiasco
- 只约束初始化，变量本身可以修改

:::info 参考链接
[constinit](https://en.cppreference.com/w/cpp/language/constinit)
:::

### 09. 模板 Lambda (`09_template_lambdas.cpp`)

> **[→ 查看完整代码](./09-template-lambdas)**

- `[]<typename T>(T x) { ... }` 在 Lambda 中使用显式模板参数
- 比 `auto` 参数更精确的类型控制
- 支持 concept 约束

:::info 参考链接
[lambda](https://en.cppreference.com/w/cpp/language/lambda)
:::

### 10. 圆括号聚合初始化 (`10_aggregate_parens_init.cpp`)

> **[→ 查看完整代码](./10-aggregate-parens-init)**

- 聚合类型现在可以用圆括号初始化
- 使 `make_shared<Aggregate>(args...)` 和 `emplace` 可以操作聚合体

:::info 参考链接
[direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization)
:::

---

## Ranges

### 20. Ranges 基础 (`20_ranges_basics.cpp`)

> **[→ 查看完整代码](./20-ranges-basics)**

:::tip 核心特性
Ranges 是 C++20 对 STL 算法的全面现代化改造。
:::

- `std::ranges::sort(v)` 替代 `std::sort(v.begin(), v.end())`
- 投影（projection）：`ranges::sort(v, {}, &Person::name)`
- 更安全：自动检查范围有效性

:::info 参考链接
[ranges](https://en.cppreference.com/w/cpp/ranges) | [ranges::sort](https://en.cppreference.com/w/cpp/algorithm/ranges/sort)
:::

### 21. Ranges 视图 (`21_ranges_views.cpp`)

> **[→ 查看完整代码](./21-ranges-views)**

:::tip 核心特性
Ranges 视图提供了函数式编程风格的数据处理管道。
:::

- 惰性求值的数据转换管道
- 管道语法：`v | filter(pred) | transform(fn) | take(5)`
- 常用视图：`iota`、`filter`、`transform`、`take`、`drop`、`reverse`

:::info 参考链接
[views](https://en.cppreference.com/w/cpp/ranges#Range_adaptors) | [views::filter](https://en.cppreference.com/w/cpp/ranges/filter_view) | [views::transform](https://en.cppreference.com/w/cpp/ranges/transform_view)
:::

### 22. 自定义 Range (`22_ranges_custom.cpp`)

> **[→ 查看完整代码](./22-ranges-custom)**

- 实现满足 `range` concept 的自定义类型
- 自定义视图适配器

:::info 参考链接
[range concept](https://en.cppreference.com/w/cpp/ranges/range) | [view_interface](https://en.cppreference.com/w/cpp/ranges/view_interface)
:::

---

## 协程

### 30. 协程基础 (`30_coroutines_basics.cpp`)

> **[→ 查看完整代码](./30-coroutines-basics)**

- `co_await`：挂起等待异步操作
- `co_yield`：生成值并挂起
- `co_return`：结束协程并返回值
- 需要自定义 `promise_type` 和 coroutine handle

:::warning 使用门槛
C++20 协程的底层机制较复杂，需要自定义 promise_type。建议先理解基本概念，实际使用时可依赖第三方库或等待 C++23 的 `std::generator`。
:::

:::info 参考链接
[coroutines](https://en.cppreference.com/w/cpp/language/coroutines) | [coroutine_handle](https://en.cppreference.com/w/cpp/coroutine/coroutine_handle)
:::

### 31. 协程 Generator (`31_coroutines_generator.cpp`)

> **[→ 查看完整代码](./31-coroutines-generator)**

- 实现惰性序列生成器
- `co_yield` 逐个产生值
- 无限序列、按需计算

:::info 参考链接
[co_yield](https://en.cppreference.com/w/cpp/language/coroutines#co_yield)
:::

---

## 标准库

### 40. std::span (`40_span.cpp`)

> **[→ 查看完整代码](./40-span)**

- 连续内存的非拥有视图
- 替代 `(T* ptr, size_t size)` 参数对
- 静态 extent 和动态 extent

:::info 参考链接
[std::span](https://en.cppreference.com/w/cpp/container/span)
:::

### 41. std::format (`41_format.cpp`)

> **[→ 查看完整代码](./41-format)**

- 类型安全的字符串格式化
- `std::format("Hello, {}!", name)` 替代 printf
- 支持位置参数、对齐、填充、精度

:::tip 现代格式化
`std::format` 结合了 printf 的便捷性和 iostream 的类型安全性，是 C++20 中最实用的标准库新增之一。
:::

:::info 参考链接
[std::format](https://en.cppreference.com/w/cpp/utility/format/format)
:::

### 42. 日历与时区 (`42_calendar_timezone.cpp`)

> **[→ 查看完整代码](./42-calendar-timezone)**

- `std::chrono::year_month_day` 日期类型
- 日期算术：加减天、月、年
- 时区转换（编译器支持程度不一）

:::info 参考链接
[chrono calendar](https://en.cppreference.com/w/cpp/chrono#Calendar) | [year_month_day](https://en.cppreference.com/w/cpp/chrono/year_month_day)
:::

### 43. source_location (`43_source_location.cpp`)

> **[→ 查看完整代码](./43-source-location)**

- `std::source_location::current()` 获取调用位置
- 替代 `__FILE__`、`__LINE__` 宏
- 在日志、断言中使用

:::info 参考链接
[std::source_location](https://en.cppreference.com/w/cpp/utility/source_location)
:::

### 44. bit_cast (`44_bit_cast.cpp`)

> **[→ 查看完整代码](./44-bit-cast)**

- `std::bit_cast<To>(from)` 安全的位模式转换
- constexpr 友好
- 替代 `reinterpret_cast` 和 `memcpy`

:::info 参考链接
[std::bit_cast](https://en.cppreference.com/w/cpp/numeric/bit_cast)
:::

### 45. 数学常量 (`45_math_constants.cpp`)

> **[→ 查看完整代码](./45-math-constants)**

- `std::numbers::pi`、`std::numbers::e` 等
- 模板化：`std::numbers::pi_v<float>`
- 替代自定义的 `#define PI`

:::info 参考链接
[std::numbers](https://en.cppreference.com/w/cpp/numeric/constants)
:::

### 46. starts_with/ends_with (`46_string_operations.cpp`)

> **[→ 查看完整代码](./46-string-operations)**

- `str.starts_with("prefix")` 和 `str.ends_with("suffix")`
- 清晰直观，替代手动子串比较

:::info 参考链接
[starts_with](https://en.cppreference.com/w/cpp/string/basic_string/starts_with) | [ends_with](https://en.cppreference.com/w/cpp/string/basic_string/ends_with)
:::

### 47. contains (`47_contains.cpp`)

> **[→ 查看完整代码](./47-contains)**

- `map.contains(key)` 替代 `map.find(key) != map.end()`
- 适用于所有关联容器和无序容器

:::info 参考链接
[map::contains](https://en.cppreference.com/w/cpp/container/map/contains) | [set::contains](https://en.cppreference.com/w/cpp/container/set/contains)
:::

---

## 并发

### 70. jthread (`70_jthread.cpp`)

> **[→ 查看完整代码](./70-jthread)**

- 析构时自动 `join()`（RAII）
- 内置 `stop_token` 取消机制
- `stop_callback` 注册取消回调

:::tip 推荐替代
`std::jthread` 是 `std::thread` 的安全替代品。它自动管理线程生命周期，避免了忘记 `join()` 或 `detach()` 导致的 `std::terminate()` 调用。
:::

:::info 参考链接
[std::jthread](https://en.cppreference.com/w/cpp/thread/jthread) | [std::stop_token](https://en.cppreference.com/w/cpp/thread/stop_token)
:::

### 71. barrier、latch (`71_barrier_latch.cpp`)

> **[→ 查看完整代码](./71-barrier-latch)**

- `std::latch`：一次性同步屏障
- `std::barrier`：可重用同步屏障
- `std::counting_semaphore`：计数信号量

:::info 参考链接
[std::latch](https://en.cppreference.com/w/cpp/thread/latch) | [std::barrier](https://en.cppreference.com/w/cpp/thread/barrier) | [std::counting_semaphore](https://en.cppreference.com/w/cpp/thread/counting_semaphore)
:::

---

## 属性

### 90. 属性 (`90_attributes.cpp`)

> **[→ 查看完整代码](./90-attributes)**

- `[[likely]]`/`[[unlikely]]`：分支预测提示
- `[[no_unique_address]]`：空成员不占用额外空间（EBO for members）

:::info 参考链接
[[[likely]]](https://en.cppreference.com/w/cpp/language/attributes/likely) | [[[no_unique_address]]](https://en.cppreference.com/w/cpp/language/attributes/no_unique_address)
:::
