cmake_minimum_required(VERSION 3.10)
project(chip8_emulator)

set(CMAKE_CXX_STANDARD 17)

# add include directories
include_directories(include)

# add third-party libraries (locally)
set(SDL_DIR "libs/SDL")
add_subdirectory(${SDL_DIR})

# add SDL include directories after adding the subdirectory
include_directories(${SDL_INCLUDE_DIRS})

# add source files
file(GLOB SOURCES "src/*.cpp")

# add executable
add_executable(chip8_emulator ${SOURCES})

# link third-party libraries
target_link_libraries(chip8_emulator SDL2)
