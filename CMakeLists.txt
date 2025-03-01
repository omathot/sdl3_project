set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
project(sdl3_project VERSION 1.0)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

set(CMAKE_CXX_FLAGS DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    

set(SOURCE_FILES
    src/main.cpp
    src/app.cpp
    src/texture.cpp
)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(
    DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
    EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
set(LIBS)

CPMAddPackage("gh:libsdl-org/SDL#b3336c5")
list(APPEND LIBS SDL3::SDL3)

set(SDL_IMAGE_AVIF OFF CACHE OBOL "" FORCE)
CPMAddPackage("gh:libsdl-org/SDL_image#6b98396")
list(APPEND LIBS SDL3_image::SDL3_image)


set(SDLTTF_VENDORED ON)
CPMAddPackage("gh:libsdl-org/SDL_ttf#347914e")
list(APPEND LIBS SDL3_ttf::SDL3_ttf)


CPMAddPackage("gh:SanderMertens/flecs#8a1e9ab")
list(APPEND LIBS flecs::flecs)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
