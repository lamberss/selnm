cmake_minimum_required(VERSION 3.14)

project(selnm CXX)
include(GNUInstallDirs)

add_subdirectory(extern)
enable_testing()

add_subdirectory(libselnm)