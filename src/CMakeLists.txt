cmake_minimum_required(VERSION 3.28)

project(PSS VERSION 1.0.0 DESCRIPTION "Problem Solving by Searching" LANGUAGES CXX)

# Enforce C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generate compile commands including definitions
# Use the compile_commands.json in C++ extension
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create project executable
add_executable(pss main.cpp graph.cpp maze.cpp cell.cpp search.cpp)

# Add compile-time constants
target_compile_definitions(pss PRIVATE TEST=123)
target_compile_options(pss PRIVATE -g)

# Add libraries
find_package(Boost MODULE REQUIRED)
target_link_libraries(pss PRIVATE Boost::boost)

# Copy res files to output for debugging
file(COPY "${CMAKE_SOURCE_DIR}/../res/input_0.txt" DESTINATION "${CMAKE_BINARY_DIR}/res")
file(COPY "${CMAKE_SOURCE_DIR}/../res/input_1.txt" DESTINATION "${CMAKE_BINARY_DIR}/res")
file(COPY "${CMAKE_SOURCE_DIR}/../res/input_2.txt" DESTINATION "${CMAKE_BINARY_DIR}/res")