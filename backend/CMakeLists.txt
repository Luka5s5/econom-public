cmake_minimum_required(VERSION 3.17)
project(backend)

set(CMAKE_CXX_STANDARD 14)

add_executable(backend
        main.cpp
        Game.cpp
        War.cpp
        Player.cpp
        City.cpp)