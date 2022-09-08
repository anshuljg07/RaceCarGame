cmake_minimum_required(VERSION 3.10)
project(Racing)

set(CMAKE_CXX_STANDARD 11)

configure_file(images/background.png images/background.png COPYONLY)
configure_file(images/car.png images/car.png COPYONLY)
configure_file(images/spaceship.png images/spaceship.png COPYONLY)
configure_file(images/heli.png images/heli.png COPYONLY)
configure_file(images/man.png images/man.png COPYONLY)
configure_file(images/man_walk.png images/man_walk.png COPYONLY)
configure_file(images/heli_full_size.png images/heli_full_size.png COPYONLY)
configure_file(images/heli_body.png images/heli_body.png COPYONLY)
configure_file(images/man_full_size.png images/man_full_size.png COPYONLY)
configure_file(images/rocket_full_size.png images/rocket_full_size.png COPYONLY)
configure_file(fonts/advanced-pixel-lcd-7/advanced_pixel_lcd-7.ttf fonts/advanced-pixel-lcd-7/advanced_pixel_lcd-7.ttf COPYONLY)
configure_file(sounds/bensound-moose.ogg sounds/bensound-moose.ogg COPYONLY)

# Define sources and executable
set(EXECUTABLE_NAME "Racing")

add_executable(${EXECUTABLE_NAME} main.cpp)

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()