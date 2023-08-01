cmake_minimum_required(VERSION 3.22)

project (search_engine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

set(JSON_BuildTests OFF CACHE INTERNAL "")

set(NLOHMANN_JSON_DIR "${PROJECT_SOURCE_DIR}/nlohmann_json")

add_subdirectory(nlohmann_json)

add_executable(search_engine ${PROJECT_SOURCE_DIR}/src/main.cpp)

add_subdirectory(src)

target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})

target_link_libraries(search_engine PRIVATE engine_lib nlohmann_json::nlohmann_json)



