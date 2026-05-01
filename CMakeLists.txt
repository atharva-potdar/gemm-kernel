cmake_minimum_required(VERSION 4.0)
project(MyModernApp LANGUAGES CXX)

add_executable(my_app src/main.cpp)

target_compile_features(my_app PRIVATE cxx_std_23)
set_target_properties(my_app PROPERTIES CXX_EXTENSIONS OFF)