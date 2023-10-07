# 获取当前目录下的所有源文件
file(GLOB CORE_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# 将源文件添加到库中
add_library(core ${CORE_SOURCE_FILES})
add_subdirectory(ui)

# 设置头文件的包含路径
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(core ui)