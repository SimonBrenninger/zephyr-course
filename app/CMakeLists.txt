cmake_minimum_required(VERSION 3.20.0)

# append custom board dir
list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
# append custom module
list(APPEND EXTRA_ZEPHYR_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/our-driver-module)

find_package(Zephyr REQUIRED)
project(my_zephyr_app)

target_sources(app PRIVATE src/main.cpp)

target_include_directories(app PRIVATE our-driver-module/drivers)
