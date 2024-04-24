cmake_minimum_required(VERSION 3.27)
project(ProgIV)

set(CMAKE_CXX_STANDARD 23)

add_executable(ProgIV curly.cpp
        menu/menu.cpp
        sqlite/sqlite3.c
        database/db2.cpp
        database/dbBusqueda.cpp
        main.cpp)




include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)


set(SOURCE_FILES curly.cpp)

target_link_libraries(ProgIV ${CMAKE_SOURCE_DIR}/lib/libcurl.dll.a)


