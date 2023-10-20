cmake_minimum_required(VERSION 3.10)
project(DCA1)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    include
    .
)

add_executable(DCA1
    main.cpp
    include/pbPlots.cpp
    include/supportLib.cpp
    )