cmake_minimum_required(VERSION 3.10)
project(MyProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 Protobuf 包
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

# 添加子目录 base
add_subdirectory(base)

# 添加包含目录
include_directories(base api proto/cggProto)

# 获取 api 目录下的所有 .cpp 文件
file(GLOB API_SOURCES "api/*.cpp")

# 获取 proto 目录下的所有 .pb.cc 文件
file(GLOB PROTO_SOURCES "proto/cggProto/*.pb.cc")

# 添加可执行文件
add_executable(MyProject server.cpp MsgConn.cpp ${API_SOURCES} ${PROTO_SOURCES})

# 链接 Protobuf 库和 base 静态库
target_link_libraries(MyProject base ${PROTOBUF_LIBRARIES})
