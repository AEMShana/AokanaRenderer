# 添加 core 库
add_subdirectory(core)

# 添加 obj_loader 库
add_subdirectory(obj_loader)

# 添加 stb 库
add_subdirectory(stb)

# add_subdirectory(ui)

# 添加可执行文件
add_executable(AsukaRenderer main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../external/glad/glad.c)

target_include_directories(AsukaRenderer PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/glad)

# 链接其他库
target_link_libraries(AsukaRenderer core obj_loader stb ui ${CMAKE_CURRENT_SOURCE_DIR}/../external/glfw/lib/glfw3.lib)
