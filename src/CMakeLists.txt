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

# connect ICU lib
FetchContent_Declare(
        icu
        GIT_REPOSITORY https://github.com/unicode-org/icu.git
        GIT_TAG        release-73-2
)

add_library(
        engine_lib
        converter_json.cpp
        custom_functions.cpp
        inverted_index.cpp
        search_server.cpp
        screen_writer.cpp
        file_reader.cpp
        formating.cpp
)

target_include_directories(engine_lib PUBLIC ${PROJECT_SOURCE_DIR} ${icu_SOURCE_DIR}/include)

target_link_libraries(engine_lib PUBLIC nlohmann_json::nlohmann_json termcolor icuuc icui18n)
