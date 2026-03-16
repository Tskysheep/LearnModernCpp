---
sidebar_position: 1
title: Getting Started
description: "Complete Modern C++ tutorial with 95 independently compilable examples covering C++11 through C++23, one file per feature, in progressive order."
tags: [C++, tutorial, modern-cpp, getting-started]
---

# LearnCpp — Complete Modern C++ Tutorial with Examples

> Covering 95 independently compilable examples from C++11 to C++23, one file per feature, in progressive order.

---

## Build Instructions

```bash
# Build all examples
cmake -B build -G "MinGW Makefiles" && cmake --build build

# Build only a specific standard version
cmake -B build -DBUILD_CPP11=ON -DBUILD_CPP14=OFF -DBUILD_CPP17=OFF -DBUILD_CPP20=OFF -DBUILD_CPP23=OFF
cmake --build build

# Run a single example
./build/cpp11/cpp11_01_auto_and_decltype
```

---

## Recommended Learning Path

### Phase 1: C++11 Fundamentals (Essential)
C++11 is the starting point of modern C++. Nearly all subsequent features build upon it.

:::tip Suggested Order
1. Type deduction -> `01_auto_and_decltype`
2. Range-based for -> `02_range_based_for`
3. Lambda expressions -> `03_lambda_expressions`
4. Smart pointers -> `40_smart_pointers` **Core**
5. Move semantics -> `11_move_semantics` **Core**
6. Rvalue references and forwarding -> `12_perfect_forwarding`
7. Remaining core language features (04-19)
8. Containers and utilities (41-46)
9. Concurrency (70-74)
:::

### Phase 2: C++14 Refinements
C++14 is a minor improvement over C++11.

:::info Suggested Order
Read in order from 01 to 11.
:::

### Phase 3: C++17 Practical Upgrades
Focus on **vocabulary types** (optional/variant/any) and **string_view**.

:::tip Suggested Order
1. Structured bindings -> `01_structured_bindings`
2. if constexpr -> `05_constexpr_if`
3. optional/variant/any -> `40-42` **Core**
4. string_view -> `43_string_view` **Core**
5. filesystem -> `50_filesystem`
6. Remaining features
:::

### Phase 4: C++20 Major Release
C++20 is the largest standard update since C++11.

:::tip Suggested Order
1. Concepts -> `01-04` **Core**
2. Ranges -> `20-22` **Core**
3. Three-way comparison -> `05_spaceship_operator`
4. Coroutines -> `30-31` (Advanced)
5. std::format -> `41_format`
6. Remaining standard library features
:::

### Phase 5: C++23 Cutting Edge
C++23 is gradually gaining compiler support.

:::tip Suggested Order
1. std::expected -> `40_expected` **Core**
2. optional monadic operations -> `41_optional_monadic`
3. Remaining features
:::

---

## Full Table of Contents

### C++11 (33 Examples) -> [Detailed Documentation](cpp11/index.md)

| No. | File | Feature |
|-----|------|---------|
| 01 | `01_auto_and_decltype.cpp` | auto type deduction, decltype |
| 02 | `02_range_based_for.cpp` | Range-based for loop |
| 03 | `03_lambda_expressions.cpp` | Lambda expressions |
| 04 | `04_nullptr.cpp` | nullptr keyword |
| 05 | `05_enum_class.cpp` | Scoped enumerations (enum class) |
| 06 | `06_uniform_initialization.cpp` | Uniform initialization {} |
| 07 | `07_default_delete.cpp` | = default, = delete |
| 08 | `08_override_final.cpp` | override, final |
| 09 | `09_delegating_constructors.cpp` | Delegating constructors |
| 10 | `10_inheriting_constructors.cpp` | Inheriting constructors |
| 11 | `11_move_semantics.cpp` | Move semantics **Core** |
| 12 | `12_perfect_forwarding.cpp` | Perfect forwarding |
| 13 | `13_noexcept.cpp` | noexcept |
| 14 | `14_static_assert.cpp` | static_assert |
| 15 | `15_raw_string_literals.cpp` | Raw string literals |
| 16 | `16_unicode_literals.cpp` | Unicode literals |
| 17 | `17_user_defined_literals.cpp` | User-defined literals |
| 18 | `18_type_aliases.cpp` | using type aliases |
| 19 | `19_alignof_alignas.cpp` | alignof, alignas |
| 20 | `20_variadic_templates.cpp` | Variadic templates |
| 21 | `21_constexpr.cpp` | constexpr |
| 22 | `22_type_traits.cpp` | type_traits |
| 23 | `23_initializer_list.cpp` | initializer_list |
| 40 | `40_smart_pointers.cpp` | Smart pointers **Core** |
| 41 | `41_tuple.cpp` | tuple |
| 42 | `42_array.cpp` | std::array |
| 43 | `43_unordered_containers.cpp` | Unordered containers |
| 44 | `44_function_and_bind.cpp` | function, bind |
| 45 | `45_forward_list.cpp` | forward_list |
| 46 | `46_ref_wrapper.cpp` | reference_wrapper |
| 60 | `60_string_conversions.cpp` | String conversions |
| 61 | `61_regex.cpp` | Regular expressions |
| 70 | `70_thread.cpp` | Threads |
| 71 | `71_mutex_and_locks.cpp` | Mutexes and locks |
| 72 | `72_condition_variable.cpp` | Condition variables |
| 73 | `73_future_promise.cpp` | future/promise |
| 74 | `74_atomic.cpp` | Atomic operations |
| 80 | `80_chrono.cpp` | chrono time library |
| 81 | `81_random.cpp` | Random numbers |
| 90 | `90_attributes.cpp` | Attributes |

### C++14 (11 Examples) -> [Detailed Documentation](cpp14/index.md)

