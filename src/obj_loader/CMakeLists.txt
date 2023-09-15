# 添加库
add_library(obj_loader INTERFACE)

# 设置头文件的包含路径
target_include_directories(obj_loader INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})