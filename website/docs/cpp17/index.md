---
sidebar_position: 1
title: "C++17 新特性概览"
description: "C++17 引入了结构化绑定、if constexpr、optional/variant/any 词汇类型、string_view 和 filesystem 等实用特性"
tags: [C++17, 现代C++, optional, variant, string_view, filesystem, 结构化绑定]
---

# C++17 新特性概览

> C++17 引入了结构化绑定、if constexpr、optional/variant/any 词汇类型、string_view 和 filesystem 等实用特性。

:::tip C++17 亮点
C++17 是一次"实用升级"，重点在于**词汇类型**（optional/variant/any）和各种便利特性。本章包含 **16 个独立可编译示例**。
:::

---

## 特性总览

| 分类 | 特性 | 核心程度 |
|------|------|:--------:|
| 语言核心 | 结构化绑定、if/switch 初始化、CTAD、折叠表达式、if constexpr、inline 变量、嵌套命名空间、\_\_has\_include | 结构化绑定、if constexpr 为核心 |
| 词汇类型 | optional、variant、any、string_view、byte、invoke/apply | optional、variant、string_view 为核心 |
| 文件系统与算法 | filesystem、并行算法、clamp/gcd/lcm | filesystem 为核心 |
| 属性 | [[nodiscard]]、[[maybe_unused]]、[[fallthrough]] | - |

---

## 语言核心特性

### 01. 结构化绑定 (`01_structured_bindings.cpp`)

> **[→ 查看完整代码](./01-structured-bindings)**

:::tip 核心特性
结构化绑定是 C++17 最常用的语法改进之一。
:::

- `auto [x, y] = std::make_pair(1, 2);`
- 适用于 pair、tuple、struct、数组
- 消除 `.first`/`.second` 样板代码

