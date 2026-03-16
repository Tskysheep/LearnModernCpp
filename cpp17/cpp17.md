# C++17 特性详解

> C++17 引入了结构化绑定、if constexpr、optional/variant/any 词汇类型、string_view 和 filesystem 等实用特性。

---

## 语言核心特性

### 01. 结构化绑定 ⭐ (`01_structured_bindings.cpp`)
- `auto [x, y] = std::make_pair(1, 2);`
- 适用于 pair、tuple、struct、数组
- 消除 `.first`/`.second` 样板代码
- 参考: [structured bindings](https://en.cppreference.com/w/cpp/language/structured_binding)

### 02. if/switch 初始化语句 (`02_if_switch_init.cpp`)
- `if (auto it = map.find(key); it != map.end()) { ... }`
- 变量作用域限制在 if/switch 块内
- 代码更紧凑，减少变量泄漏
- 参考: [if statement](https://en.cppreference.com/w/cpp/language/if) | [switch statement](https://en.cppreference.com/w/cpp/language/switch)

### 03. 类模板参数推导 CTAD (`03_ctad.cpp`)
- `std::pair p{1, 3.14};` 无需写 `std::pair<int, double>`
- `std::vector v{1, 2, 3};` 自动推导为 `vector<int>`
- 可自定义推导指引（deduction guide）
- 参考: [class template argument deduction](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction)

### 04. 折叠表达式 (`04_fold_expressions.cpp`)
- `(args + ...)` 一元右折叠
- `(... + args)` 一元左折叠
- 一行代码替代递归的可变参数模板展开
- 参考: [fold expression](https://en.cppreference.com/w/cpp/language/fold)

### 05. if constexpr ⭐ (`05_constexpr_if.cpp`)
- 编译期条件分支，未选中的分支不会被实例化
- 替代 SFINAE 和标签分发
- 模板编程的重大简化
- 参考: [if constexpr](https://en.cppreference.com/w/cpp/language/if#Constexpr_if)

### 06. inline 变量 (`06_inline_variables.cpp`)
- 头文件中定义全局变量而不违反 ODR
- `inline constexpr` 是定义头文件常量的推荐方式
- 参考: [inline specifier](https://en.cppreference.com/w/cpp/language/inline)

### 07. 嵌套命名空间 (`07_nested_namespaces.cpp`)
- `namespace A::B::C { }` 替代多层嵌套
- 参考: [namespace](https://en.cppreference.com/w/cpp/language/namespace)

### 08. \_\_has\_include (`08_has_include.cpp`)
- 预处理期检查头文件是否存在
- 实现可移植的特性检测
- 参考: [__has_include](https://en.cppreference.com/w/cpp/preprocessor/include#__has_include)

---

## 词汇类型（标准库核心）

### 40. std::optional ⭐ (`40_optional.cpp`)
- 表示"可能有值，也可能没有"
- `std::nullopt` 表示无值
- `value_or(default)` 提供默认值
- 替代指针/哨兵值/bool+value 模式
- 参考: [std::optional](https://en.cppreference.com/w/cpp/utility/optional)

### 41. std::variant ⭐ (`41_variant.cpp`)
- 类型安全的联合体（union）
- `std::visit` 实现访问者模式
- 永远不为空（valueless_by_exception 除外）
- overloaded 模式：用多个 Lambda 处理不同类型
- 参考: [std::variant](https://en.cppreference.com/w/cpp/utility/variant) | [std::visit](https://en.cppreference.com/w/cpp/utility/variant/visit)

### 42. std::any (`42_any.cpp`)
- 存储任意类型的值（类型安全的 `void*`）
- `std::any_cast` 取回值，类型不匹配抛异常
- 适合插件系统、配置存储等场景
- 参考: [std::any](https://en.cppreference.com/w/cpp/utility/any)

### 43. std::string_view ⭐ (`43_string_view.cpp`)
- 非拥有的字符串引用（指针 + 长度）
- 避免不必要的字符串拷贝
- **陷阱**：不要持有临时 string 的 string_view
- 参考: [std::string_view](https://en.cppreference.com/w/cpp/string/basic_string_view)

### 44. std::byte (`44_byte.cpp`)
- 类型安全的字节表示
- 只支持位运算，不支持算术运算
- 明确表达"这是原始字节数据"的语义
- 参考: [std::byte](https://en.cppreference.com/w/cpp/types/byte)

### 45. invoke 与 apply (`45_invoke_apply.cpp`)
- `std::invoke` 统一调用任何可调用对象（包括成员函数）
- `std::apply` 将 tuple 展开为函数参数
- 参考: [std::invoke](https://en.cppreference.com/w/cpp/utility/functional/invoke) | [std::apply](https://en.cppreference.com/w/cpp/utility/apply)

---

## 文件系统与算法

### 50. filesystem (`50_filesystem.cpp`)
- 跨平台文件系统操作
- 路径操作、目录遍历、文件状态查询
- 创建/删除/拷贝文件和目录
- 参考: [filesystem](https://en.cppreference.com/w/cpp/filesystem) | [std::filesystem::path](https://en.cppreference.com/w/cpp/filesystem/path)

### 55. 并行算法 (`55_parallel_algorithms.cpp`)
- `std::execution::par` 并行执行策略
- `std::sort(std::execution::par, v.begin(), v.end())`
- 一行代码开启并行化
- 参考: [execution policy](https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t)

### 56. clamp、gcd、lcm (`56_clamp_gcd_lcm.cpp`)
- `std::clamp(value, lo, hi)` 值钳制
- `std::gcd` 最大公约数
- `std::lcm` 最小公倍数
- 参考: [std::clamp](https://en.cppreference.com/w/cpp/algorithm/clamp) | [std::gcd](https://en.cppreference.com/w/cpp/numeric/gcd) | [std::lcm](https://en.cppreference.com/w/cpp/numeric/lcm)

---

## 属性

### 90. 属性 (`90_attributes.cpp`)
- `[[nodiscard]]`：警告忽略返回值
- `[[maybe_unused]]`：消除未使用变量/参数的警告
- `[[fallthrough]]`：标注 switch case 的有意贯穿
- 参考: [[[nodiscard]]](https://en.cppreference.com/w/cpp/language/attributes/nodiscard) | [[[maybe_unused]]](https://en.cppreference.com/w/cpp/language/attributes/maybe_unused) | [[[fallthrough]]](https://en.cppreference.com/w/cpp/language/attributes/fallthrough)
