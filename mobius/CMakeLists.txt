cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(morpheus-client)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif()
endif()

# Our Project
file(GLOB files "./data/fonts/*")
foreach(file ${files})
    file(RELATIVE_PATH file ${CMAKE_CURRENT_SOURCE_DIR} ${file})
    configure_file(${file} ${file} COPYONLY)
endforeach()

# project(test)
include_directories(include)
# file(GLOB SOURCES "src/*.cpp")
# add_executable(test ${SOURCES})

add_executable(${PROJECT_NAME}
    "src/main.cpp"
    "src/globals.cpp"
)
set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
# target_include_directories(${PROJECT_NAME} PRIVATE
#     "./include"
# )


# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
