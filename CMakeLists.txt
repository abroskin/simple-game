project(simple-game)
cmake_minimum_required(VERSION 2.8)
aux_source_directory("src" SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

configure_file("data/Blue.png" "data/Blue.png" COPYONLY)
configure_file("data/Green.png" "data/Green.png" COPYONLY)
configure_file("data/Purple.png" "data/Purple.png" COPYONLY)
configure_file("data/Red.png" "data/Red.png" COPYONLY)
configure_file("data/Yellow.png" "data/Yellow.png" COPYONLY)
configure_file("data/BackGround.jpg" "data/BackGround.jpg" COPYONLY)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
