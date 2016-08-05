cmake_minimum_required(VERSION 2.8)

add_executable(final main.c arg.c worker.c) 
set_target_properties(final PROPERTIES COMPILE_FLAGS "-w -std=c11 -D_POSIX_SOURCE")
target_link_libraries(final pthread ev)
