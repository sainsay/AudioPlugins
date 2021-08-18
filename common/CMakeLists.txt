cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "Common")

project(${PROJECT_NAME})

file( GLOB_RECURSE common_source_list "${CMAKE_CURRENT_LIST_DIR}/src/*cpp" "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
file( GLOB_RECURSE common_private_list "${CMAKE_CURRENT_LIST_DIR}/src/*h" "${CMAKE_CURRENT_LIST_DIR}/src/*hpp") 
file( GLOB_RECURSE common_public_list "${CMAKE_CURRENT_LIST_DIR}/include/*h" "${CMAKE_CURRENT_LIST_DIR}/include/*hpp")

add_library(${PROJECT_NAME} STATIC 
    ${common_public_list}
    ${common_private_list}
    ${common_source_list}
)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include/)

