---
sidebar_position: 1
title: "C++11 新特性概览"
description: "C++11 是现代 C++ 的里程碑，引入了移动语义、Lambda、智能指针、并发支持等重大特性"
tags: [C++11, 现代C++, 移动语义, Lambda, 智能指针, 并发]
---

# C++11 新特性概览

> C++11 是现代 C++ 的里程碑，引入了移动语义、Lambda、智能指针、并发支持等重大特性，彻底改变了 C++ 编程方式。

:::tip C++11 的重要性
C++11 是现代 C++ 的起点。掌握 C++11 是学习后续所有 C++ 标准版本的基础。本章包含 **33 个独立可编译示例**，覆盖语言核心、模板、标准库、并发等所有方面。
:::

---

## 特性总览

| 分类 | 特性数量 | 核心特性 |
|------|:--------:|----------|
| 语言核心 | 19 | auto、Lambda、移动语义、完美转发 |
| 模板与元编程 | 4 | 可变参数模板、constexpr、type_traits |
| 标准库 - 容器与工具 | 7 | 智能指针、tuple、无序容器 |
| 字符串与 I/O | 2 | 字符串转换、正则表达式 |
| 并发 | 5 | thread、mutex、future、atomic |
| 其他 | 3 | chrono、random、attributes |

---

## 语言核心特性

### 01. auto 与 decltype (`01_auto_and_decltype.cpp`)

> **[→ 查看完整代码](./01-auto-and-decltype)**

- **auto** 让编译器自动推导变量类型，消除冗长的迭代器声明
- **decltype** 查询表达式的类型，用于泛型编程中的类型推导
- 尾置返回类型 `auto f() -> T` 支持依赖参数的返回类型

