cmake_minimum_required(VERSION 3.14)
project(Pathfinding_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Pathfinding_project src/main.cpp libs/xml-parser/tinyxml2.cpp)