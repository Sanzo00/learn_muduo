# CMake的最低版本
cmake_minimum_required( VERSION 2.8 )

# 指定项目名称
project(learn_muduo)

# 将目录 .  下的所有源文件保存到变量DIR_SRCS
aux_source_directory(. DIR_SRCS)

# 指定头文件目录
include_directories(.)

# 添加子目录
add_subdirectory(muduo/base)
add_subdirectory(muduo/net)

# c++11编译选项
add_compile_options(-std=c++11 -Wall)

# 生成可执行文件
add_executable(main ${DIR_SRCS})

# 添加链接库
target_link_libraries(main muduo_base muduo_net)

option(MUDUO_BUILD_TEST "Build muduo test" ON)
if (MUDUO_BUILD_TEST)
	add_subdirectory(test)
endif()


