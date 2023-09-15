# 添加 core 库
add_subdirectory(core)

# 添加 obj_loader 库
add_subdirectory(obj_loader)

# 添加 stb 库
add_subdirectory(stb)

# 添加可执行文件
add_executable(AsukaRenderer main.cpp)

# 链接其他库
target_link_libraries(AsukaRenderer core obj_loader stb)
