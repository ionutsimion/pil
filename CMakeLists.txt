cmake_minimum_required(VERSION 3.23)
project(pil)

set(CMAKE_CXX_STANDARD 17)

add_executable(pil main.cpp lexer.cpp lexer.hpp)
