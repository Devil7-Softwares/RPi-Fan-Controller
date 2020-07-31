cmake_minimum_required(VERSION 3.0.0)
project (RPi-Fan-Controller VERSION 0.0.1)

file(GLOB_RECURSE FC_SOURCES "src/*.cpp")

include_directories("src/include")
add_executable(fancontrold ${FC_SOURCES})