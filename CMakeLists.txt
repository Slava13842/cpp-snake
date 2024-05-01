cmake_minimum_required(VERSION 3.12)
project(cpp-snake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(snake src/main.cpp)
if (MSVC)
  target_compile_options(snake PRIVATE /W4)
else()
  target_compile_options(snake PRIVATE -Wall -Wextra -Wpedantic)
endif()