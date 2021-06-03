find_package(GTest CONFIG REQUIRED)

add_executable(tests 
    json_playground.cpp
    ServerPipelineTests.cpp)
    
target_link_libraries(tests 
    PRIVATE 
    GTest::gmock 
    GTest::gtest 
    GTest::gmock_main 
    GTest::gtest_main)
target_include_directories(tests 
    PRIVATE 
    ${Boost_INCLUDE_DIRS} 
    ${GTEST_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include)
    