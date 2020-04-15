# c++11编译选项
add_compile_options(-std=c++11 -Wall)

# 生成可执行文件
add_executable(TcpClient TcpClient.cpp)
# 添加链接库
target_link_libraries(TcpClient muduo_base muduo_net)
