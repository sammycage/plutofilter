cmake_minimum_required(VERSION 3.14)
project(plutofilter LANGUAGES C)

add_library(plutofilter INTERFACE)
target_include_directories(plutofilter INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