:::info 参考链接
[structured bindings](https://en.cppreference.com/w/cpp/language/structured_binding)
:::

### 02. if/switch 初始化语句 (`02_if_switch_init.cpp`)

> **[→ 查看完整代码](./02-if-switch-init)**

- `if (auto it = map.find(key); it != map.end()) { ... }`
- 变量作用域限制在 if/switch 块内
- 代码更紧凑，减少变量泄漏

:::info 参考链接
[if statement](https://en.cppreference.com/w/cpp/language/if) | [switch statement](https://en.cppreference.com/w/cpp/language/switch)
:::

### 03. 类模板参数推导 CTAD (`03_ctad.cpp`)

> **[→ 查看完整代码](./03-ctad)**

- `std::pair p{1, 3.14};` 无需写 `std::pair<int, double>`
- `std::vector v{1, 2, 3};` 自动推导为 `vector<int>`
- 可自定义推导指引（deduction guide）

:::info 参考链接
[class template argument deduction](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction)
:::

### 04. 折叠表达式 (`04_fold_expressions.cpp`)

> **[→ 查看完整代码](./04-fold-expressions)**

- `(args + ...)` 一元右折叠
- `(... + args)` 一元左折叠
- 一行代码替代递归的可变参数模板展开

:::tip 简化模板
折叠表达式极大地简化了可变参数模板的代码量，不再需要递归终止函数。
:::

:::info 参考链接
[fold expression](https://en.cppreference.com/w/cpp/language/fold)
:::

### 05. if constexpr (`05_constexpr_if.cpp`)

> **[→ 查看完整代码](./05-constexpr-if)**

:::tip 核心特性
`if constexpr` 是模板编程的重大简化，能够替代大量 SFINAE 和标签分发的代码。
:::

- 编译期条件分支，未选中的分支不会被实例化
- 替代 SFINAE 和标签分发
- 模板编程的重大简化

:::info 参考链接
[if constexpr](https://en.cppreference.com/w/cpp/language/if#Constexpr_if)
:::

### 06. inline 变量 (`06_inline_variables.cpp`)

> **[→ 查看完整代码](./06-inline-variables)**

- 头文件中定义全局变量而不违反 ODR
- `inline constexpr` 是定义头文件常量的推荐方式

:::info 参考链接
[inline specifier](https://en.cppreference.com/w/cpp/language/inline)
:::

### 07. 嵌套命名空间 (`07_nested_namespaces.cpp`)

> **[→ 查看完整代码](./07-nested-namespaces)**

- `namespace A::B::C { }` 替代多层嵌套

:::info 参考链接
[namespace](https://en.cppreference.com/w/cpp/language/namespace)
:::

### 08. \_\_has\_include (`08_has_include.cpp`)

> **[→ 查看完整代码](./08-has-include)**

- 预处理期检查头文件是否存在
- 实现可移植的特性检测

:::info 参考链接
[\_\_has\_include](https://en.cppreference.com/w/cpp/preprocessor/include#__has_include)
:::

---

## 词汇类型（标准库核心）

### 40. std::optional (`40_optional.cpp`)

> **[→ 查看完整代码](./40-optional)**

:::tip 核心特性
`std::optional` 是日常开发中使用频率极高的类型。
:::

- 表示"可能有值，也可能没有"
- `std::nullopt` 表示无值
- `value_or(default)` 提供默认值
- 替代指针/哨兵值/bool+value 模式

:::warning 常见陷阱
不要在没有检查的情况下直接调用 `value()`，当 optional 为空时会抛出 `std::bad_optional_access` 异常。优先使用 `value_or()` 或先检查 `has_value()`。
:::

:::info 参考链接
[std::optional](https://en.cppreference.com/w/cpp/utility/optional)
:::

### 41. std::variant (`41_variant.cpp`)

> **[→ 查看完整代码](./41-variant)**

:::tip 核心特性
`std::variant` 是类型安全的联合体，配合 `std::visit` 可以实现优雅的访问者模式。
:::

- 类型安全的联合体（union）
- `std::visit` 实现访问者模式
- 永远不为空（valueless_by_exception 除外）
- overloaded 模式：用多个 Lambda 处理不同类型

:::info 参考链接
[std::variant](https://en.cppreference.com/w/cpp/utility/variant) | [std::visit](https://en.cppreference.com/w/cpp/utility/variant/visit)
:::

### 42. std::any (`42_any.cpp`)

> **[→ 查看完整代码](./42-any)**

- 存储任意类型的值（类型安全的 `void*`）
- `std::any_cast` 取回值，类型不匹配抛异常
- 适合插件系统、配置存储等场景

:::info 参考链接
[std::any](https://en.cppreference.com/w/cpp/utility/any)
:::

### 43. std::string_view (`43_string_view.cpp`)

> **[→ 查看完整代码](./43-string-view)**

:::tip 核心特性
`string_view` 是避免不必要字符串拷贝的利器。
:::

- 非拥有的字符串引用（指针 + 长度）
- 避免不必要的字符串拷贝

:::warning 生命周期陷阱
**不要持有临时 string 的 string_view！** `string_view` 不拥有底层数据，如果原始字符串被销毁，`string_view` 将变成悬垂引用。
:::

:::info 参考链接
[std::string_view](https://en.cppreference.com/w/cpp/string/basic_string_view)
:::

### 44. std::byte (`44_byte.cpp`)

> **[→ 查看完整代码](./44-byte)**

- 类型安全的字节表示
- 只支持位运算，不支持算术运算
- 明确表达"这是原始字节数据"的语义

:::info 参考链接
[std::byte](https://en.cppreference.com/w/cpp/types/byte)
:::

### 45. invoke 与 apply (`45_invoke_apply.cpp`)

> **[→ 查看完整代码](./45-invoke-apply)**

- `std::invoke` 统一调用任何可调用对象（包括成员函数）
- `std::apply` 将 tuple 展开为函数参数

:::info 参考链接
[std::invoke](https://en.cppreference.com/w/cpp/utility/functional/invoke) | [std::apply](https://en.cppreference.com/w/cpp/utility/apply)
:::

---

## 文件系统与算法

### 50. filesystem (`50_filesystem.cpp`)

> **[→ 查看完整代码](./50-filesystem)**

- 跨平台文件系统操作
- 路径操作、目录遍历、文件状态查询
- 创建/删除/拷贝文件和目录

:::tip 跨平台
`std::filesystem` 统一了 Windows 和 POSIX 文件系统操作的接口，是替代平台特定 API 的最佳选择。
:::

:::info 参考链接
[filesystem](https://en.cppreference.com/w/cpp/filesystem) | [std::filesystem::path](https://en.cppreference.com/w/cpp/filesystem/path)
:::

### 55. 并行算法 (`55_parallel_algorithms.cpp`)

> **[→ 查看完整代码](./55-parallel-algorithms)**

- `std::execution::par` 并行执行策略
- `std::sort(std::execution::par, v.begin(), v.end())`
- 一行代码开启并行化

:::info 参考链接
[execution policy](https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t)
:::

### 56. clamp、gcd、lcm (`56_clamp_gcd_lcm.cpp`)

> **[→ 查看完整代码](./56-clamp-gcd-lcm)**

- `std::clamp(value, lo, hi)` 值钳制
- `std::gcd` 最大公约数
- `std::lcm` 最小公倍数

:::info 参考链接
[std::clamp](https://en.cppreference.com/w/cpp/algorithm/clamp) | [std::gcd](https://en.cppreference.com/w/cpp/numeric/gcd) | [std::lcm](https://en.cppreference.com/w/cpp/numeric/lcm)
:::

---

## 属性

### 90. 属性 (`90_attributes.cpp`)

> **[→ 查看完整代码](./90-attributes)**

- `[[nodiscard]]`：警告忽略返回值
- `[[maybe_unused]]`：消除未使用变量/参数的警告
- `[[fallthrough]]`：标注 switch case 的有意贯穿

:::tip 最佳实践
在返回错误码或资源句柄的函数上使用 `[[nodiscard]]`，可以有效防止忽略错误的 bug。
:::

:::info 参考链接
[[[nodiscard]]](https://en.cppreference.com/w/cpp/language/attributes/nodiscard) | [[[maybe_unused]]](https://en.cppreference.com/w/cpp/language/attributes/maybe_unused) | [[[fallthrough]]](https://en.cppreference.com/w/cpp/language/attributes/fallthrough)
:::
