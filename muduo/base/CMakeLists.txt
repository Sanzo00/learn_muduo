# 将目录 .  下的所有源文件保存到变量DIR_BASE_SRCS
aux_source_directory(. DIR_BASE_SRCS)

# 生成链接库
add_library(muduo_base ${DIR_BASE_SRCS})

# 添加链接库
target_link_libraries(muduo_base pthread)

# 将所有的.h文件保存到变量 HEADERS
file(GLOB HEADERS "*.h")

# 安装到CMAKE_INSTALL_PREFIX, 默认为/usr/local
install(FILES ${HEADERS} DESTINATION include/muduo/base)
install(TARGETS muduo_base DESTINATION lib)
