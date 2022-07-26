cmake_minimum_required(VERSION 3.22)
project(Renderer)

set(CMAKE_CXX_STANDARD 17)

# 设置OpenCV目录所在
set(OpenCV_DIR "D:\\Env\\opencv\\mingw-build")

# 搜索OpenCV目录
find_package(OpenCV REQUIRED)

# 添加OpenCV头文件目录
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Renderer main.cpp gl.cpp gl.h Utils/matrix.cpp Utils/matrix.h triangle.h triangle.cpp scene.cpp scene.h
                        camera.cpp camera.h)

# 链接OpenCV库文件
target_link_libraries(Renderer ${OpenCV_LIBS})