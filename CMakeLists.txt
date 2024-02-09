cmake_minimum_required(VERSION 3.28.3)

project(gaussian_blur)

add_subdirectory(lib/pybind11)
pybind11_add_module(gaussian_blur src/gaussian_blur.cpp)
target_include_directories(gaussian_blur PRIVATE lib/pybind11/include/)