cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(BaseLibrary)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 base 目录下的所有源文件并存入变量 BASE_SOURCES
file(GLOB BASE_SOURCES "*.cpp")

# 生成静态库 libbase.a
add_library(base STATIC ${BASE_SOURCES})

# 指定库的包含目录
target_include_directories(base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

message("Base sources: ${BASE_SOURCES}")

