cmake_minimum_required(VERSION 3.10)
project(maze_finder)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(maze_finder main.cpp windowapp.cpp algorithm.cpp algorithm.hpp point.hpp)
target_link_libraries(maze_finder sfml-graphics sfml-window sfml-system)



