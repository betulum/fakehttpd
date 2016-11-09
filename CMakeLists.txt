cmake_minimum_required(VERSION 2.8)

add_executable(fakehttpd main.c arg.c worker.c) 
set_target_properties(fakehttpd PROPERTIES COMPILE_FLAGS "-w -std=c11 -D_POSIX_SOURCE")
target_link_libraries(fakehttpd pthread ev)
