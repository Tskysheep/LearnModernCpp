set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- 线程库（并发示例需要） ----
find_package(Threads REQUIRED)

macro(add_example NAME)
    add_executable(cpp14_${NAME} ${NAME}.cpp)
endmacro()

macro(add_example_threaded NAME)
    add_executable(cpp14_${NAME} ${NAME}.cpp)
    target_link_libraries(cpp14_${NAME} PRIVATE Threads::Threads)
endmacro()

add_example(01_generic_lambdas)
add_example(02_return_type_deduction)
add_example(03_variable_templates)
add_example(04_binary_literals_separators)
add_example(05_relaxed_constexpr)
add_example(06_make_unique)
add_example(07_deprecated_attribute)
add_example_threaded(08_shared_timed_mutex)
add_example(09_integer_sequence)
add_example(10_exchange)
add_example(11_quoted)
