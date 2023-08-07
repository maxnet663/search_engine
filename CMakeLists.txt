cmake_minimum_required(VERSION 3.26)

project (search_engine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(PROJECT_ROOT_PATH="${PROJECT_SOURCE_DIR}")

add_compile_definitions(PROJECT_JSONS_DIR="${PROJECT_SOURCE_DIR}/JSONs/")

add_compile_definitions(PROJECT_RESOURCES_DIR="${PROJECT_SOURCE_DIR}/resources/")

add_compile_definitions(TEST_JSONS_DIR="${PROJECT_SOURCE_DIR}/test/JSONs_test/")

add_compile_definitions(TEST_RESOURCES_DIR="${PROJECT_SOURCE_DIR}/test/resources_test/")

set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)

FetchContent_MakeAvailable(json)

add_executable(search_engine ${PROJECT_SOURCE_DIR}/src/main.cpp)

add_subdirectory(src)

target_include_directories(search_engine PUBLIC ${PROJECT_INCLUDE_DIR})

target_link_libraries(search_engine PRIVATE engine_lib)

FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

#set(gtest_disable_pthreads on)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

target_link_libraries(search_engine PRIVATE gtest_main)

include(GoogleTest)

gtest_discover_tests(search_engine)

add_subdirectory(test)

