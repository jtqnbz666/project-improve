# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(MyProject)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加子目录 base , 意思是base目录有CMakeList.txt
# 只需要在最外层创建build再执行cmake ..， 不用单独创建base/build
add_subdirectory(base)

# 添加包含目录
include_directories(base api .)

# 获取 api 目录下的所有 .cpp 文件
file(GLOB API_SOURCES "api/*.cpp")

# 添加可执行文件
add_executable(MyProject server.cpp MsgConn.cpp  ${API_SOURCES})

# 链接静态库 libbase.a
target_link_libraries(MyProject base)
