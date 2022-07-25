cmake_minimum_required(VERSION 3.22)
project(Renderer)

set(CMAKE_CXX_STANDARD 17)

add_executable(Renderer main.cpp gl.cpp gl.h Utils/matrix.cpp Utils/matrix.h triangle.h triangle.cpp scene.cpp scene.h)
