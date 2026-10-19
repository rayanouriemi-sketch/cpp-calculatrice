cmake_minimum_required(VERSION 3.16)
project(cpp_calculatrice CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic)

add_library(calc_lib src/tokenizer.cpp src/calculator.cpp)
add_executable(calc src/main.cpp)
target_link_libraries(calc calc_lib)

enable_testing()
add_executable(test_calculator tests/test_calculator.cpp)
target_link_libraries(test_calculator calc_lib)
add_test(NAME test_calculator COMMAND test_calculator)
