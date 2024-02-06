cmake_minimum_required(VERSION 3.12)

project(dbms_cpp_backend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/database/context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/controller/linearSearchController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/index/blockStorage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test/test_loader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/model)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/database)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/controller)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/index)

add_executable(dbms_cpp_backend ${SOURCES})

set_target_properties(dbms_cpp_backend PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

if (MSVC)
    target_compile_options(dbms_cpp_backend PRIVATE /Zi /EHsc)
    set_target_properties(dbms_cpp_backend PROPERTIES LINK_FLAGS "/debug")
else()
    target_compile_options(dbms_cpp_backend PRIVATE -g -Wall -Wextra -Wpedantic)
endif()

set_target_properties(dbms_cpp_backend PROPERTIES OUTPUT_NAME dbms_cpp_backend)
