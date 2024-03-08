cmake_minimum_required(VERSION 3.28)

project(PSS VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Uninformed uninformed.cpp)
add_executable(Informed informed.cpp)

find_package(Boost CONFIG REQUIRED)

target_link_libraries(Uninformed PRIVATE Boost::boost)
target_link_libraries(Informed PRIVATE Boost::boost)