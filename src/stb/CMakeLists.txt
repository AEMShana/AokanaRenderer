# 添加库
add_library(stb INTERFACE)

# 设置头文件的包含路径
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})