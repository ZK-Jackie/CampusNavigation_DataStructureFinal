cmake_minimum_required(VERSION 3.26)
project(CampusNavigation C)

set(CMAKE_C_STANDARD 11)

# find all C resources files
aux_source_directory(. DIR_SRCS)
# add subdirectory
add_subdirectory(src/graph)
add_subdirectory(src/queue)
add_subdirectory(src/utils)
# add iconv library
include_directories(src/utils/libiconv)
include_directories(src/graph)
include_directories(src/queue)
include_directories(src/utils)
include_directories(src)

add_executable(CampusNavigation main.c)

#link to libraries
target_link_libraries(CampusNavigation graphFunctions queueFunctions utilsFunctions)
