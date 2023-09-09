find_package(SDL2 REQUIRED)
set(SDL2_LIBRARIES SDL2::SDL2 SDL2::SDL2main)

# Resources
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/)

# SDL_assetsys_test
add_executable(SDL_assetsys_test SDL_assetsys_test.c)
target_compile_options(SDL_assetsys_test PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion)
target_link_libraries(SDL_assetsys_test PUBLIC
    ${SDL2_LIBRARIES}
    SDL_assetsys
)

# Set up the test
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
add_test(NAME SDL_assetsys_test COMMAND SDL_assetsys_test)