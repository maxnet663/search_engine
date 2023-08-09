add_library(
        engine_lib
        converter_json.cpp
        file_helper.cpp
        inverted_index.cpp
)

target_include_directories(engine_lib PUBLIC ${PROJECT_INCLUDE_DIR} ${NLOHMANN_JSON_DIR})

target_link_libraries(engine_lib PUBLIC nlohmann_json::nlohmann_json)
