cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
project(sdl3_project VERSION 1.0)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

include(CMakePrintHelpers)
cmake_print_variable(CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    

set(SOURCE_FILES
    src/main.cpp
    src/app.cpp
    src/texture.cpp
    src/player.cpp
    src/assetManager.cpp
    src/animation.cpp
)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(
    DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
set(LIBS)

CPMAddPackage("gh:libsdl-org/SDL#release-3.2.10")
list(APPEND LIBS SDL3::SDL3)

set(SDL_IMAGE_AVIF OFF)
CPMAddPackage("gh:libsdl-org/SDL_image#release-3.2.4")
list(APPEND LIBS SDL3_image::SDL3_image)


CPMAddPackage("gh:libsdl-org/SDL_ttf#release-3.2.2")
list(APPEND LIBS SDL3_ttf::SDL3_ttf)


# CPMAddPackage("gh:SanderMertens/flecs#v4.0.5")
# list(APPEND LIBS flecs::flecs)

CPMAddPackage("gh:gabime/spdlog#v1.15.2")
list(APPEND LIBS spdlog::spdlog)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
