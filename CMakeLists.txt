cmake_minimum_required(VERSION 3.21)

project (search_engine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        json
        URL
        https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)

FetchContent_MakeAvailable(json)

add_subdirectory(src)

add_executable(search_engine ${PROJECT_SOURCE_DIR}/src/main.cpp)

target_include_directories(search_engine PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(search_engine PRIVATE engine_lib)

add_executable(search_engine_simple ${PROJECT_SOURCE_DIR}/src/main_simple.cpp)

target_include_directories(search_engine_simple PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(search_engine_simple PRIVATE engine_lib)

install(TARGETS search_engine DESTINATION .)
install(TARGETS search_engine_simple DESTINATION .)
install(FILES
        install_files/config.json
        install_files/requests.json
        DESTINATION
        jsons)
install(FILES
        resources/file001.txt
        resources/file002.txt
        resources/file003.txt
        resources/file004.txt
        DESTINATION
        resources)

FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

FetchContent_MakeAvailable(googletest)

set(INSTALL_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

set(gtest_force_shared_crt ON CACHE INTERNAL "")
set(gtest_disable_pthreads ON CACHE INTERNAL "")
set(BUILD_GMOCK OFF CACHE INTERNAL "")
set(BUILD_GTEST OFF CACHE INTERNAL "")

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

include(GoogleTest)

add_subdirectory(test)


