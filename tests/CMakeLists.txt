cmake_minimum_required(VERSION 2.8)

project(cpp-serialization)

set(SOURCES
    main.cpp
)

include_directories(../src)

add_executable(tests ${SOURCES})

