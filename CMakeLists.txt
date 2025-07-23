cmake_minimum_required(VERSION 4.0.2)

project(GbaEmuTests)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(src)   # Run CMakeLists.txt in src directory
add_subdirectory(tests) # Run CMakeLists.txt in tests directory