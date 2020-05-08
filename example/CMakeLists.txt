# c++11编译选项
add_compile_options(-std=c++11 -Wall)

# 生成可执行文件
add_executable(EchoServer EchoServer.cpp)
# 添加链接库
target_link_libraries(EchoServer muduo_net)
