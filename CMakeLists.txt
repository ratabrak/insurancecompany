cmake_minimum_required(VERSION 3.30)
project(insurance)

set(CMAKE_CXX_STANDARD 23)

add_executable(insurance src/main.cpp
        include/UI.h
        include/GameState.h
)

