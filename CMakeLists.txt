cmake_minimum_required(VERSION 3.10)
project(eagle_maps)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add httplib library
include(FetchContent)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.13.3
)
FetchContent_MakeAvailable(httplib)

# List all source files explicitly
set(SOURCES
    main.cpp
    eaglemaps.cpp
    api_server.cpp
)

# Create executable
add_executable(eagle_maps_server ${SOURCES})

# Explicitly add httplib include directory
target_include_directories(eagle_maps_server PRIVATE 
    ${httplib_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/httplib-src
)

# Windows-specific libraries
if(WIN32)
    target_link_libraries(eagle_maps_server PRIVATE 
        Ws2_32 
        Crypt32
    )
endif()