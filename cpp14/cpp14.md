# C++14 特性详解

> C++14 是对 C++11 的小幅改进和修补，放松了 constexpr 限制，增加了泛型 Lambda 和一些实用工具。

---

## 特性列表

### 01. 泛型 Lambda (`01_generic_lambdas.cpp`)
- Lambda 参数使用 `auto`，相当于模板函数
- `[](auto x, auto y) { return x + y; }` 可处理任意类型
- 使 Lambda 成为真正的多态可调用对象
- 参考: [lambda](https://en.cppreference.com/w/cpp/language/lambda)

### 02. 函数返回类型推导 (`02_return_type_deduction.cpp`)
- 函数可以用 `auto` 作为返回类型，编译器自动推导
- `decltype(auto)` 保留引用和 cv 限定符
- 递归函数需要至少一个 return 在递归调用前
- 参考: [auto](https://en.cppreference.com/w/cpp/language/auto) | [decltype(auto)](https://en.cppreference.com/w/cpp/language/auto#Return_type_deduction)

### 03. 变量模板 (`03_variable_templates.cpp`)
- `template<typename T> constexpr T pi = T(3.14159265358979);`
- 类型参数化的常量值
- 简化 type_traits：`is_same_v<T, U>` 替代 `is_same<T, U>::value`
- 参考: [variable template](https://en.cppreference.com/w/cpp/language/variable_template)

### 04. 二进制字面量与数字分隔符 (`04_binary_literals_separators.cpp`)
- `0b1010` 表示二进制数
- `1'000'000` 用单引号分隔提升可读性
- 分隔符可用于任何进制
- 参考: [integer literal](https://en.cppreference.com/w/cpp/language/integer_literal)

### 05. 放松的 constexpr (`05_relaxed_constexpr.cpp`)
- C++14 允许 constexpr 函数包含：
  - 局部变量声明
  - `if`/`switch` 条件语句
  - 循环（`for`、`while`）
  - 多条语句
- C++11 的 constexpr 函数只能有一条 return 语句
- 参考: [constexpr](https://en.cppreference.com/w/cpp/language/constexpr)

### 06. std::make_unique (`06_make_unique.cpp`)
- `auto p = std::make_unique<T>(args...)`
- 填补 C++11 的遗漏（当时只有 `make_shared`）
- 异常安全：避免 `new` 和 `unique_ptr` 构造之间的内存泄漏
- 参考: [std::make_unique](https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique)

### 07. [[deprecated]] (`07_deprecated_attribute.cpp`)
- `[[deprecated("使用新API")]]` 标记弃用的接口
- 编译器产生警告但不阻止编译
- 适用于函数、类、变量、枚举等
- 参考: [[[deprecated]]](https://en.cppreference.com/w/cpp/language/attributes/deprecated)

### 08. 读写锁 (`08_shared_timed_mutex.cpp`)
- `std::shared_timed_mutex` + `std::shared_lock`
- 多个读者可以同时持有共享锁
- 写者需要独占锁
- 适合读多写少的场景
- 参考: [std::shared_timed_mutex](https://en.cppreference.com/w/cpp/thread/shared_timed_mutex) | [std::shared_lock](https://en.cppreference.com/w/cpp/thread/shared_lock)

### 09. integer_sequence (`09_integer_sequence.cpp`)
- `std::index_sequence<0, 1, 2, ...>` 编译期整数序列
- `std::make_index_sequence<N>` 生成 0 到 N-1 的序列
- 用于 tuple 元素展开等元编程场景
- 参考: [std::integer_sequence](https://en.cppreference.com/w/cpp/utility/integer_sequence)

### 10. std::exchange (`10_exchange.cpp`)
- `auto old = std::exchange(obj, new_value);` 替换并返回旧值
- 简化移动构造函数和移动赋值运算符
- 原子操作：先保存旧值，再设置新值
- 参考: [std::exchange](https://en.cppreference.com/w/cpp/utility/exchange)

### 11. std::quoted (`11_quoted.cpp`)
- `std::quoted(str)` 自动添加引号和转义
- 支持 I/O 流的往返（输出再输入不丢失信息）
- 可自定义引号字符和转义字符
- 参考: [std::quoted](https://en.cppreference.com/w/cpp/io/manip/quoted)
