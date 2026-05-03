cmake_minimum_required(VERSION 4.0)
project(MyModernApp LANGUAGES CXX)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(my_app)

target_sources(my_app PRIVATE
    src/main.cpp
    src/matrix.cpp
)

find_package(OpenMP REQUIRED)

target_include_directories(my_app PRIVATE include)

target_link_libraries(my_app PRIVATE OpenMP::OpenMP_CXX)

target_compile_options(my_app PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -pedantic
    -O3
    -march=native
    -fopenmp
)

target_link_options(my_app PRIVATE -fopenmp)
