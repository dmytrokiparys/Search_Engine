cmake_minimum_required (VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(SrcFiles
        ${MY_SRC_DIR}/InvertedIndex.cpp
        ${MY_SRC_DIR}/SearchServer.cpp
        ${MY_SRC_DIR}/ConverterJSON.cpp
        )
set(HeadFiles
        ../include/ConverterJSON.h
        ../include/SearchServer.h
        ../include/InvertedIndex.h
        )

include_directories(../include ../src )
include_directories(../nlohmann_json)

set(TestsFiles SearchEngine_test.cpp)

add_executable(GtestTests ${TestsFiles} ${SrcFiles} ${HeadFiles})

include(GoogleTest)

target_link_libraries(GtestTests PRIVATE nlohmann_json::nlohmann_json gtest gtest_main )

gtest_discover_tests(GtestTests)