cmake_minimum_required(VERSION 3.30)
project(insurance)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(dependencies)
add_executable(insurance src/main.cpp)

target_link_libraries(insurance PRIVATE ImGui-SFML::ImGui-SFML SFML::Window SFML::Graphics SFML::System)

