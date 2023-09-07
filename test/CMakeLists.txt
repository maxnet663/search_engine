FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

FetchContent_MakeAvailable(googletest)

#disabling gtest library installation
set(INSTALL_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

set(gtest_force_shared_crt ON CACHE INTERNAL "")
set(gtest_disable_pthreads ON CACHE INTERNAL "")

#don't build test sources if main tests are built
set(BUILD_GMOCK OFF CACHE INTERNAL "")
set(BUILD_GTEST OFF CACHE INTERNAL "")

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

include(GoogleTest)

add_executable(test_main test_main.cpp)

add_compile_definitions(TEST -DTESTS_DIR="${PROJECT_SOURCE_DIR}/test")

target_include_directories(
        test_main
        PUBLIC
        "${PROJECT_SOURCE_DIR}/src"
)

target_link_libraries(test_main PRIVATE gtest gtest_main nlohmann_json::nlohmann_json engine_lib)

target_compile_options(test_main PRIVATE -Wall -Wextra -pedantic -Werror)

gtest_discover_tests(test_main)
