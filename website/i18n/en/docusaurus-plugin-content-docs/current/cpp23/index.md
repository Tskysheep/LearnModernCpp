---
sidebar_position: 1
title: "C++23 Features Overview"
description: "Comprehensive guide to C++23 features — std::expected, deducing this, std::print, std::mdspan, optional monadic operations, and more. Some features have limited compiler support."
tags: [C++23, modern-cpp, expected, deducing-this, print, mdspan, generator]
---

# C++23 Features in Detail

> C++23 continues to refine modern C++, introducing std::expected, deducing this, std::print, std::mdspan, and other important features. Some features still have limited compiler support.

---

## Compilable Examples

### 01. if consteval — Compile-time Evaluation Detection (`01_if_consteval.cpp`)
- Detects whether the current evaluation is happening at compile time inside a `constexpr` function
- `if consteval { /* compile-time path */ } else { /* runtime path */ }`
- Replaces `std::is_constant_evaluated()` (C++20) with cleaner syntax
- Reference: [if consteval](https://en.cppreference.com/w/cpp/language/if#Consteval_if)

### 02. Multidimensional Subscript Operator (`02_multidim_subscript.cpp`)
- `operator[](size_t i, size_t j)` supports multiple parameters
- `matrix[i, j]` replaces `matrix(i, j)` or `matrix[i][j]`
- More intuitive multidimensional array access
- Reference: [operator[]](https://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_subscript_operator)

### 03. size_t Literal Suffix (`03_size_t_literal.cpp`)
- `42uz` or `42zu` suffix produces a `size_t` type
- Eliminates signed/unsigned comparison warnings
- `for (auto i = 0uz; i < v.size(); ++i)`
- Reference: [integer literal](https://en.cppreference.com/w/cpp/language/integer_literal)

### 05. Lambda Improvements (`05_lambda_improvements.cpp`)
- Empty parameter list `()` can be omitted when attributes are present
- `static` lambda: Stateless lambdas can be marked as static
- Reference: [lambda](https://en.cppreference.com/w/cpp/language/lambda)

### 40. std::expected — Value-or-Error Type (`40_expected.cpp`)

:::warning Core Feature
std::expected provides a modern, type-safe alternative to exceptions for error handling.
:::

- `std::expected<T, E>` represents "a value or an error"
- An alternative to exceptions for error handling
- Supports monadic operations: `and_then`, `transform`, `or_else`
- More type-safe than error codes, more efficient than exceptions
- Reference: [std::expected](https://en.cppreference.com/w/cpp/utility/expected)

### 41. optional Monadic Operations (`41_optional_monadic.cpp`)
- `optional::and_then(f)`: If has value, calls f (f returns optional)
- `optional::transform(f)`: If has value, calls f (f returns a value)
- `optional::or_else(f)`: If empty, calls f
- Functional-style chained operations

:::tip Functional Programming Style
Monadic operations allow you to chain optional operations without nested if-checks, resulting in cleaner and more composable code.
:::

- Reference: [optional::and_then](https://en.cppreference.com/w/cpp/utility/optional/and_then) | [optional::transform](https://en.cppreference.com/w/cpp/utility/optional/transform) | [optional::or_else](https://en.cppreference.com/w/cpp/utility/optional/or_else)

### 42. move_only_function (`42_move_only_function.cpp`)
- `std::move_only_function` can store move-only callable objects
- `std::function` requires callable objects to be copyable
- Suitable for storing lambdas that capture a `unique_ptr`
- Reference: [std::move_only_function](https://en.cppreference.com/w/cpp/utility/functional/move_only_function)

### 43. to_underlying — Enum to Underlying Type (`43_to_underlying.cpp`)
- `std::to_underlying(enum_value)` converts an enum to its underlying type
- Replaces `static_cast<std::underlying_type_t<E>>(e)`
- More concise and safer
- Reference: [std::to_underlying](https://en.cppreference.com/w/cpp/utility/to_underlying)

### 44. std::unreachable — Unreachable Code Marker (`44_unreachable.cpp`)
- Marks an unreachable code path
- Allows the compiler to perform more aggressive optimizations
- Undefined behavior if actually reached (indicates a bug)
- Reference: [std::unreachable](https://en.cppreference.com/w/cpp/utility/unreachable)

### 61. stacktrace — Runtime Call Stack (`61_stacktrace.cpp`)
- Obtain call stack information at runtime
- Used for error diagnostics and debugging
- Compiler support varies
- Reference: [std::stacktrace](https://en.cppreference.com/w/cpp/utility/basic_stacktrace)

### 70. byteswap — Byte Order Reversal (`70_byteswap.cpp`)
- `std::byteswap(value)` reverses byte order
- Used in network programming (endianness conversion)
- constexpr-friendly
- Reference: [std::byteswap](https://en.cppreference.com/w/cpp/numeric/byteswap)

### 90. [[assume]] Attribute (`90_attributes.cpp`)
- `[[assume(expr)]]` tells the compiler that a condition is always true
- The compiler can use this for optimization
- Undefined behavior if the assumption does not hold
- Reference: [[\[assume\]]](https://en.cppreference.com/w/cpp/language/attributes/assume)

---

## Documentation-only Features

The following features have incomplete support in current mainstream compilers (GCC 14, Clang 18, MSVC 2022) and are described here for documentation purposes only.

### Deducing this (Explicit Object Parameter)

Eliminates code duplication between const and non-const member function overloads.

Reference: [deducing this](https://en.cppreference.com/w/cpp/language/member_functions#Explicit_object_parameter)

```cpp
// Before C++23, you needed two overloads:
struct Widget {
    int& get() { return data; }
    const int& get() const { return data; }
    int data;
};

// C++23 with deducing this — a single version handles both:
struct Widget {
    // this auto&& self is the explicit object parameter
    auto&& get(this auto&& self) {
        return std::forward_like<decltype(self)>(self.data);
    }
    int data;
};
```

**Key points:**
- `this auto&& self` as the first parameter
- The compiler automatically deduces the type of self based on the caller's const-qualification
- Can also be used to simplify the CRTP pattern

### std::print / std::println

A modern output method that replaces `std::cout`.

Reference: [std::print](https://en.cppreference.com/w/cpp/io/print) | [std::println](https://en.cppreference.com/w/cpp/io/println)

```cpp
#include <print>

int main() {
    std::println("Hello, {}!", "World");      // Automatic newline
    std::print("pi = {:.4f}\n", 3.14159);     // Manual newline
    std::println("{:>10}", 42);                // Right-aligned
}
```

**Key points:**
- Uses `std::format`-based formatting syntax
- `println` automatically appends a newline
- More efficient than `cout` (fewer virtual function calls and locale handling)
- Can output to any output stream

### std::mdspan (Multidimensional Array View)

A non-owning multidimensional array view, replacing manual multidimensional index calculations.

Reference: [std::mdspan](https://en.cppreference.com/w/cpp/container/mdspan)

```cpp
#include <mdspan>

void process_matrix(std::mdspan<float, std::dextents<size_t, 2>> mat) {
    for (size_t i = 0; i < mat.extent(0); ++i)
        for (size_t j = 0; j < mat.extent(1); ++j)
            mat[i, j] *= 2.0f;
}

int main() {
    std::vector<float> data(12);
    // Interpret one-dimensional data as a 3x4 matrix
    auto mat = std::mdspan(data.data(), 3, 4);
    process_matrix(mat);
}
```

**Key points:**
- Supports arbitrary dimensions
- Supports multiple layout strategies (row-major, column-major, custom strides)
- Similar to `std::span` but with multidimensional support
- Powerful tool for scientific computing and image processing

### std::flat_map / std::flat_set

Cache-friendly sorted associative containers.

Reference: [std::flat_map](https://en.cppreference.com/w/cpp/container/flat_map) | [std::flat_set](https://en.cppreference.com/w/cpp/container/flat_set)

```cpp
#include <flat_map>
#include <flat_set>

int main() {
    std::flat_map<std::string, int> scores;
    scores["Alice"] = 95;
    scores["Bob"] = 87;

    std::flat_set<int> s = {3, 1, 4, 1, 5};
    // Internally uses a sorted vector, with faster iteration
}
```

**Key points:**
- Internally uses a sorted contiguous container (typically `vector`)
- Lookup is O(log n) (binary search), but more cache-friendly
- Insertion/deletion is O(n), making them suitable for read-heavy, write-infrequent scenarios
- Lower memory footprint than `std::map` (red-black tree)

### std::generator

A standard lazy generator coroutine type.

Reference: [std::generator](https://en.cppreference.com/w/cpp/coroutine/generator)

```cpp
#include <generator>

std::generator<int> fibonacci() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;
        auto next = a + b;
        a = b;
        b = next;
    }
}

int main() {
    for (auto n : fibonacci()) {
        if (n > 100) break;
        std::println("{}", n);
    }
}
```

**Key points:**
- C++20 coroutines lacked a standard Generator type, requiring hand-written promise_type
- C++23 provides `std::generator<T>` out of the box
- Supports the `ranges` interface
- Can be used for recursive generators (`std::generator<T>::yielded`)

### static operator()

Zero-overhead invocation for stateless function objects.

Reference: [static operator()](https://en.cppreference.com/w/cpp/language/operators#Function_call_operator)

```cpp
struct Hasher {
    static size_t operator()(int x) noexcept {
        return x * 2654435761u;
    }
};

// Lambdas can also be static
auto add = [](int a, int b) static { return a + b; };
```

**Key points:**
- Stateless callable objects do not need a this pointer
- `static operator()` can be called directly without an object instance
- The compiler can optimize more aggressively

### New std::ranges Views

C++23 significantly expanded the ranges views library.

Reference: [ranges](https://en.cppreference.com/w/cpp/ranges) | [views::chunk](https://en.cppreference.com/w/cpp/ranges/chunk_view) | [views::zip](https://en.cppreference.com/w/cpp/ranges/zip_view) | [views::enumerate](https://en.cppreference.com/w/cpp/ranges/enumerate_view)

```cpp
#include <ranges>

// chunk: Split into fixed-size blocks
auto chunks = v | std::views::chunk(3);
// [1,2,3], [4,5,6], [7,8,9]

// slide: Sliding window
auto windows = v | std::views::slide(3);
// [1,2,3], [2,3,4], [3,4,5], ...

// zip: Combine multiple ranges in parallel
auto zipped = std::views::zip(names, scores);

// enumerate: Iterate with index
for (auto [i, val] : v | std::views::enumerate) {
    std::println("[{}] = {}", i, val);
}

// chunk_by: Group by predicate
auto groups = v | std::views::chunk_by(std::less{});

// join_with: Join with a separator
auto joined = strs | std::views::join_with(',');

// cartesian_product: Cartesian product
for (auto [x, y] : std::views::cartesian_product(xs, ys)) {
    // ...
}
```

**New Views Reference Table:**

| View | Function | Reference |
|------|----------|-----------|
| `chunk(n)` | Split into fixed-size blocks of n | [chunk_view](https://en.cppreference.com/w/cpp/ranges/chunk_view) |
| `slide(n)` | Sliding window of size n | [slide_view](https://en.cppreference.com/w/cpp/ranges/slide_view) |
| `zip` | Combine multiple ranges in parallel | [zip_view](https://en.cppreference.com/w/cpp/ranges/zip_view) |
| `zip_transform` | Combine and transform | [zip_transform_view](https://en.cppreference.com/w/cpp/ranges/zip_transform_view) |
| `enumerate` | Iterate with index | [enumerate_view](https://en.cppreference.com/w/cpp/ranges/enumerate_view) |
| `chunk_by(pred)` | Group by predicate | [chunk_by_view](https://en.cppreference.com/w/cpp/ranges/chunk_by_view) |
| `join_with(sep)` | Join with separator | [join_with_view](https://en.cppreference.com/w/cpp/ranges/join_with_view) |
| `stride(n)` | Take every nth element | [stride_view](https://en.cppreference.com/w/cpp/ranges/stride_view) |
| `cartesian_product` | Cartesian product | [cartesian_product_view](https://en.cppreference.com/w/cpp/ranges/cartesian_product_view) |
| `as_rvalue` | Convert to rvalue view | [as_rvalue_view](https://en.cppreference.com/w/cpp/ranges/as_rvalue_view) |
| `as_const` | Convert to const view | [as_const_view](https://en.cppreference.com/w/cpp/ranges/as_const_view) |
| `repeat(val)` | Infinite repetition | [repeat_view](https://en.cppreference.com/w/cpp/ranges/repeat_view) |
| `repeat(val, n)` | Finite repetition | [repeat_view](https://en.cppreference.com/w/cpp/ranges/repeat_view) |
