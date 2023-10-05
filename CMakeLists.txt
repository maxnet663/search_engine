cmake_minimum_required(VERSION 3.21)

project (search_engine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

# add targets search_engine_session_simulator
add_executable(se ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_include_directories(se PUBLIC ${PROJECT_SOURCE_DIR})
target_link_libraries(se PRIVATE engine_lib)

#uncomment if installing without using a script
#set(CMAKE_INSTALL_PREFIX /path/to/install)

#setup instruction
install(TARGETS se DESTINATION .)
install(FILES
        install_files/config.json
        install_files/requests.json
        install_files/help.json
        DESTINATION
        .)
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


