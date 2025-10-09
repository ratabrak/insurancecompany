cmake_minimum_required(VERSION 3.30)
project(insurance)

set(CMAKE_CXX_STANDARD 23)
#set_target_properties(insurance PROPERTIES LINKER_LANGUAGE CXX)

include_directories(include)

add_subdirectory(dependencies)

file(GLOB insurance_src
        "src/*.h"
        "src/*.cpp"
)

message(${insurance_src})

add_executable(insurance ${insurance_src})
target_link_libraries(insurance PRIVATE ImGui-SFML::ImGui-SFML SFML::Window SFML::Graphics SFML::System)

