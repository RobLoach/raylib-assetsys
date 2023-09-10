# Add the cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)

find_package(Raylib)

# raylib-physfs-test
add_executable(raylib-assetsys-test raylib-assetsys-test.c)
target_link_libraries(raylib-assetsys-test PUBLIC
    raylib
    raylib-assetsys
)

# Copy the resources
file(GLOB resources resources/*)
set(test_resources)
list(APPEND test_resources ${resources})
file(COPY ${test_resources} DESTINATION "resources/")

# Set up the test
add_test(NAME raylib-assetsys-test COMMAND raylib-assetsys-test)
