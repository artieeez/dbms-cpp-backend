cmake_minimum_required(VERSION 3.12)

project(test_block)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/database/context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/index/blockStorage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test/test_block.cpp
)

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/model)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/database)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/index)

# Create the executable
add_executable(test_block ${SOURCES})

# Set the output directories for object files and executable
set_target_properties(test_block PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build
)

# Set compiler flags
target_compile_options(test_block PRIVATE /Zi /EHsc)

# Set linker flags
set_target_properties(test_block PROPERTIES LINK_FLAGS "/debug")

# Specify the output file name
set_target_properties(test_block PROPERTIES OUTPUT_NAME test_block)
