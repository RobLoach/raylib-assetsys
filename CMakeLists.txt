cmake_minimum_required(VERSION 3.11)
project(SDL_assetsys
    DESCRIPTION "SDL_assetsys: Use the file system abstraction system, assetsys, with SDL."
    HOMEPAGE_URL "https://github.com/RobLoach/SDL_assetsys"
    VERSION 0.1.0
    LANGUAGES C
)

# SDL_assetsys
add_library(SDL_assetsys INTERFACE)
target_include_directories(SDL_assetsys INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Options
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(SDL_ASSETSYS_IS_MAIN TRUE)
else()
    set(SDL_ASSETSYS_IS_MAIN FALSE)
endif()

# Examples
if (SDL_ASSETSYS_IS_MAIN)
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
