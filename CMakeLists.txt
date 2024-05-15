cmake_minimum_required(VERSION 3.27)
project(VerseVault)

set(CMAKE_CXX_STANDARD 23)

add_executable(cliente curly/descargaArchivos.cpp
        menu/menu.cpp
        sqlite/sqlite3.c
        database/db2.cpp
        database/dbBusqueda.cpp
        client.cpp
        sockets2/ServerWithProtocol/main.cpp)




include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)


set(SOURCE_FILES curly/descargaArchivos.cpp)

target_link_libraries(cliente ${CMAKE_SOURCE_DIR}/lib/libcurl.dll.a ws2_32)


