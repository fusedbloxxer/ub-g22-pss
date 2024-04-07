cmake_minimum_required(VERSION 3.28)

project(PSS VERSION 1.0.0 DESCRIPTION "Problem Solving by Searching" LANGUAGES CXX)

# Enforce C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generate compile commands including definitions
# Use the compile_commands.json in C++ extension
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setup Project Paths
set(RESOURCES_DIR ${PROJECT_SOURCE_DIR}/../resources)

# List required source files
set(
    SRC_FILES
    main.cpp
    maze/cell.cpp
    maze/maze.cpp
    maze/graph.cpp
    search/search.cpp
    search/visitor.cpp
    search/problem.cpp
    manager/manager.cpp
    manager/event_manager.cpp
    manager/render_manager.cpp
    manager/resource_manager.cpp
    components/component.cpp
    components/scene_object.cpp
    animations/grid_animation.cpp
)

# Create project executable
add_executable(pss ${SRC_FILES})

# Add compile-time constants
target_compile_definitions(pss PRIVATE TEST=123)
target_compile_options(pss PRIVATE -g)

# Add Boost Graph Library
find_package(Boost MODULE REQUIRED)
target_link_libraries(pss PRIVATE Boost::boost)

# Add SFML Library
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(pss PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

# Copy res files to output for debugging
file(COPY ${RESOURCES_DIR} DESTINATION ${CMAKE_BINARY_DIR})