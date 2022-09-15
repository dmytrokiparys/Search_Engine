cmake_minimum_required(VERSION 3.23)
project(search_engine)

set(JSON_BuildTests OFF CACHE INTERNAL "")


set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(nlohmann_json)
add_subdirectory(tests)



include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)


set(CMAKE_CXX_STANDARD 14)

set(gtest_disable_pthreads on)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(search_engine main.cpp include/ConverterJSON.h src/ConverterJSON.cpp include/InvertedIndex.h src/InvertedIndex.cpp include/SearchServer.h src/SearchServer.cpp include/Exceptions.h tests/SearchEngine_test.cpp)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(search_engine PRIVATE gtest_main)

include(GoogleTest)

gtest_discover_tests(search_engine)

