# Copyright 2018, GlobalLogic Inc.
# All rights reserved.
#

cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)

project(main)


add_executable(main
    main.cpp
)

target_include_directories(main PUBLIC
    pthread
)

