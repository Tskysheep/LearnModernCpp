# C++20 特性详解

> C++20 是继 C++11 后最大的标准更新，引入了 Concepts、Ranges、Coroutines、Modules 四大核心特性，以及大量标准库改进。

---

## 语言核心特性

### 01. Concepts 基础 ⭐ (`01_concepts_basics.cpp`)
- `concept` 定义对模板参数的约束
- `requires` 子句替代 SFINAE，错误信息更清晰
- 标准库提供 `std::integral`、`std::floating_point` 等
- 参考: [concepts](https://en.cppreference.com/w/cpp/language/constraints) | [standard concepts](https://en.cppreference.com/w/cpp/concepts)

### 02. Concepts 进阶 (`02_concepts_advanced.cpp`)
- 自定义 concept 组合（`&&`、`||`）
- concept 细化（refinement）
- subsumption 规则决定重载优先级
- 参考: [requires clause](https://en.cppreference.com/w/cpp/language/constraints#Requires_clauses)

### 03. requires 表达式 (`03_requires_expressions.cpp`)
- 简单需求：`requires { expr; }`
- 类型需求：`requires { typename T::type; }`
- 复合需求：`requires { { expr } -> concept; }`
- 嵌套需求：`requires { requires concept<T>; }`
- 参考: [requires expression](https://en.cppreference.com/w/cpp/language/requires)

### 04. 简写函数模板 (`04_abbreviated_templates.cpp`)
- `void f(auto x)` 等价于 `template<typename T> void f(T x)`
- `void f(std::integral auto x)` 带约束的简写
- 使简单模板的声明更简洁
- 参考: [abbreviated function template](https://en.cppreference.com/w/cpp/language/function_template#Abbreviated_function_template)

### 05. 三路比较 <=> ⭐ (`05_spaceship_operator.cpp`)
- `auto operator<=>(const T&) const = default;` 生成全部 6 个比较
- 三种排序类别：strong_ordering、weak_ordering、partial_ordering
- 大幅减少比较运算符的样板代码
- 参考: [three-way comparison](https://en.cppreference.com/w/cpp/language/operator_comparison#Three-way_comparison) | [std::strong_ordering](https://en.cppreference.com/w/cpp/utility/compare/strong_ordering)

### 06. 指定初始化器 (`06_designated_initializers.cpp`)
- `Point p{.x = 1, .y = 2};` 命名成员初始化
- 必须按成员声明顺序
- 自文档化的初始化方式
- 参考: [aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization#Designated_initializers)

### 07. consteval (`07_consteval.cpp`)
- `consteval` 函数**保证**在编译期执行
- 与 `constexpr` 的区别：constexpr 是"可以"，consteval 是"必须"
- 用于编译期验证、代码生成
- 参考: [consteval](https://en.cppreference.com/w/cpp/language/consteval)

### 08. constinit (`08_constinit.cpp`)
- 保证变量在编译期初始化
- 防止 Static Initialization Order Fiasco
- 只约束初始化，变量本身可以修改
- 参考: [constinit](https://en.cppreference.com/w/cpp/language/constinit)

### 09. 模板 Lambda (`09_template_lambdas.cpp`)
- `[]<typename T>(T x) { ... }` 在 Lambda 中使用显式模板参数
- 比 `auto` 参数更精确的类型控制
- 支持 concept 约束
- 参考: [lambda](https://en.cppreference.com/w/cpp/language/lambda)

### 10. 圆括号聚合初始化 (`10_aggregate_parens_init.cpp`)
- 聚合类型现在可以用圆括号初始化
- 使 `make_shared<Aggregate>(args...)` 和 `emplace` 可以操作聚合体
- 参考: [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization)

---

## Ranges

### 20. Ranges 基础 ⭐ (`20_ranges_basics.cpp`)
- `std::ranges::sort(v)` 替代 `std::sort(v.begin(), v.end())`
- 投影（projection）：`ranges::sort(v, {}, &Person::name)`
- 更安全：自动检查范围有效性
- 参考: [ranges](https://en.cppreference.com/w/cpp/ranges) | [ranges::sort](https://en.cppreference.com/w/cpp/algorithm/ranges/sort)

### 21. Ranges 视图 ⭐ (`21_ranges_views.cpp`)
- 惰性求值的数据转换管道
- 管道语法：`v | filter(pred) | transform(fn) | take(5)`
- 常用视图：`iota`、`filter`、`transform`、`take`、`drop`、`reverse`
- 参考: [views](https://en.cppreference.com/w/cpp/ranges#Range_adaptors) | [views::filter](https://en.cppreference.com/w/cpp/ranges/filter_view) | [views::transform](https://en.cppreference.com/w/cpp/ranges/transform_view)

### 22. 自定义 Range (`22_ranges_custom.cpp`)
- 实现满足 `range` concept 的自定义类型
- 自定义视图适配器
- 参考: [range concept](https://en.cppreference.com/w/cpp/ranges/range) | [view_interface](https://en.cppreference.com/w/cpp/ranges/view_interface)

---

## 协程

### 30. 协程基础 (`30_coroutines_basics.cpp`)
- `co_await`：挂起等待异步操作
- `co_yield`：生成值并挂起
- `co_return`：结束协程并返回值
- 需要自定义 `promise_type` 和 coroutine handle
- 参考: [coroutines](https://en.cppreference.com/w/cpp/language/coroutines) | [coroutine_handle](https://en.cppreference.com/w/cpp/coroutine/coroutine_handle)

### 31. 协程 Generator (`31_coroutines_generator.cpp`)
- 实现惰性序列生成器
- `co_yield` 逐个产生值
- 无限序列、按需计算
- 参考: [co_yield](https://en.cppreference.com/w/cpp/language/coroutines#co_yield)

---

## 标准库

### 40. std::span (`40_span.cpp`)
- 连续内存的非拥有视图
- 替代 `(T* ptr, size_t size)` 参数对
- 静态 extent 和动态 extent
- 参考: [std::span](https://en.cppreference.com/w/cpp/container/span)

### 41. std::format (`41_format.cpp`)
- 类型安全的字符串格式化
- `std::format("Hello, {}!", name)` 替代 printf
- 支持位置参数、对齐、填充、精度
- 参考: [std::format](https://en.cppreference.com/w/cpp/utility/format/format)

### 42. 日历与时区 (`42_calendar_timezone.cpp`)
- `std::chrono::year_month_day` 日期类型
- 日期算术：加减天、月、年
- 时区转换（编译器支持程度不一）
- 参考: [chrono calendar](https://en.cppreference.com/w/cpp/chrono#Calendar) | [year_month_day](https://en.cppreference.com/w/cpp/chrono/year_month_day)

### 43. source_location (`43_source_location.cpp`)
- `std::source_location::current()` 获取调用位置
- 替代 `__FILE__`、`__LINE__` 宏
- 在日志、断言中使用
- 参考: [std::source_location](https://en.cppreference.com/w/cpp/utility/source_location)

### 44. bit_cast (`44_bit_cast.cpp`)
- `std::bit_cast<To>(from)` 安全的位模式转换
- constexpr 友好
- 替代 `reinterpret_cast` 和 `memcpy`
- 参考: [std::bit_cast](https://en.cppreference.com/w/cpp/numeric/bit_cast)

### 45. 数学常量 (`45_math_constants.cpp`)
- `std::numbers::pi`、`std::numbers::e` 等
- 模板化：`std::numbers::pi_v<float>`
- 替代自定义的 `#define PI`
- 参考: [std::numbers](https://en.cppreference.com/w/cpp/numeric/constants)

### 46. starts_with/ends_with (`46_string_operations.cpp`)
- `str.starts_with("prefix")` 和 `str.ends_with("suffix")`
- 清晰直观，替代手动子串比较
- 参考: [starts_with](https://en.cppreference.com/w/cpp/string/basic_string/starts_with) | [ends_with](https://en.cppreference.com/w/cpp/string/basic_string/ends_with)

### 47. contains (`47_contains.cpp`)
- `map.contains(key)` 替代 `map.find(key) != map.end()`
- 适用于所有关联容器和无序容器
- 参考: [map::contains](https://en.cppreference.com/w/cpp/container/map/contains) | [set::contains](https://en.cppreference.com/w/cpp/container/set/contains)

---

## 并发

### 70. jthread (`70_jthread.cpp`)
- 析构时自动 `join()`（RAII）
- 内置 `stop_token` 取消机制
- `stop_callback` 注册取消回调
- 参考: [std::jthread](https://en.cppreference.com/w/cpp/thread/jthread) | [std::stop_token](https://en.cppreference.com/w/cpp/thread/stop_token)

### 71. barrier、latch (`71_barrier_latch.cpp`)
- `std::latch`：一次性同步屏障
- `std::barrier`：可重用同步屏障
- `std::counting_semaphore`：计数信号量
- 参考: [std::latch](https://en.cppreference.com/w/cpp/thread/latch) | [std::barrier](https://en.cppreference.com/w/cpp/thread/barrier) | [std::counting_semaphore](https://en.cppreference.com/w/cpp/thread/counting_semaphore)

---

## 属性

### 90. 属性 (`90_attributes.cpp`)
- `[[likely]]`/`[[unlikely]]`：分支预测提示
- `[[no_unique_address]]`：空成员不占用额外空间（EBO for members）
- 参考: [[[likely]]](https://en.cppreference.com/w/cpp/language/attributes/likely) | [[[no_unique_address]]](https://en.cppreference.com/w/cpp/language/attributes/no_unique_address)
