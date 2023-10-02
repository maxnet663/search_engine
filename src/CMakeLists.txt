include(FetchContent)

# connect nlohmann json for modern c++
FetchContent_Declare(
        json
        URL
        https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# connect termcolor for colored printing
FetchContent_Declare(
        termcolor
        URL
        https://github.com/ikalnytskyi/termcolor/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(termcolor)

add_library(
        engine_lib
        converter_json.cpp
        custom_functions.cpp
        inverted_index.cpp
        search_server.cpp
        shell.cpp
        file_reader.cpp
        formatting.cpp
        cmd.cpp
)

target_include_directories(engine_lib PUBLIC ${PROJECT_SOURCE_DIR} ${icu_SOURCE_DIR}/include)

target_link_libraries(engine_lib PUBLIC nlohmann_json::nlohmann_json termcolor icuuc icui18n)