| No. | File | Feature |
|-----|------|---------|
| 01 | `01_generic_lambdas.cpp` | Generic lambdas |
| 02 | `02_return_type_deduction.cpp` | Return type deduction |
| 03 | `03_variable_templates.cpp` | Variable templates |
| 04 | `04_binary_literals_separators.cpp` | Binary literals, digit separators |
| 05 | `05_relaxed_constexpr.cpp` | Relaxed constexpr |
| 06 | `06_make_unique.cpp` | make_unique |
| 07 | `07_deprecated_attribute.cpp` | [[deprecated]] |
| 08 | `08_shared_timed_mutex.cpp` | Shared (reader-writer) locks |
| 09 | `09_integer_sequence.cpp` | integer_sequence |
| 10 | `10_exchange.cpp` | std::exchange |
| 11 | `11_quoted.cpp` | std::quoted |

### C++17 (16 Examples) -> [Detailed Documentation](cpp17/index.md)

| No. | File | Feature |
|-----|------|---------|
| 01 | `01_structured_bindings.cpp` | Structured bindings **Core** |
| 02 | `02_if_switch_init.cpp` | if/switch with initializer |
| 03 | `03_ctad.cpp` | Class template argument deduction (CTAD) |
| 04 | `04_fold_expressions.cpp` | Fold expressions |
| 05 | `05_constexpr_if.cpp` | if constexpr **Core** |
| 06 | `06_inline_variables.cpp` | inline variables |
| 07 | `07_nested_namespaces.cpp` | Nested namespaces |
| 08 | `08_has_include.cpp` | \_\_has\_include |
| 40 | `40_optional.cpp` | std::optional **Core** |
| 41 | `41_variant.cpp` | std::variant **Core** |
| 42 | `42_any.cpp` | std::any |
| 43 | `43_string_view.cpp` | string_view **Core** |
| 44 | `44_byte.cpp` | std::byte |
| 45 | `45_invoke_apply.cpp` | invoke, apply |
| 50 | `50_filesystem.cpp` | filesystem |
| 55 | `55_parallel_algorithms.cpp` | Parallel algorithms |
| 56 | `56_clamp_gcd_lcm.cpp` | clamp, gcd, lcm |
| 90 | `90_attributes.cpp` | Attributes |

### C++20 (23 Examples) -> [Detailed Documentation](cpp20/index.md)

| No. | File | Feature |
|-----|------|---------|
| 01 | `01_concepts_basics.cpp` | Concepts basics **Core** |
| 02 | `02_concepts_advanced.cpp` | Concepts advanced |
| 03 | `03_requires_expressions.cpp` | requires expressions |
| 04 | `04_abbreviated_templates.cpp` | Abbreviated function templates |
| 05 | `05_spaceship_operator.cpp` | Three-way comparison `<=>` **Core** |
| 06 | `06_designated_initializers.cpp` | Designated initializers |
| 07 | `07_consteval.cpp` | consteval |
| 08 | `08_constinit.cpp` | constinit |
| 09 | `09_template_lambdas.cpp` | Template lambdas |
| 10 | `10_aggregate_parens_init.cpp` | Parenthesized aggregate initialization |
| 20 | `20_ranges_basics.cpp` | Ranges basics **Core** |
| 21 | `21_ranges_views.cpp` | Ranges views **Core** |
| 22 | `22_ranges_custom.cpp` | Custom Range |
| 30 | `30_coroutines_basics.cpp` | Coroutines basics |
| 31 | `31_coroutines_generator.cpp` | Coroutine Generator |
| 40 | `40_span.cpp` | std::span |
| 41 | `41_format.cpp` | std::format |
| 42 | `42_calendar_timezone.cpp` | Calendar and time zones |
| 43 | `43_source_location.cpp` | source_location |
| 44 | `44_bit_cast.cpp` | bit_cast |
| 45 | `45_math_constants.cpp` | Mathematical constants |
| 46 | `46_string_operations.cpp` | starts_with/ends_with |
| 47 | `47_contains.cpp` | contains() |
| 70 | `70_jthread.cpp` | jthread |
| 71 | `71_barrier_latch.cpp` | barrier, latch |
| 90 | `90_attributes.cpp` | Attributes |

### C++23 (12 Compilable Examples + Documentation) -> [Detailed Documentation](cpp23/index.md)

| No. | File | Feature |
|-----|------|---------|
| 01 | `01_if_consteval.cpp` | if consteval |
| 02 | `02_multidim_subscript.cpp` | Multidimensional subscript operator |
| 03 | `03_size_t_literal.cpp` | size_t literal suffix |
| 05 | `05_lambda_improvements.cpp` | Lambda improvements |
| 40 | `40_expected.cpp` | std::expected **Core** |
| 41 | `41_optional_monadic.cpp` | optional monadic operations |
| 42 | `42_move_only_function.cpp` | move_only_function |
| 43 | `43_to_underlying.cpp` | to_underlying |
| 44 | `44_unreachable.cpp` | std::unreachable |
| 61 | `61_stacktrace.cpp` | stacktrace |
| 70 | `70_byteswap.cpp` | byteswap |
| 90 | `90_attributes.cpp` | [[assume]] |

**Documentation-only features** (see [cpp23 documentation](cpp23/index.md)):
- Deducing this
- std::print/println
- std::mdspan
- std::flat_map/flat_set
- std::generator
- static operator()
- New std::ranges views

---

## Statistics

| Standard | .cpp Files | Documentation |
|----------|-----------|---------------|
| C++11 | 33 | cpp11.md |
| C++14 | 11 | cpp14.md |
| C++17 | 16 | cpp17.md |
| C++20 | 23 | cpp20.md |
| C++23 | 12 | cpp23.md |
| **Total** | **95** | **5 + content.md** |
