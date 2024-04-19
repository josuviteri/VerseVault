cmake_minimum_required(VERSION 3.27)
project(ProgIV)

set(CMAKE_CXX_STANDARD 23)

add_executable(ProgIV main.cpp)




include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)


set(SOURCE_FILES main.cpp)

target_link_libraries(ProgIV ${CMAKE_SOURCE_DIR}/lib/libcurl.dll.a)
