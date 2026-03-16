set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- 线程库（并行算法需要） ----
find_package(Threads REQUIRED)

macro(add_example NAME)
    add_executable(cpp17_${NAME} ${NAME}.cpp)
endmacro()

macro(add_example_threaded NAME)
    add_executable(cpp17_${NAME} ${NAME}.cpp)
    target_link_libraries(cpp17_${NAME} PRIVATE Threads::Threads)
endmacro()

# ---- 语言核心 ----
add_example(01_structured_bindings)
add_example(02_if_switch_init)
add_example(03_ctad)
add_example(04_fold_expressions)
add_example(05_constexpr_if)
add_example(06_inline_variables)
add_example(07_nested_namespaces)
add_example(08_has_include)

# ---- 词汇类型 ----
add_example(40_optional)
add_example(41_variant)
add_example(42_any)
add_example(43_string_view)
add_example(44_byte)
add_example(45_invoke_apply)

# ---- 文件系统和算法 ----
add_executable(cpp17_50_filesystem 50_filesystem.cpp)
# GCC < 9 需要链接 stdc++fs
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    target_link_libraries(cpp17_50_filesystem PRIVATE stdc++fs)
endif()

add_example_threaded(55_parallel_algorithms)
add_example(56_clamp_gcd_lcm)

# ---- 属性 ----
add_example(90_attributes)
