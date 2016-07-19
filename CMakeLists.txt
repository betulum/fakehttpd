cmake_minimum_required(VERSION 2.8)

add_executable(final main.c arg.c worker.c) 
set_target_properties(final PROPERTIES COMPILE_FLAGS "-std=c11 -D_POSIX_SOURCE -D_DEFAULT_SOURCE" LINK_FLAGS "-lpthread -lev")
