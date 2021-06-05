cmake_minimum_required(VERSION 3.1)

project(web_vis_pipeline)

find_package(Boost)
find_package(Threads)

add_subdirectory(tests)
add_subdirectory(server)