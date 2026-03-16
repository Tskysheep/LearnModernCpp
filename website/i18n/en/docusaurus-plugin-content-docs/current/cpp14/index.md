---
sidebar_position: 1
title: "C++14 Features Overview"
description: "Comprehensive guide to C++14 features — a minor refinement of C++11, relaxing constexpr restrictions, adding generic lambdas, and useful utilities."
tags: [C++14, modern-cpp, generic-lambda, constexpr, make_unique]
---

# C++14 Features in Detail

> C++14 is a minor refinement and bug-fix release for C++11. It relaxed constexpr restrictions, added generic lambdas, and introduced several practical utilities.

---

## Feature List

### 01. Generic Lambdas (`01_generic_lambdas.cpp`)
- Lambda parameters using `auto` work like template functions
- `[](auto x, auto y) { return x + y; }` can handle any type
- Makes lambdas truly polymorphic callable objects
- Reference: [lambda](https://en.cppreference.com/w/cpp/language/lambda)

### 02. Function Return Type Deduction (`02_return_type_deduction.cpp`)
- Functions can use `auto` as the return type, with the compiler deducing it automatically
- `decltype(auto)` preserves references and cv-qualifiers
- Recursive functions require at least one return statement before the recursive call
- Reference: [auto](https://en.cppreference.com/w/cpp/language/auto) | [decltype(auto)](https://en.cppreference.com/w/cpp/language/auto#Return_type_deduction)

### 03. Variable Templates (`03_variable_templates.cpp`)
- `template<typename T> constexpr T pi = T(3.14159265358979);`
- Type-parameterized constant values
- Simplifies type_traits: `is_same_v<T, U>` replaces `is_same<T, U>::value`
- Reference: [variable template](https://en.cppreference.com/w/cpp/language/variable_template)

### 04. Binary Literals and Digit Separators (`04_binary_literals_separators.cpp`)
- `0b1010` represents a binary number
- `1'000'000` uses single quotes as separators to improve readability
- Separators can be used with any numeric base
- Reference: [integer literal](https://en.cppreference.com/w/cpp/language/integer_literal)

### 05. Relaxed constexpr (`05_relaxed_constexpr.cpp`)
- C++14 allows constexpr functions to contain:
  - Local variable declarations
  - `if`/`switch` conditional statements
  - Loops (`for`, `while`)
  - Multiple statements
- C++11 constexpr functions were limited to a single return statement

:::info C++11 vs C++14 constexpr
C++11 constexpr functions could only contain a single return statement. C++14 significantly relaxed these restrictions, making constexpr much more practical for real-world use.
:::

- Reference: [constexpr](https://en.cppreference.com/w/cpp/language/constexpr)

### 06. std::make_unique (`06_make_unique.cpp`)
- `auto p = std::make_unique<T>(args...)`
- Fills an omission from C++11 (which only had `make_shared`)
- Exception-safe: prevents memory leaks between `new` and `unique_ptr` construction

:::tip Best Practice
Always prefer `std::make_unique` over `new` when creating `unique_ptr` objects.
:::

- Reference: [std::make_unique](https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique)

### 07. [[deprecated]] Attribute (`07_deprecated_attribute.cpp`)
- `[[deprecated("Use new API")]]` marks deprecated interfaces
- The compiler produces a warning but does not prevent compilation
- Applicable to functions, classes, variables, enumerators, etc.
- Reference: [[\[deprecated\]]](https://en.cppreference.com/w/cpp/language/attributes/deprecated)

### 08. Shared (Reader-Writer) Locks (`08_shared_timed_mutex.cpp`)
- `std::shared_timed_mutex` + `std::shared_lock`
- Multiple readers can hold shared locks simultaneously
- Writers require exclusive locks
- Ideal for read-heavy, write-infrequent scenarios
- Reference: [std::shared_timed_mutex](https://en.cppreference.com/w/cpp/thread/shared_timed_mutex) | [std::shared_lock](https://en.cppreference.com/w/cpp/thread/shared_lock)

### 09. integer_sequence (`09_integer_sequence.cpp`)
- `std::index_sequence<0, 1, 2, ...>` is a compile-time integer sequence
- `std::make_index_sequence<N>` generates a sequence from 0 to N-1
- Used in metaprogramming scenarios such as expanding tuple elements
- Reference: [std::integer_sequence](https://en.cppreference.com/w/cpp/utility/integer_sequence)

### 10. std::exchange (`10_exchange.cpp`)
- `auto old = std::exchange(obj, new_value);` replaces and returns the old value
- Simplifies move constructors and move assignment operators
- Atomic operation: saves the old value first, then sets the new value
- Reference: [std::exchange](https://en.cppreference.com/w/cpp/utility/exchange)

### 11. std::quoted (`11_quoted.cpp`)
- `std::quoted(str)` automatically adds quotes and escapes
- Supports round-tripping with I/O streams (output then input without losing information)
- Customizable quote character and escape character
- Reference: [std::quoted](https://en.cppreference.com/w/cpp/io/manip/quoted)
