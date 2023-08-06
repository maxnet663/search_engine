add_executable(test_main test_main.cpp)

target_compile_definitions(test_main PUBLIC TEST)

target_include_directories(test_main
        PUBLIC
        "${PROJECT_SOURCE_DIR}/src"
        ${PROJECT_INCLUDE_DIR}
        "${PROJECT_SOURCE_DIR}/nlohmann_json"
)

target_link_libraries(test_main PRIVATE gtest gtest_main nlohmann_json::nlohmann_json engine_lib)

target_compile_options(test_main PRIVATE -Wall -Wextra -pedantic -Werror)