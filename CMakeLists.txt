cmake_minimum_required(VERSION 3.25)
project(imapcl)

set(CMAKE_CXX_STANDARD 20)

add_executable(imapcl main.cpp
        parser.cpp
        parser.h
        error.cpp
        error.h)
