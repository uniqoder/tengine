cmake_minimum_required(VERSION 3.25)

project(
    tengine
    VERSION 0.1.0
    DESCRIPTION "A terminal-based 3D game engine"
    LANGUAGES CXX
)

# --------------------------------------------------
# Global configuration
# --------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(TENGINE_BUILD_TESTS "Build engine tests" ON)
option(TENGINE_BUILD_EXAMPLES "Build engine examples" ON)

# --------------------------------------------------
# Build configuration
# --------------------------------------------------

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

include(CompilerWarnings)

# --------------------------------------------------
# Dependencies
# --------------------------------------------------

if(TENGINE_BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.16.0
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(Catch2)

    enable_testing()
endif()

# --------------------------------------------------
# Engine
# --------------------------------------------------

add_subdirectory(src)

# --------------------------------------------------
# Tests
# --------------------------------------------------

if(TENGINE_BUILD_TESTS)
    add_subdirectory(tests)
endif()

# --------------------------------------------------
# Examples
# --------------------------------------------------

if(TENGINE_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
