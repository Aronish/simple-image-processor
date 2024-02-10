cmake_minimum_required(VERSION 3.28.3)

project(gaussian_blur)

set(CMAKE_CXX_FLAGS_RELEASE "/O2")

add_subdirectory(lib/pybind11)
pybind11_add_module(gaussian_blur src/gaussian_blur.cpp)
target_include_directories(gaussian_blur PUBLIC lib/pybind11/include/)

add_custom_command(TARGET gaussian_blur POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:gaussian_blur>/gaussian_blur.cp312-win_amd64.pyd ${CMAKE_CURRENT_SOURCE_DIR}
)