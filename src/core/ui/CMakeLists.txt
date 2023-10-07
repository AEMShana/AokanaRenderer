# 获取当前目录下的所有源文件
file(GLOB UI_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# 将源文件添加到库中
add_library(ui ${UI_SOURCE_FILES})

# 设置头文件的包含路径
target_include_directories(ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})