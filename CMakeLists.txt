cmake_minimum_required(VERSION 3.1)

project(web_vis_pipeline)

find_package(Boost)
find_package(Threads)

# add_executable(playground main.cpp)
# target_include_directories(
#     playground 
#     PUBLIC 
#     ${Boost_INCLUDE_DIRS} 
#     ${PROJECT_SOURCE_DIR}/include)
# target_link_libraries(
#     playground 
#     Threads::Threads)

add_subdirectory(tests)