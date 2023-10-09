cmake_minimum_required(VERSION 3.12)
project(AsukaRenderer)

# 设置 C++ 的标准版本
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(/NODEFAULTLIB:MSVCRT)

if(WIN32)
    add_compile_options(/W4)
    add_compile_options(/wd4819)
endif()

# 添加源代码目录
add_subdirectory(src)

# 添加外部依赖项
add_subdirectory(external)

set_target_properties(AsukaRenderer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)