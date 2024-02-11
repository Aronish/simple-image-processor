cmake_minimum_required(VERSION 3.28.3)

project(image_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "/O2")

add_subdirectory(lib/pybind11)
pybind11_add_module(image_processor src/convertible_image.cpp)
target_include_directories(image_processor PUBLIC lib/pybind11/include/)

add_custom_command(TARGET image_processor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:image_processor>/image_processor.cp312-win_amd64.pyd ${CMAKE_CURRENT_SOURCE_DIR}       
)