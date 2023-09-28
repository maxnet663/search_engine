cmake_minimum_required(VERSION 3.21)

project (search_engine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

# add targets search_engine_session_simulator
add_executable(search_engine_session_simulator ${PROJECT_SOURCE_DIR}/src/main_ss.cpp)
target_include_directories(search_engine_session_simulator PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(search_engine_session_simulator PRIVATE engine_lib)

# add targets search_engine
add_executable(search_engine ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_include_directories(search_engine PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(search_engine PRIVATE engine_lib)

#uncomment if installing without using a script
#set(CMAKE_INSTALL_PREFIX /path/to/install)

#setup instruction
install(TARGETS search_engine_session_simulator DESTINATION .)
install(TARGETS search_engine DESTINATION .)
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
        resources/arab_text.txt
        resources/chinese_text.txt
        resources/russian_text.txt
        resources/spanish_text.txt
        DESTINATION
        resources)

add_subdirectory(test)


