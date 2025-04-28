set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

cmake_minimum_required(VERSION 3.16)
project(RaylibTemplate)

set(CMAKE_CXX_STANDARD 17)

set(RAYLIB_PATH "C:/raylib-5.5_win64_mingw-w64/raylib-5.5_win64_mingw-w64")

# Include headers
include_directories(${RAYLIB_PATH}/include)
include_directories(${RAYLIB_PATH}/include/external)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the executable
file(GLOB SOURCES src/*.cpp)
add_executable(my_project ${SOURCES})

# Link statically to raylib
target_link_directories(my_project PRIVATE ${RAYLIB_PATH}/lib)
target_link_libraries(my_project raylib winmm gdi32 opengl32 shell32)
