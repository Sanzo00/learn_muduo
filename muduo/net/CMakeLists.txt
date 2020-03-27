# CMake的最低版本
cmake_minimum_required( VERSION 2.8 )

# 指定项目名称
project(learn_muduo)

# 将目录 .  下的所有源文件保存到变量DIR_NET_SRCS
aux_source_directory(. DIR_NET_SRCS)

# 生成链接库
add_library(muduo_net ${DIR_NET_SRCS})

# 添加链接库
target_link_libraries(muduo_net muduo_base)

# 将所有的.h文件保存到变量 HEADERS
file(GLOB HEADERS "*.h")

# 安装到CMAKE_INSTALL_PREFIX, 默认为/usr/local
install(FILES ${HEADERS} DESTINATION include/muduo/net)
install(TARGETS muduo_net DESTINATION lib)
