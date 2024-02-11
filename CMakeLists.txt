cmake_minimum_required(VERSION 3.28.3)

project(convertible_image)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "/O2")

add_subdirectory(lib/pybind11)
pybind11_add_module(convertible_image src/convertible_image.cpp)
target_include_directories(convertible_image PUBLIC lib/pybind11/include/)

add_custom_command(TARGET convertible_image POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:convertible_image>/convertible_image.cp312-win_amd64.pyd ${CMAKE_CURRENT_SOURCE_DIR}
)