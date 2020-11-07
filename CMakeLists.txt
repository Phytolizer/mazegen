cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mazegen)
file(GLOB_RECURSE SOURCES "src/mazegen/*.cpp")
file(GLOB_RECURSE HEADERS "src/mazegen/*.hpp")
add_executable(mazegen ${SOURCES} ${HEADERS})

set(BOOST_ROOT "C:\\dev\\libs\\boost")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(mazegen Boost::headers Boost::program_options)

target_include_directories(mazegen PRIVATE "C:\\dev\\libs\\SDL2\\include")
target_link_directories(mazegen PRIVATE "C:\\dev\\libs\\SDL2\\lib\\x64")
target_link_libraries(mazegen SDL2 SDL2main)

target_link_directories(mazegen PRIVATE "${CMAKE_CURRENT_LIST_DIR}/voronoi/Release")
target_include_directories(mazegen PRIVATE "${CMAKE_CURRENT_LIST_DIR}/voronoi")
target_link_libraries(mazegen V)

target_link_directories(mazegen PRIVATE "${CMAKE_CURRENT_LIST_DIR}/delaunay/build/Debug")
target_include_directories(mazegen PRIVATE "${CMAKE_CURRENT_LIST_DIR}/delaunay")
target_link_libraries(mazegen delaunay)

target_link_directories(mazegen PRIVATE "${CMAKE_CURRENT_LIST_DIR}/quad")
target_include_directories(mazegen PRIVATE "${CMAKE_CURRENT_LIST_DIR}/quad")
target_link_libraries(mazegen quad)
