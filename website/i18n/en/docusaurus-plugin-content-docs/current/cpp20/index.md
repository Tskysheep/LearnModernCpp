---
sidebar_position: 1
title: "C++20 Features Overview"
description: "Comprehensive guide to C++20 features — the largest standard update since C++11, introducing Concepts, Ranges, Coroutines, Modules, and extensive standard library improvements."
tags: [C++20, modern-cpp, concepts, ranges, coroutines, format, span, jthread]
---

# C++20 Features in Detail

> C++20 is the largest standard update since C++11, introducing four major core features — Concepts, Ranges, Coroutines, and Modules — along with extensive standard library improvements.

---

## Core Language Features

### 01. Concepts Basics (`01_concepts_basics.cpp`)

:::warning Core Feature
Concepts fundamentally improve template error messages and expressiveness.
:::

- `concept` defines constraints on template parameters
- `requires` clauses replace SFINAE with much clearer error messages
- The standard library provides `std::integral`, `std::floating_point`, etc.
- Reference: [concepts](https://en.cppreference.com/w/cpp/language/constraints) | [standard concepts](https://en.cppreference.com/w/cpp/concepts)

### 02. Concepts Advanced (`02_concepts_advanced.cpp`)
- Custom concept composition (`&&`, `||`)
- Concept refinement
- Subsumption rules determine overload priority
- Reference: [requires clause](https://en.cppreference.com/w/cpp/language/constraints#Requires_clauses)

### 03. requires Expressions (`03_requires_expressions.cpp`)
- Simple requirements: `requires { expr; }`
- Type requirements: `requires { typename T::type; }`
- Compound requirements: `requires { { expr } -> concept; }`
- Nested requirements: `requires { requires concept<T>; }`
- Reference: [requires expression](https://en.cppreference.com/w/cpp/language/requires)

### 04. Abbreviated Function Templates (`04_abbreviated_templates.cpp`)
- `void f(auto x)` is equivalent to `template<typename T> void f(T x)`
- `void f(std::integral auto x)` adds a constraint in the abbreviated form
- Makes simple template declarations more concise
- Reference: [abbreviated function template](https://en.cppreference.com/w/cpp/language/function_template#Abbreviated_function_template)

### 05. Three-way Comparison — Spaceship Operator `<=>` (`05_spaceship_operator.cpp`)

:::warning Core Feature
The spaceship operator can generate all six comparison operators automatically.
:::

- `auto operator<=>(const T&) const = default;` generates all 6 comparison operators
- Three ordering categories: `strong_ordering`, `weak_ordering`, `partial_ordering`
- Dramatically reduces boilerplate code for comparison operators
- Reference: [three-way comparison](https://en.cppreference.com/w/cpp/language/operator_comparison#Three-way_comparison) | [std::strong_ordering](https://en.cppreference.com/w/cpp/utility/compare/strong_ordering)

### 06. Designated Initializers (`06_designated_initializers.cpp`)
- `Point p{.x = 1, .y = 2};` named member initialization
- Must follow the member declaration order
- Self-documenting initialization style
- Reference: [aggregate initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization#Designated_initializers)

### 07. consteval — Immediate Functions (`07_consteval.cpp`)
- `consteval` functions are **guaranteed** to execute at compile time
- Difference from `constexpr`: constexpr means "can be," consteval means "must be"
- Used for compile-time validation and code generation
- Reference: [consteval](https://en.cppreference.com/w/cpp/language/consteval)

### 08. constinit — Compile-time Initialization (`08_constinit.cpp`)
- Guarantees that a variable is initialized at compile time
- Prevents the Static Initialization Order Fiasco
- Only constrains initialization; the variable itself can still be modified at runtime
- Reference: [constinit](https://en.cppreference.com/w/cpp/language/constinit)

### 09. Template Lambdas (`09_template_lambdas.cpp`)
- `[]<typename T>(T x) { ... }` uses explicit template parameters in a lambda
- More precise type control than `auto` parameters
- Supports concept constraints
- Reference: [lambda](https://en.cppreference.com/w/cpp/language/lambda)

### 10. Parenthesized Aggregate Initialization (`10_aggregate_parens_init.cpp`)
- Aggregate types can now be initialized with parentheses
- Enables `make_shared<Aggregate>(args...)` and `emplace` to work with aggregates
- Reference: [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization)

---

## Ranges

### 20. Ranges Basics (`20_ranges_basics.cpp`)

:::warning Core Feature
Ranges modernize the way we write algorithms in C++.
:::

- `std::ranges::sort(v)` replaces `std::sort(v.begin(), v.end())`
- Projections: `ranges::sort(v, {}, &Person::name)`
- Safer: automatic range validity checks
- Reference: [ranges](https://en.cppreference.com/w/cpp/ranges) | [ranges::sort](https://en.cppreference.com/w/cpp/algorithm/ranges/sort)

### 21. Ranges Views — Lazy Data Transformation (`21_ranges_views.cpp`)

:::warning Core Feature
Views enable composable, lazy data transformation pipelines.
:::

- Lazily evaluated data transformation pipelines
- Pipe syntax: `v | filter(pred) | transform(fn) | take(5)`
- Common views: `iota`, `filter`, `transform`, `take`, `drop`, `reverse`
- Reference: [views](https://en.cppreference.com/w/cpp/ranges#Range_adaptors) | [views::filter](https://en.cppreference.com/w/cpp/ranges/filter_view) | [views::transform](https://en.cppreference.com/w/cpp/ranges/transform_view)

### 22. Custom Range (`22_ranges_custom.cpp`)
- Implement custom types that satisfy the `range` concept
- Custom view adaptors
- Reference: [range concept](https://en.cppreference.com/w/cpp/ranges/range) | [view_interface](https://en.cppreference.com/w/cpp/ranges/view_interface)

---

## Coroutines

### 30. Coroutines Basics (`30_coroutines_basics.cpp`)
- `co_await`: Suspend and wait for an asynchronous operation
- `co_yield`: Produce a value and suspend
- `co_return`: Finish the coroutine and return a value
- Requires a custom `promise_type` and coroutine handle
- Reference: [coroutines](https://en.cppreference.com/w/cpp/language/coroutines) | [coroutine_handle](https://en.cppreference.com/w/cpp/coroutine/coroutine_handle)

### 31. Coroutine Generator (`31_coroutines_generator.cpp`)
- Implement a lazy sequence generator
- `co_yield` produces values one at a time
- Infinite sequences, on-demand computation
- Reference: [co_yield](https://en.cppreference.com/w/cpp/language/coroutines#co_yield)

---

## Standard Library

### 40. std::span — Non-owning Contiguous View (`40_span.cpp`)
- A non-owning view over a contiguous memory region
- Replaces `(T* ptr, size_t size)` parameter pairs
- Static extent and dynamic extent
- Reference: [std::span](https://en.cppreference.com/w/cpp/container/span)

### 41. std::format — Type-safe String Formatting (`41_format.cpp`)
- Type-safe string formatting
- `std::format("Hello, {}!", name)` replaces printf
- Supports positional arguments, alignment, padding, and precision
- Reference: [std::format](https://en.cppreference.com/w/cpp/utility/format/format)

### 42. Calendar and Time Zones (`42_calendar_timezone.cpp`)
- `std::chrono::year_month_day` date type
- Date arithmetic: add/subtract days, months, years
- Time zone conversion (compiler support varies)
- Reference: [chrono calendar](https://en.cppreference.com/w/cpp/chrono#Calendar) | [year_month_day](https://en.cppreference.com/w/cpp/chrono/year_month_day)

### 43. source_location — Compile-time Call Site Info (`43_source_location.cpp`)
- `std::source_location::current()` obtains the call site location
- Replaces `__FILE__` and `__LINE__` macros
- Used in logging and assertions
- Reference: [std::source_location](https://en.cppreference.com/w/cpp/utility/source_location)

### 44. bit_cast — Safe Bit-pattern Conversion (`44_bit_cast.cpp`)
- `std::bit_cast<To>(from)` performs a safe bit-pattern conversion
- constexpr-friendly
- Replaces `reinterpret_cast` and `memcpy` patterns
- Reference: [std::bit_cast](https://en.cppreference.com/w/cpp/numeric/bit_cast)

### 45. Mathematical Constants (`45_math_constants.cpp`)
- `std::numbers::pi`, `std::numbers::e`, etc.
- Templated: `std::numbers::pi_v<float>`
- Replaces custom `#define PI` macros
- Reference: [std::numbers](https://en.cppreference.com/w/cpp/numeric/constants)

### 46. starts_with / ends_with String Operations (`46_string_operations.cpp`)
- `str.starts_with("prefix")` and `str.ends_with("suffix")`
- Clear and intuitive, replacing manual substring comparisons
- Reference: [starts_with](https://en.cppreference.com/w/cpp/string/basic_string/starts_with) | [ends_with](https://en.cppreference.com/w/cpp/string/basic_string/ends_with)

### 47. contains() — Associative Container Lookup (`47_contains.cpp`)
- `map.contains(key)` replaces `map.find(key) != map.end()`
- Works with all associative and unordered containers
- Reference: [map::contains](https://en.cppreference.com/w/cpp/container/map/contains) | [set::contains](https://en.cppreference.com/w/cpp/container/set/contains)

---

## Concurrency

### 70. jthread — Auto-joining Thread (`70_jthread.cpp`)
- Automatically calls `join()` on destruction (RAII)
- Built-in `stop_token` cancellation mechanism
- `stop_callback` registers cancellation callbacks
- Reference: [std::jthread](https://en.cppreference.com/w/cpp/thread/jthread) | [std::stop_token](https://en.cppreference.com/w/cpp/thread/stop_token)

### 71. barrier, latch, and semaphore (`71_barrier_latch.cpp`)
- `std::latch`: A single-use synchronization barrier
- `std::barrier`: A reusable synchronization barrier
- `std::counting_semaphore`: A counting semaphore
- Reference: [std::latch](https://en.cppreference.com/w/cpp/thread/latch) | [std::barrier](https://en.cppreference.com/w/cpp/thread/barrier) | [std::counting_semaphore](https://en.cppreference.com/w/cpp/thread/counting_semaphore)

---

## Attributes

### 90. Attributes (`90_attributes.cpp`)
- `[[likely]]`/`[[unlikely]]`: Branch prediction hints
- `[[no_unique_address]]`: Empty members do not take up extra space (EBO for members)
- Reference: [[\[likely\]]](https://en.cppreference.com/w/cpp/language/attributes/likely) | [[\[no_unique_address\]]](https://en.cppreference.com/w/cpp/language/attributes/no_unique_address)
