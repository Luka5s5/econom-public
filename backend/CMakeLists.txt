cmake_minimum_required(VERSION 3.10)
project(econom_igra)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

include_directories(.)

add_executable(econom_igra
        main.cpp
        Game.cpp
        War.cpp 
)
