---
sidebar_position: 1
title: "C++11 Features Overview"
description: "Comprehensive guide to C++11 features — the milestone of modern C++, introducing move semantics, lambda expressions, smart pointers, concurrency support, and more."
tags: [C++11, modern-cpp, move-semantics, lambda, smart-pointers, concurrency]
---

# C++11 Features in Detail

> C++11 is the milestone of modern C++. It introduced move semantics, lambda expressions, smart pointers, concurrency support, and other major features that fundamentally changed the way C++ is written.

---

## Core Language Features

### 01. auto Type Deduction and decltype (`01_auto_and_decltype.cpp`)
- **auto** lets the compiler automatically deduce variable types, eliminating verbose iterator declarations
- **decltype** queries the type of an expression, used for type deduction in generic programming
- Trailing return type `auto f() -> T` supports return types that depend on parameters
- Reference: [auto](https://en.cppreference.com/w/cpp/language/auto) | [decltype](https://en.cppreference.com/w/cpp/language/decltype)

### 02. Range-based for Loop (`02_range_based_for.cpp`)
- `for (auto& x : container)` replaces traditional iterator-based loops
- Works with any type that provides `begin()`/`end()`
- Eliminates out-of-bounds indexing risks with cleaner code
- Reference: [range-based for](https://en.cppreference.com/w/cpp/language/range-for)

### 03. Lambda Expressions (`03_lambda_expressions.cpp`)
- Syntax: `[capture](parameters) -> return_type { body }`
- Capture modes: `[=]` capture by value, `[&]` capture by reference, `[this]` capture this pointer
- Replaces verbose functor classes; pass behavior directly to STL algorithms
- Reference: [lambda](https://en.cppreference.com/w/cpp/language/lambda)

### 04. nullptr Keyword (`04_nullptr.cpp`)
- Type-safe null pointer constant with type `std::nullptr_t`
- Fixes ambiguity of `NULL`/`0` in function overload resolution
- Reference: [nullptr](https://en.cppreference.com/w/cpp/language/nullptr) | [std::nullptr_t](https://en.cppreference.com/w/cpp/types/nullptr_t)

### 05. Scoped Enumerations — enum class (`05_enum_class.cpp`)
- `enum class` does not implicitly convert to integers
- Enumerator names do not leak into the enclosing scope
- The underlying type can be explicitly specified
- Reference: [enum](https://en.cppreference.com/w/cpp/language/enum)

### 06. Uniform Initialization (`06_uniform_initialization.cpp`)
- Braces `{}` provide a consistent initialization syntax
- Prevents narrowing conversions (e.g., `int x{3.14}` causes a compile error)
- Resolves the most vexing parse problem
- Reference: [list initialization](https://en.cppreference.com/w/cpp/language/list_initialization) | [value initialization](https://en.cppreference.com/w/cpp/language/value_initialization)

### 07. = default and = delete (`07_default_delete.cpp`)
- `= default` explicitly requests the compiler to generate the default implementation
- `= delete` prohibits specific functions (e.g., copy constructor)
- Clearly expresses design intent
- Reference: [default constructor](https://en.cppreference.com/w/cpp/language/default_constructor) | [deleted functions](https://en.cppreference.com/w/cpp/language/function#Deleted_functions)

### 08. override and final Specifiers (`08_override_final.cpp`)
- `override` ensures a function actually overrides a base class virtual function
- `final` prevents further inheritance or overriding
- The compiler catches subtle bugs from signature mismatches
- Reference: [override](https://en.cppreference.com/w/cpp/language/override) | [final](https://en.cppreference.com/w/cpp/language/final)

### 09. Delegating Constructors (`09_delegating_constructors.cpp`)
- One constructor can call another constructor of the same class
- Eliminates code duplication between constructors
- Reference: [delegating constructor](https://en.cppreference.com/w/cpp/language/constructor#Delegating_constructor)

### 10. Inheriting Constructors (`10_inheriting_constructors.cpp`)
- `using Base::Base;` automatically inherits base class constructors
- Avoids writing numerous forwarding constructors in derived classes
- Reference: [inheriting constructors](https://en.cppreference.com/w/cpp/language/using_declaration#Inheriting_constructors)

### 11. Move Semantics (`11_move_semantics.cpp`)

:::warning Core Feature
Move semantics is one of the most important features in C++11.
:::

- Rvalue references `&&` identify temporary objects about to be destroyed
- `std::move` casts an lvalue to an rvalue reference, enabling move operations
- Move constructor / move assignment transfers resource ownership, avoiding deep copies
- Reference: [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) | [std::move](https://en.cppreference.com/w/cpp/utility/move) | [rvalue reference](https://en.cppreference.com/w/cpp/language/reference)

### 12. Perfect Forwarding (`12_perfect_forwarding.cpp`)
- Universal reference `T&&` (in a template parameter deduction context)
- `std::forward<T>` preserves the value category of the argument
- Reference collapsing rules: `& + && = &`, `&& + && = &&`
- Reference: [std::forward](https://en.cppreference.com/w/cpp/utility/forward) | [reference collapsing](https://en.cppreference.com/w/cpp/language/reference#Reference_collapsing)

### 13. noexcept Specifier (`13_noexcept.cpp`)
- Marks a function as guaranteed not to throw exceptions
- Containers only use move operations when moves are `noexcept` (otherwise they fall back to copies)
- The `noexcept` operator checks whether an expression is non-throwing
- Reference: [noexcept specifier](https://en.cppreference.com/w/cpp/language/noexcept_spec) | [noexcept operator](https://en.cppreference.com/w/cpp/language/noexcept)

### 14. static_assert Compile-time Assertion (`14_static_assert.cpp`)
- Compile-time assertion that outputs a custom error message on failure
- Often used with `type_traits` to check template parameters
- Reference: [static_assert](https://en.cppreference.com/w/cpp/language/static_assert)

### 15. Raw String Literals (`15_raw_string_literals.cpp`)
- In `R"(content)"`, backslashes are not escape characters
- A lifesaver for regular expressions, Windows paths, and multiline strings
- Reference: [string literal](https://en.cppreference.com/w/cpp/language/string_literal)

### 16. Unicode String Literals (`16_unicode_literals.cpp`)
- `u8""`, `u""`, `U""` correspond to UTF-8, UTF-16, and UTF-32 respectively
- New types `char16_t` and `char32_t` introduced
- Reference: [string literal](https://en.cppreference.com/w/cpp/language/string_literal) | [char16_t](https://en.cppreference.com/w/cpp/language/types#char16_t)

### 17. User-defined Literals (`17_user_defined_literals.cpp`)
- `operator""` implements custom suffixes (e.g., `10_km`, `3.14_deg`)
- The standard library provides `s` (string), `ms` (chrono), etc.
- Reference: [user-defined literals](https://en.cppreference.com/w/cpp/language/user_literal)

### 18. using Type Aliases (`18_type_aliases.cpp`)
- `using Name = Type;` replaces `typedef`
- Supports template aliases: `template<typename T> using Vec = std::vector<T>;`
- Reference: [type alias](https://en.cppreference.com/w/cpp/language/type_alias)

### 19. alignof and alignas (`19_alignof_alignas.cpp`)
- `alignof` queries the alignment requirement of a type
- `alignas` specifies the alignment of a variable or type
- Useful for performance optimization scenarios such as cache-line alignment
- Reference: [alignof](https://en.cppreference.com/w/cpp/language/alignof) | [alignas](https://en.cppreference.com/w/cpp/language/alignas)

---

## Templates and Metaprogramming

### 20. Variadic Templates (`20_variadic_templates.cpp`)
- `template<typename... Args>` accepts any number of template parameters
- Recursively expand parameter packs
- `sizeof...(Args)` gets the number of parameters
- Reference: [parameter pack](https://en.cppreference.com/w/cpp/language/parameter_pack)

### 21. constexpr Compile-time Evaluation (`21_constexpr.cpp`)
- `constexpr` functions can be evaluated at compile time
- C++11 has strict limitations (single return statement only)
- `constexpr` variables must be initialized at compile time
- Reference: [constexpr](https://en.cppreference.com/w/cpp/language/constexpr)

### 22. type_traits Compile-time Type Introspection (`22_type_traits.cpp`)
- Compile-time type inspection: `is_integral`, `is_same`, `is_pointer`, etc.
- `enable_if` implements SFINAE
- Type transformations: `conditional`, `remove_const`, etc.
- Reference: [type_traits](https://en.cppreference.com/w/cpp/header/type_traits) | [enable_if](https://en.cppreference.com/w/cpp/types/enable_if)

### 23. initializer_list (`23_initializer_list.cpp`)
- `std::initializer_list<T>` enables brace-enclosed initialization syntax
- The underlying mechanism for `vector<int> v = {1, 2, 3};`
- Reference: [std::initializer_list](https://en.cppreference.com/w/cpp/utility/initializer_list)

---

## Standard Library — Containers and Utilities

### 40. Smart Pointers (`40_smart_pointers.cpp`)

:::warning Core Feature
Smart pointers are essential for modern C++ memory management.
:::

- **unique_ptr**: Exclusive ownership, zero-overhead abstraction
- **shared_ptr**: Shared ownership with reference counting
- **weak_ptr**: Breaks circular references
- `make_shared`/`make_unique` (C++14) are more efficient
- Reference: [unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) | [shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) | [weak_ptr](https://en.cppreference.com/w/cpp/memory/weak_ptr)

### 41. tuple (`41_tuple.cpp`)
- Fixed-size collection of heterogeneous values
- `std::tie` unpacks into variables
- Supports comparison and hashing
- Reference: [std::tuple](https://en.cppreference.com/w/cpp/utility/tuple) | [std::tie](https://en.cppreference.com/w/cpp/utility/tuple/tie)

### 42. std::array (`42_array.cpp`)
- Fixed-size array with an STL interface
- `at()` provides bounds checking
- Replaces C-style arrays
- Reference: [std::array](https://en.cppreference.com/w/cpp/container/array)

### 43. Unordered Containers (`43_unordered_containers.cpp`)
- `unordered_map`/`unordered_set` are hash-table-based
- O(1) average lookup time
- Custom types require a hash function
- Reference: [unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map) | [unordered_set](https://en.cppreference.com/w/cpp/container/unordered_set)

### 44. std::function and std::bind (`44_function_and_bind.cpp`)
- `std::function` is a type-erased general-purpose callable wrapper
- `std::bind` binds some arguments (partial application)
- In modern C++, lambdas have largely replaced bind
- Reference: [std::function](https://en.cppreference.com/w/cpp/utility/functional/function) | [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind)

### 45. forward_list — Singly-linked List (`45_forward_list.cpp`)
- Singly-linked list with minimal memory overhead
- Only supports forward iteration
- `insert_after`/`erase_after` operations
- Reference: [std::forward_list](https://en.cppreference.com/w/cpp/container/forward_list)

### 46. reference_wrapper (`46_ref_wrapper.cpp`)
- `std::ref`/`std::cref` create reference wrappers
- Solves reference passing issues in templates and threads
- Reference: [std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper) | [std::ref](https://en.cppreference.com/w/cpp/utility/functional/ref)

---

## Strings and I/O

### 60. String Conversions (`60_string_conversions.cpp`)
- `std::to_string` converts numbers to strings
- `std::stoi`/`stol`/`stof` etc. convert strings to numbers
- Replaces unsafe `sprintf`/`sscanf`
- Reference: [std::to_string](https://en.cppreference.com/w/cpp/string/basic_string/to_string) | [std::stoi](https://en.cppreference.com/w/cpp/string/basic_string/stol)

### 61. Regular Expressions (`61_regex.cpp`)
- `regex_match`: Full match
- `regex_search`: Search for substrings
- `regex_replace`: Replace matches
- Performance note: Compiling a regex object is expensive
- Reference: [regex](https://en.cppreference.com/w/cpp/regex) | [std::regex_match](https://en.cppreference.com/w/cpp/regex/regex_match)

---

## Concurrency

### 70. Threads (`70_thread.cpp`)
- The first standard threading library in C++
- `std::thread` creates threads
- `join()` waits for completion, `detach()` detaches the thread
- Reference: [std::thread](https://en.cppreference.com/w/cpp/thread/thread)

### 71. Mutexes and Locks (`71_mutex_and_locks.cpp`)
- `std::mutex` protects shared data
- `lock_guard` provides RAII-based automatic lock management
- `unique_lock` offers more flexible lock control
- Reference: [std::mutex](https://en.cppreference.com/w/cpp/thread/mutex) | [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard) | [std::unique_lock](https://en.cppreference.com/w/cpp/thread/unique_lock)

### 72. Condition Variables (`72_condition_variable.cpp`)
- The standard implementation of the producer-consumer pattern
- `wait`/`notify_one`/`notify_all`
- Beware of spurious wakeups
- Reference: [std::condition_variable](https://en.cppreference.com/w/cpp/thread/condition_variable)

### 73. future and promise (`73_future_promise.cpp`)
- `std::async` launches asynchronous tasks
- `promise`-`future` is a cross-thread communication channel
- `packaged_task` wraps callable objects
- Reference: [std::future](https://en.cppreference.com/w/cpp/thread/future) | [std::promise](https://en.cppreference.com/w/cpp/thread/promise) | [std::async](https://en.cppreference.com/w/cpp/thread/async)

### 74. Atomic Operations (`74_atomic.cpp`)
- `std::atomic<T>` provides lock-free atomic operations
- Memory ordering controls visibility of concurrent accesses
- Enables lock-free counters and more
- Reference: [std::atomic](https://en.cppreference.com/w/cpp/atomic/atomic) | [memory order](https://en.cppreference.com/w/cpp/atomic/memory_order)

---

## Miscellaneous

### 80. chrono Time Library (`80_chrono.cpp`)
- `duration`: Type-safe time duration
- `time_point`: A point in time
- `steady_clock`: Monotonic clock (preferred for timing)
- Reference: [chrono](https://en.cppreference.com/w/cpp/chrono) | [std::chrono::duration](https://en.cppreference.com/w/cpp/chrono/duration) | [std::chrono::steady_clock](https://en.cppreference.com/w/cpp/chrono/steady_clock)

### 81. Random Number Generation (`81_random.cpp`)
- `mt19937`: Mersenne Twister engine
- `uniform_int_distribution` and other distributions
- Replaces the flawed `rand()`
- Reference: [random](https://en.cppreference.com/w/cpp/numeric/random) | [std::mt19937](https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine)

### 90. Attributes (`90_attributes.cpp`)
- `[[noreturn]]`: Marks functions that never return
- C++11 introduced the standardized attribute syntax
- Reference: [attributes](https://en.cppreference.com/w/cpp/language/attributes) | [[\[noreturn\]]](https://en.cppreference.com/w/cpp/language/attributes/noreturn)
