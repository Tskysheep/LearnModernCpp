---
sidebar_position: 1
title: "C++17 Features Overview"
description: "Comprehensive guide to C++17 features — structured bindings, if constexpr, vocabulary types (optional/variant/any), string_view, filesystem, and more."
tags: [C++17, modern-cpp, structured-bindings, optional, variant, string_view, filesystem]
---

# C++17 Features in Detail

> C++17 introduced structured bindings, if constexpr, vocabulary types (optional/variant/any), string_view, filesystem, and other practical features.

---

## Core Language Features

### 01. Structured Bindings (`01_structured_bindings.cpp`)

:::warning Core Feature
Structured bindings are one of the most frequently used C++17 features.
:::

- `auto [x, y] = std::make_pair(1, 2);`
- Works with pair, tuple, struct, and arrays
- Eliminates `.first`/`.second` boilerplate code
- Reference: [structured bindings](https://en.cppreference.com/w/cpp/language/structured_binding)

### 02. if/switch with Initializer Statements (`02_if_switch_init.cpp`)
- `if (auto it = map.find(key); it != map.end()) { ... }`
- Variable scope is limited to the if/switch block
- More compact code, reduces variable leakage
- Reference: [if statement](https://en.cppreference.com/w/cpp/language/if) | [switch statement](https://en.cppreference.com/w/cpp/language/switch)

### 03. Class Template Argument Deduction — CTAD (`03_ctad.cpp`)
- `std::pair p{1, 3.14};` without writing `std::pair<int, double>`
- `std::vector v{1, 2, 3};` automatically deduced as `vector<int>`
- Custom deduction guides can be defined
- Reference: [class template argument deduction](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction)

### 04. Fold Expressions (`04_fold_expressions.cpp`)
- `(args + ...)` unary right fold
- `(... + args)` unary left fold
- Replaces recursive variadic template expansion with a single line of code
- Reference: [fold expression](https://en.cppreference.com/w/cpp/language/fold)

### 05. if constexpr — Compile-time Conditional Branching (`05_constexpr_if.cpp`)

:::warning Core Feature
if constexpr is a major simplification for template programming.
:::

- Compile-time conditional branching; the unselected branch is not instantiated
- Replaces SFINAE and tag dispatch patterns
- A significant simplification for template programming
- Reference: [if constexpr](https://en.cppreference.com/w/cpp/language/if#Constexpr_if)

### 06. inline Variables (`06_inline_variables.cpp`)
- Define global variables in header files without violating the ODR
- `inline constexpr` is the recommended way to define constants in headers
- Reference: [inline specifier](https://en.cppreference.com/w/cpp/language/inline)

### 07. Nested Namespaces (`07_nested_namespaces.cpp`)
- `namespace A::B::C { }` replaces multi-level nesting
- Reference: [namespace](https://en.cppreference.com/w/cpp/language/namespace)

### 08. \_\_has\_include Preprocessor Check (`08_has_include.cpp`)
- Preprocessor-level check for header file existence
- Enables portable feature detection
- Reference: [__has_include](https://en.cppreference.com/w/cpp/preprocessor/include#__has_include)

---

## Vocabulary Types (Core Standard Library)

### 40. std::optional — Nullable Value Wrapper (`40_optional.cpp`)

:::warning Core Feature
std::optional is a fundamental vocabulary type for expressing "may or may not have a value."
:::

- Represents "might have a value, or might not"
- `std::nullopt` represents the empty state
- `value_or(default)` provides a fallback default value
- Replaces pointer / sentinel value / bool+value patterns
- Reference: [std::optional](https://en.cppreference.com/w/cpp/utility/optional)

### 41. std::variant — Type-safe Union (`41_variant.cpp`)

:::warning Core Feature
std::variant provides a type-safe alternative to C unions.
:::

- A type-safe union
- `std::visit` implements the visitor pattern
- Never empty (except for the `valueless_by_exception` state)
- Overloaded pattern: use multiple lambdas to handle different types
- Reference: [std::variant](https://en.cppreference.com/w/cpp/utility/variant) | [std::visit](https://en.cppreference.com/w/cpp/utility/variant/visit)

### 42. std::any — Type-erased Value Container (`42_any.cpp`)
- Stores a value of any type (a type-safe `void*`)
- `std::any_cast` retrieves the value; throws an exception on type mismatch
- Suitable for plugin systems, configuration storage, etc.
- Reference: [std::any](https://en.cppreference.com/w/cpp/utility/any)

### 43. std::string_view — Non-owning String Reference (`43_string_view.cpp`)

:::warning Core Feature
string_view avoids unnecessary string copies and is essential for performance-critical code.
:::

- A non-owning string reference (pointer + length)
- Avoids unnecessary string copies

:::danger Pitfall
Do not hold a `string_view` to a temporary `std::string`. The string_view will dangle once the temporary is destroyed.
:::

- Reference: [std::string_view](https://en.cppreference.com/w/cpp/string/basic_string_view)

### 44. std::byte — Type-safe Byte Representation (`44_byte.cpp`)
- A type-safe byte representation
- Only supports bitwise operations, not arithmetic operations
- Clearly expresses the semantics of "this is raw byte data"
- Reference: [std::byte](https://en.cppreference.com/w/cpp/types/byte)

### 45. std::invoke and std::apply (`45_invoke_apply.cpp`)
- `std::invoke` uniformly calls any callable object (including member functions)
- `std::apply` expands a tuple into function arguments
- Reference: [std::invoke](https://en.cppreference.com/w/cpp/utility/functional/invoke) | [std::apply](https://en.cppreference.com/w/cpp/utility/apply)

---

## Filesystem and Algorithms

### 50. filesystem — Cross-platform File Operations (`50_filesystem.cpp`)
- Cross-platform filesystem operations
- Path manipulation, directory traversal, file status queries
- Create / delete / copy files and directories
- Reference: [filesystem](https://en.cppreference.com/w/cpp/filesystem) | [std::filesystem::path](https://en.cppreference.com/w/cpp/filesystem/path)

### 55. Parallel Algorithms (`55_parallel_algorithms.cpp`)
- `std::execution::par` parallel execution policy
- `std::sort(std::execution::par, v.begin(), v.end())`
- Enable parallelism with a single line of code
- Reference: [execution policy](https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t)

### 56. clamp, gcd, and lcm (`56_clamp_gcd_lcm.cpp`)
- `std::clamp(value, lo, hi)` clamps a value to a range
- `std::gcd` computes the greatest common divisor
- `std::lcm` computes the least common multiple
- Reference: [std::clamp](https://en.cppreference.com/w/cpp/algorithm/clamp) | [std::gcd](https://en.cppreference.com/w/cpp/numeric/gcd) | [std::lcm](https://en.cppreference.com/w/cpp/numeric/lcm)

---

## Attributes

### 90. Attributes (`90_attributes.cpp`)
- `[[nodiscard]]`: Warns when a return value is discarded
- `[[maybe_unused]]`: Suppresses unused variable/parameter warnings
- `[[fallthrough]]`: Marks intentional fallthrough in switch cases
- Reference: [[\[nodiscard\]]](https://en.cppreference.com/w/cpp/language/attributes/nodiscard) | [[\[maybe_unused\]]](https://en.cppreference.com/w/cpp/language/attributes/maybe_unused) | [[\[fallthrough\]]](https://en.cppreference.com/w/cpp/language/attributes/fallthrough)
