add_executable(test_main test_main.cpp)

add_compile_definitions(TEST)

target_include_directories(
        test_main
        PUBLIC
        "${PROJECT_SOURCE_DIR}/src"
)

target_link_libraries(test_main PRIVATE gtest gtest_main nlohmann_json::nlohmann_json engine_lib)

target_compile_options(test_main PRIVATE -Wall -Wextra -pedantic -Werror)

gtest_discover_tests(test_main)
