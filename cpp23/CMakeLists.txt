set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

macro(add_example NAME)
    add_executable(cpp23_${NAME} ${NAME}.cpp)
endmacro()

# ---- 语言核心 ----
add_example(01_if_consteval)
add_example(02_multidim_subscript)
add_example(03_size_t_literal)
add_example(05_lambda_improvements)

# ---- 标准库 ----
add_example(40_expected)
add_example(41_optional_monadic)
add_example(42_move_only_function)
add_example(43_to_underlying)
add_example(44_unreachable)

# ---- 其他 ----
add_example(61_stacktrace)
add_example(70_byteswap)

# ---- 属性 ----
add_example(90_attributes)