:::info 参考链接
[auto](https://en.cppreference.com/w/cpp/language/auto) | [decltype](https://en.cppreference.com/w/cpp/language/decltype)
:::

### 02. 范围 for 循环 (`02_range_based_for.cpp`)

> **[→ 查看完整代码](./02-range-based-for)**

- `for (auto& x : container)` 替代传统的迭代器循环
- 支持所有提供 `begin()`/`end()` 的类型
- 消除下标越界风险，代码更简洁

:::info 参考链接
[range-based for](https://en.cppreference.com/w/cpp/language/range-for)
:::

### 03. Lambda 表达式 (`03_lambda_expressions.cpp`)

> **[→ 查看完整代码](./03-lambda-expressions)**

- 语法：`[捕获](参数) -> 返回类型 { 函数体 }`
- 捕获方式：`[=]` 值捕获、`[&]` 引用捕获、`[this]` 捕获 this
- 替代冗长的仿函数类，直接传递行为给 STL 算法

:::info 参考链接
[lambda](https://en.cppreference.com/w/cpp/language/lambda)
:::

### 04. nullptr (`04_nullptr.cpp`)

> **[→ 查看完整代码](./04-nullptr)**

- 类型安全的空指针常量，类型为 `std::nullptr_t`
- 修复 `NULL`/`0` 在函数重载决议中的歧义

:::info 参考链接
[nullptr](https://en.cppreference.com/w/cpp/language/nullptr) | [std::nullptr_t](https://en.cppreference.com/w/cpp/types/nullptr_t)
:::

### 05. 强类型枚举 (`05_enum_class.cpp`)

> **[→ 查看完整代码](./05-enum-class)**

- `enum class` 不会隐式转换为整数
- 枚举值不会泄漏到外围作用域
- 可以指定底层类型

:::info 参考链接
[enum](https://en.cppreference.com/w/cpp/language/enum)
:::

### 06. 统一初始化 (`06_uniform_initialization.cpp`)

> **[→ 查看完整代码](./06-uniform-initialization)**

- 花括号 `{}` 提供一致的初始化语法
- 防止窄化转换（如 `int x{3.14}` 编译报错）
- 解决 most vexing parse 问题

:::warning 注意
花括号初始化在某些场景下会优先匹配 `initializer_list` 构造函数，需要注意。
:::

:::info 参考链接
[list initialization](https://en.cppreference.com/w/cpp/language/list_initialization) | [value initialization](https://en.cppreference.com/w/cpp/language/value_initialization)
:::

### 07. = default 与 = delete (`07_default_delete.cpp`)

> **[→ 查看完整代码](./07-default-delete)**

- `= default` 显式要求编译器生成默认实现
- `= delete` 禁止特定函数（如拷贝构造）
- 明确表达设计意图

:::info 参考链接
[default constructor](https://en.cppreference.com/w/cpp/language/default_constructor) | [deleted functions](https://en.cppreference.com/w/cpp/language/function#Deleted_functions)
:::

### 08. override 与 final (`08_override_final.cpp`)

> **[→ 查看完整代码](./08-override-final)**

- `override` 确保函数确实覆盖了基类虚函数
- `final` 阻止进一步继承或覆盖
- 编译器捕获签名不匹配的隐蔽 bug

:::info 参考链接
[override](https://en.cppreference.com/w/cpp/language/override) | [final](https://en.cppreference.com/w/cpp/language/final)
:::

### 09. 委托构造函数 (`09_delegating_constructors.cpp`)

> **[→ 查看完整代码](./09-delegating-constructors)**

- 一个构造函数可以调用同类的另一个构造函数
- 消除构造函数之间的代码重复

:::info 参考链接
[delegating constructor](https://en.cppreference.com/w/cpp/language/constructor#Delegating_constructor)
:::

### 10. 继承构造函数 (`10_inheriting_constructors.cpp`)

> **[→ 查看完整代码](./10-inheriting-constructors)**

- `using Base::Base;` 自动继承基类构造函数
- 避免在派生类中编写大量转发构造函数

:::info 参考链接
[inheriting constructors](https://en.cppreference.com/w/cpp/language/using_declaration#Inheriting_constructors)
:::

### 11. 移动语义 (`11_move_semantics.cpp`)

> **[→ 查看完整代码](./11-move-semantics)**

:::tip 核心特性
**C++11 最核心的特性之一**，理解移动语义是掌握现代 C++ 的关键。
:::

- 右值引用 `&&` 标识即将销毁的临时对象
- `std::move` 将左值转为右值引用，启用移动操作
- 移动构造/移动赋值转移资源所有权，避免深拷贝

:::info 参考链接
[move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) | [std::move](https://en.cppreference.com/w/cpp/utility/move) | [rvalue reference](https://en.cppreference.com/w/cpp/language/reference)
:::

### 12. 完美转发 (`12_perfect_forwarding.cpp`)

> **[→ 查看完整代码](./12-perfect-forwarding)**

- 万能引用 `T&&`（模板参数推导上下文）
- `std::forward<T>` 保持参数的值类别
- 引用折叠规则：`& + && = &`，`&& + && = &&`

:::info 参考链接
[std::forward](https://en.cppreference.com/w/cpp/utility/forward) | [reference collapsing](https://en.cppreference.com/w/cpp/language/reference#Reference_collapsing)
:::

### 13. noexcept (`13_noexcept.cpp`)

> **[→ 查看完整代码](./13-noexcept)**

- 标记函数不会抛出异常
- 容器在 `noexcept` 移动时才使用移动操作（否则回退到拷贝）
- `noexcept` 运算符检查表达式是否不抛异常

:::warning 重要
如果你的移动构造函数没有标记 `noexcept`，`std::vector` 在扩容时会使用拷贝而非移动，可能导致性能问题。
:::

:::info 参考链接
[noexcept specifier](https://en.cppreference.com/w/cpp/language/noexcept_spec) | [noexcept operator](https://en.cppreference.com/w/cpp/language/noexcept)
:::

### 14. static_assert (`14_static_assert.cpp`)

> **[→ 查看完整代码](./14-static-assert)**

- 编译期断言，失败时输出自定义错误信息
- 常与 `type_traits` 配合检查模板参数

:::info 参考链接
[static_assert](https://en.cppreference.com/w/cpp/language/static_assert)
:::

### 15. 原始字符串字面量 (`15_raw_string_literals.cpp`)

> **[→ 查看完整代码](./15-raw-string-literals)**

- `R"(内容)"` 中的反斜杠不是转义符
- 正则表达式、Windows 路径、多行字符串的救星

:::info 参考链接
[string literal](https://en.cppreference.com/w/cpp/language/string_literal)
:::

### 16. Unicode 字面量 (`16_unicode_literals.cpp`)

> **[→ 查看完整代码](./16-unicode-literals)**

- `u8""`、`u""`、`U""` 分别对应 UTF-8、UTF-16、UTF-32
- 新增 `char16_t`、`char32_t` 类型

:::info 参考链接
[string literal](https://en.cppreference.com/w/cpp/language/string_literal) | [char16_t](https://en.cppreference.com/w/cpp/language/types#char16_t)
:::

### 17. 用户自定义字面量 (`17_user_defined_literals.cpp`)

> **[→ 查看完整代码](./17-user-defined-literals)**

- `operator""` 实现自定义后缀（如 `10_km`、`3.14_deg`）
- 标准库提供了 `s`（string）、`ms`（chrono）等

:::info 参考链接
[user-defined literals](https://en.cppreference.com/w/cpp/language/user_literal)
:::

### 18. using 类型别名 (`18_type_aliases.cpp`)

> **[→ 查看完整代码](./18-type-aliases)**

- `using Name = Type;` 替代 `typedef`
- 支持模板别名：`template<typename T> using Vec = std::vector<T>;`

:::info 参考链接
[type alias](https://en.cppreference.com/w/cpp/language/type_alias)
:::

### 19. alignof 与 alignas (`19_alignof_alignas.cpp`)

> **[→ 查看完整代码](./19-alignof-alignas)**

- `alignof` 查询类型的对齐要求
- `alignas` 指定变量/类型的对齐方式
- 缓存行对齐等性能优化场景

:::info 参考链接
[alignof](https://en.cppreference.com/w/cpp/language/alignof) | [alignas](https://en.cppreference.com/w/cpp/language/alignas)
:::

---

## 模板与元编程

### 20. 可变参数模板 (`20_variadic_templates.cpp`)

> **[→ 查看完整代码](./20-variadic-templates)**

- `template<typename... Args>` 接受任意数量的模板参数
- 递归展开参数包
- `sizeof...(Args)` 获取参数个数

:::info 参考链接
[parameter pack](https://en.cppreference.com/w/cpp/language/parameter_pack)
:::

### 21. constexpr (`21_constexpr.cpp`)

> **[→ 查看完整代码](./21-constexpr)**

- `constexpr` 函数可在编译期求值
- C++11 限制较多（单条 return 语句）
- `constexpr` 变量必须在编译期初始化

:::info 参考链接
[constexpr](https://en.cppreference.com/w/cpp/language/constexpr)
:::

### 22. type_traits (`22_type_traits.cpp`)

> **[→ 查看完整代码](./22-type-traits)**

- 编译期类型内省：`is_integral`、`is_same`、`is_pointer` 等
- `enable_if` 实现 SFINAE
- `conditional`、`remove_const` 等类型变换

:::info 参考链接
[type_traits](https://en.cppreference.com/w/cpp/header/type_traits) | [enable_if](https://en.cppreference.com/w/cpp/types/enable_if)
:::

### 23. initializer_list (`23_initializer_list.cpp`)

> **[→ 查看完整代码](./23-initializer-list)**

- `std::initializer_list<T>` 支持花括号初始化语法
- `vector<int> v = {1, 2, 3};` 的底层机制

:::info 参考链接
[std::initializer_list](https://en.cppreference.com/w/cpp/utility/initializer_list)
:::

---

## 标准库 - 容器与工具

### 40. 智能指针 (`40_smart_pointers.cpp`)

> **[→ 查看完整代码](./40-smart-pointers)**

:::tip 核心特性
智能指针是现代 C++ 内存管理的基石，**务必掌握**。
:::

- **unique_ptr**：独占所有权，零开销抽象
- **shared_ptr**：共享所有权，引用计数
- **weak_ptr**：打破循环引用
- `make_shared`/`make_unique`（C++14）更高效

:::info 参考链接
[unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) | [shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) | [weak_ptr](https://en.cppreference.com/w/cpp/memory/weak_ptr)
:::

### 41. tuple (`41_tuple.cpp`)

> **[→ 查看完整代码](./41-tuple)**

- 异构值的固定大小集合
- `std::tie` 解包到变量
- 支持比较、哈希

:::info 参考链接
[std::tuple](https://en.cppreference.com/w/cpp/utility/tuple) | [std::tie](https://en.cppreference.com/w/cpp/utility/tuple/tie)
:::

### 42. std::array (`42_array.cpp`)

> **[→ 查看完整代码](./42-array)**

- 固定大小数组，带 STL 接口
- `at()` 提供边界检查
- 替代 C 风格数组

:::info 参考链接
[std::array](https://en.cppreference.com/w/cpp/container/array)
:::

### 43. 无序容器 (`43_unordered_containers.cpp`)

> **[→ 查看完整代码](./43-unordered-containers)**

- `unordered_map`/`unordered_set` 基于哈希表
- O(1) 平均查找时间
- 自定义类型需要提供哈希函数

:::info 参考链接
[unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map) | [unordered_set](https://en.cppreference.com/w/cpp/container/unordered_set)
:::

### 44. function 与 bind (`44_function_and_bind.cpp`)

> **[→ 查看完整代码](./44-function-and-bind)**

- `std::function` 是类型擦除的通用可调用包装器
- `std::bind` 绑定部分参数
- 现代 C++ 中 Lambda 已大量替代 bind

:::info 参考链接
[std::function](https://en.cppreference.com/w/cpp/utility/functional/function) | [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind)
:::

### 45. forward_list (`45_forward_list.cpp`)

> **[→ 查看完整代码](./45-forward-list)**

- 单向链表，最小内存开销
- 只支持前向迭代
- `insert_after`/`erase_after` 操作

:::info 参考链接
[std::forward_list](https://en.cppreference.com/w/cpp/container/forward_list)
:::

### 46. reference_wrapper (`46_ref_wrapper.cpp`)

> **[→ 查看完整代码](./46-ref-wrapper)**

- `std::ref`/`std::cref` 创建引用包装器
- 解决模板/线程中引用传递的问题

:::info 参考链接
[std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper) | [std::ref](https://en.cppreference.com/w/cpp/utility/functional/ref)
:::

---

## 字符串与 I/O

### 60. 字符串转换 (`60_string_conversions.cpp`)

> **[→ 查看完整代码](./60-string-conversions)**

- `std::to_string` 数值转字符串
- `std::stoi`/`stol`/`stof` 等字符串转数值
- 替代不安全的 `sprintf`/`sscanf`

:::info 参考链接
[std::to_string](https://en.cppreference.com/w/cpp/string/basic_string/to_string) | [std::stoi](https://en.cppreference.com/w/cpp/string/basic_string/stol)
:::

### 61. 正则表达式 (`61_regex.cpp`)

> **[→ 查看完整代码](./61-regex)**

- `regex_match`：完整匹配
- `regex_search`：搜索子串
- `regex_replace`：替换

:::warning 性能注意
编译 regex 开销较大，应尽量复用已编译的正则表达式对象。
:::

:::info 参考链接
[regex](https://en.cppreference.com/w/cpp/regex) | [std::regex_match](https://en.cppreference.com/w/cpp/regex/regex_match)
:::

---

## 并发

### 70. 线程 (`70_thread.cpp`)

> **[→ 查看完整代码](./70-thread)**

- C++ 首个标准线程库
- `std::thread` 创建线程
- `join()` 等待结束、`detach()` 分离

:::info 参考链接
[std::thread](https://en.cppreference.com/w/cpp/thread/thread)
:::

### 71. 互斥锁 (`71_mutex_and_locks.cpp`)

> **[→ 查看完整代码](./71-mutex-and-locks)**

- `std::mutex` 保护共享数据
- `lock_guard` RAII 自动锁管理
- `unique_lock` 更灵活的锁控制

:::info 参考链接
[std::mutex](https://en.cppreference.com/w/cpp/thread/mutex) | [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard) | [std::unique_lock](https://en.cppreference.com/w/cpp/thread/unique_lock)
:::

### 72. 条件变量 (`72_condition_variable.cpp`)

> **[→ 查看完整代码](./72-condition-variable)**

- 生产者-消费者模式的标准实现
- `wait`/`notify_one`/`notify_all`

:::warning 虚假唤醒
条件变量可能发生虚假唤醒（spurious wakeup），必须在循环中检查条件或使用带谓词的 `wait` 重载。
:::

:::info 参考链接
[std::condition_variable](https://en.cppreference.com/w/cpp/thread/condition_variable)
:::

### 73. future 与 promise (`73_future_promise.cpp`)

> **[→ 查看完整代码](./73-future-promise)**

- `std::async` 启动异步任务
- `promise`-`future` 跨线程通信通道
- `packaged_task` 包装可调用对象

:::info 参考链接
[std::future](https://en.cppreference.com/w/cpp/thread/future) | [std::promise](https://en.cppreference.com/w/cpp/thread/promise) | [std::async](https://en.cppreference.com/w/cpp/thread/async)
:::

### 74. 原子操作 (`74_atomic.cpp`)

> **[→ 查看完整代码](./74-atomic)**

- `std::atomic<T>` 无锁原子操作
- 内存序控制并发访问的可见性
- 实现无锁计数器等

:::info 参考链接
[std::atomic](https://en.cppreference.com/w/cpp/atomic/atomic) | [memory order](https://en.cppreference.com/w/cpp/atomic/memory_order)
:::

---

## 其他

### 80. chrono (`80_chrono.cpp`)

> **[→ 查看完整代码](./80-chrono)**

- `duration`：类型安全的时间段
- `time_point`：时间点
- `steady_clock`：单调时钟（计时首选）

:::info 参考链接
[chrono](https://en.cppreference.com/w/cpp/chrono) | [std::chrono::duration](https://en.cppreference.com/w/cpp/chrono/duration) | [std::chrono::steady_clock](https://en.cppreference.com/w/cpp/chrono/steady_clock)
:::

### 81. 随机数 (`81_random.cpp`)

> **[→ 查看完整代码](./81-random)**

- `mt19937`：梅森旋转引擎
- `uniform_int_distribution` 等分布
- 替代有缺陷的 `rand()`

:::warning 不要使用 rand()
`rand()` 存在分布不均、线程不安全等问题，应使用 `<random>` 头文件中的设施。
:::

:::info 参考链接
[random](https://en.cppreference.com/w/cpp/numeric/random) | [std::mt19937](https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine)
:::

### 90. 属性 (`90_attributes.cpp`)

> **[→ 查看完整代码](./90-attributes)**

- `[[noreturn]]`：标记不会返回的函数
- C++11 引入的标准化属性语法

:::info 参考链接
[attributes](https://en.cppreference.com/w/cpp/language/attributes) | [[[noreturn]]](https://en.cppreference.com/w/cpp/language/attributes/noreturn)
:::
