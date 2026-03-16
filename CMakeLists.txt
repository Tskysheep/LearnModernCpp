cmake_minimum_required(VERSION 3.20)
project(LearnCpp LANGUAGES CXX)

# ---- 编译警告设置 ----
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ---- 构建选项：控制每个标准版本的构建 ----
option(BUILD_CPP11 "构建 C++11 示例" ON)
option(BUILD_CPP14 "构建 C++14 示例" ON)
option(BUILD_CPP17 "构建 C++17 示例" ON)
option(BUILD_CPP20 "构建 C++20 示例" ON)
option(BUILD_CPP23 "构建 C++23 示例" ON)

# ---- 添加子目录 ----
if(BUILD_CPP11)
    add_subdirectory(cpp11)
endif()

if(BUILD_CPP14)
    add_subdirectory(cpp14)
endif()

if(BUILD_CPP17)
    add_subdirectory(cpp17)
endif()

if(BUILD_CPP20)
    add_subdirectory(cpp20)
endif()

if(BUILD_CPP23)
    add_subdirectory(cpp23)
endif()
