cmake_minimum_required(VERSION 3.11)
project(raylib-assetsys
    DESCRIPTION "raylib-assetsys: Use the file system abstraction system, assetsys, with raylib."
    HOMEPAGE_URL "https://github.com/RobLoach/raylib-assetsys"
    VERSION 0.1.0
    LANGUAGES C
)

# raylib_assetsys
add_library(raylib-assetsys INTERFACE)
target_include_directories(raylib-assetsys INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(RAYLIB_ASSETSYS_IS_MAIN TRUE)
else()
    set(RAYLIB_ASSETSYS_IS_MAIN FALSE)
endif()

# Examples
if (RAYLIB_ASSETSYS_IS_MAIN)
    # Testing
    include(CTest)
    enable_testing()
    if (BUILD_TESTING)
        set(CTEST_CUSTOM_TESTS_IGNORE
            pkg-config--static
        )
       add_subdirectory(test)
    endif()
endif()
