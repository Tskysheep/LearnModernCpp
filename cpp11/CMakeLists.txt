set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- 线程库（并发示例需要） ----
find_package(Threads REQUIRED)

macro(add_example NAME)
    add_executable(cpp11_${NAME} ${NAME}.cpp)
endmacro()

macro(add_example_threaded NAME)
    add_executable(cpp11_${NAME} ${NAME}.cpp)
    target_link_libraries(cpp11_${NAME} PRIVATE Threads::Threads)
endmacro()

# ---- 语言核心 ----
add_example(01_auto_and_decltype)
add_example(02_range_based_for)
add_example(03_lambda_expressions)
add_example(04_nullptr)
add_example(05_enum_class)
add_example(06_uniform_initialization)
add_example(07_default_delete)
add_example(08_override_final)
add_example(09_delegating_constructors)
add_example(10_inheriting_constructors)
add_example(11_move_semantics)
add_example(12_perfect_forwarding)
add_example(13_noexcept)
add_example(14_static_assert)
add_example(15_raw_string_literals)
add_example(16_unicode_literals)
add_example(17_user_defined_literals)
add_example(18_type_aliases)
add_example(19_alignof_alignas)

# ---- 模板与元编程 ----
add_example(20_variadic_templates)
add_example(21_constexpr)
add_example(22_type_traits)
add_example(23_initializer_list)

# ---- 标准库 - 容器与工具 ----
add_example(40_smart_pointers)
add_example(41_tuple)
add_example(42_array)
add_example(43_unordered_containers)
add_example(44_function_and_bind)
add_example(45_forward_list)
add_example(46_ref_wrapper)

# ---- 字符串与 I/O ----
add_example(60_string_conversions)
add_example(61_regex)

# ---- 并发 ----
add_example_threaded(70_thread)
add_example_threaded(71_mutex_and_locks)
add_example_threaded(72_condition_variable)
add_example_threaded(73_future_promise)
add_example_threaded(74_atomic)

# ---- 其他标准库 ----
add_example(80_chrono)
add_example(81_random)

# ---- 属性 ----
add_example(90_attributes)
