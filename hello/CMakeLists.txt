project(MAIN)
cmake_minimum_required(VERSION 2.6)
aux_source_directory(. DIR_SRCS)
add_executable(main ${DIR_SRCS})
