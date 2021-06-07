find_package(Boost COMPONENTS thread)
find_package(Threads)

add_executable(server 
    main.cpp
    )
target_link_libraries(server Boost::thread)
target_include_directories( 
    server    
    PRIVATE 
    ${Boost_INCLUDE_DIRS} 
    ${PROJECT_SOURCE_DIR}/include)

set_property(TARGET server PROPERTY CXX_STANDARD 20)
    