set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- 线程库 ----
find_package(Threads REQUIRED)

macro(add_example NAME)
    add_executable(cpp20_${NAME} ${NAME}.cpp)
endmacro()

macro(add_example_threaded NAME)
    add_executable(cpp20_${NAME} ${NAME}.cpp)
    target_link_libraries(cpp20_${NAME} PRIVATE Threads::Threads)
endmacro()

# ---- 语言核心 ----
add_example(01_concepts_basics)
add_example(02_concepts_advanced)
add_example(03_requires_expressions)
add_example(04_abbreviated_templates)
add_example(05_spaceship_operator)
add_example(06_designated_initializers)
add_example(07_consteval)
add_example(08_constinit)
add_example(09_template_lambdas)
add_example(10_aggregate_parens_init)

# ---- Ranges ----
add_example(20_ranges_basics)
add_example(21_ranges_views)
add_example(22_ranges_custom)

# ---- 协程 ----
add_example(30_coroutines_basics)
add_example(31_coroutines_generator)

# ---- 标准库 ----
add_example(40_span)
add_example(41_format)
add_example(42_calendar_timezone)
add_example(43_source_location)
add_example(44_bit_cast)
add_example(45_math_constants)
add_example(46_string_operations)
add_example(47_contains)

# ---- 并发 ----
add_example_threaded(70_jthread)
add_example_threaded(71_barrier_latch)

# ---- 属性 ----
add_example(90_attributes)
