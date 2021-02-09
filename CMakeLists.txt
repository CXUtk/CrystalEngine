# CMakeList.txt: CMakeTest 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project (CrystalEngine VERSION 1.0)

file (GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

file (GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/src/*.h")


# 将源代码添加到此项目的可执行文件。
add_executable (CrystalEngine ${SRC_FILES} ${HEADER_FILES})

set_target_properties (CrystalEngine PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      )
  

target_include_directories (CrystalEngine PUBLIC "${PROJECT_SOURCE_DIR}/deps")

include_directories (${PROJECT_SOURCE_DIR}/src)
include_directories (${PROJECT_SOURCE_DIR}/deps)

FIND_PACKAGE (OpenGL REQUIRED)

add_subdirectory ("deps/glad")
add_subdirectory ("deps/lodepng")


#target_link_directories (CMakeTest PRIVATE ${PROJECT_SOURCE_DIR}/deps/GLFW/lib)
target_link_libraries (CrystalEngine PRIVATE "${PROJECT_SOURCE_DIR}/deps/GLFW/lib/glfw3.lib")
target_link_libraries (CrystalEngine PRIVATE glad)
target_link_libraries (CrystalEngine PRIVATE lodepng)

add_custom_command(TARGET CrystalEngine PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${PROJECT_SOURCE_DIR}/Resources  ${PROJECT_BINARY_DIR}/Resources)


# TODO: 如有需要，请添加测试并安装目标。
