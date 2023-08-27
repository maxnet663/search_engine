add_library(
        engine_lib
        converter_json.cpp
        custom_functions.cpp
        inverted_index.cpp
        search_server.cpp
)

target_include_directories(engine_lib PUBLIC ${PROJECT_SOURCE_DIR} ${NLOHMANN_JSON_DIR})

target_link_libraries(engine_lib PUBLIC nlohmann_json::nlohmann_json)
