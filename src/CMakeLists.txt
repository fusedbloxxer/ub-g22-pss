cmake_minimum_required(VERSION 3.28)

project(PSS VERSION 1.0.0 DESCRIPTION "Problem Solving by Searching" LANGUAGES CXX)

# Enforce C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generate compile commands including definitions
# Use the compile_commands.json in C++ extension
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(pss main.cpp)
target_compile_definitions(pss PRIVATE TEST=123)

find_package(Boost CONFIG REQUIRED)
target_link_libraries(pss PRIVATE Boost::boost)