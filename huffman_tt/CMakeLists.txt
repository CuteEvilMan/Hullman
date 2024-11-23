cmake_minimum_required(VERSION 3.29)
project(huffman_tt)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(.idea)

add_executable(huffman_tt
    .idea/.gitignore
    .idea/vcs.xml
    .idea/workspace.xml
    huffman.h
    main.c)
