#ifndef SCREEN_WRITER_H
#define SCREEN_WRITER_H

#include <string>
#include <map>
#include <memory> // unique_ptr
#include <queue>
#include <iostream>

#include "include/converter_json.h"
#include "include/custom_functions.h"
#include "include/inverted_index.h"
#include "include/search_server.h"
#include "include/project_constants.h"
#include "include/project_types.h"
#include "include/formatting.h"

/**
 * invite the user to enter
 */
#define PRINT_INVITATION std::cout << "> ";

class ScreenWriter {
    ConverterPtr pconverter;
    InvertedIndex document_base;
    SearchServer srv;

    std::string engine_name;
    std::string engine_version;
    PathsList indexed_documents;
    PathType config_path;
    PathType requests_path;
    UpdatedTime last_changes_config;
    UpdatedTime last_changes_requests;

    std::map<std::string, void(ScreenWriter::*)()> commands;

public:

    ScreenWriter() : ScreenWriter(std::queue<std::string>()) {}

    explicit ScreenWriter(ArgsList args);

    void operator()();

    /**
     * Method constructs ConverterJSON
     * @tparam Args: variadic parameters
     * @param args: arguments will be passed to the constructor ConverterJSON
     * can be (), (string), (string, string)
     * @return unique_ptr to a ConverterJson
     */
    template<class... Args>
    static ConverterPtr makeConverter(Args&&... args);

    /**
     * An alternative way to construct ConverterJSON
     * through dialogue with the user
     * @return unique_ptr to created ConverterJSON
     */
    static ConverterPtr handMakeConverter();

    /**
     * Turns a string into a sequence of commands
     * @param cmd: string containing commands
     * @return queue containing commands
     */
    static ArgsList commandParser(const std::string &cmd);

private:

    /**
     * Command handler
     * @param cmd: format string representing a command
     */
    void handler(std::string &cmd);

    /**
     * The method checks whether the config.json has changed
     * since the last update or program launch;
     * if so, it updates the file and extracts new information from it
     */
    void updateDB();

    /**
     * The method checks whether the requests.json has changed
     * since the last update or program launch;
     * if so, it updates the file and extracts new information from it
     */
    void updateRequests();

    /**
     * Checks if config.json or requests.json have been overwritten
     * since last update or program launch
     * @return
     */
    bool checkUpdate();

    /**
     * Prints the command's list
     */
    void showHelp();

    /**
     * Prints information about the current session:
     * the name and version of the engine and the
     * current configuration files
     */
    void showStat();

    /**
     * Prints the request's list
     */
    void showRequests();

    /**
     * Prints the list of current indexed documents
     */
    void showIndexedDocs();

    /**
     * Make search with current requests in current indexed documents
     */
    void search();

    /**
     * If the method finds a answers.json, it will print its
     * contents, otherwise it will issue a warning
     */
    void showAnswers();

    /**
     * Helper method for showAnswers. Print contents
     * of answers
     * @param answers json object in answers form
     */
    void printAnswers(const json &answers);

    /**
     * The method replaces relative paths with absolute ones;
     * if a file at that path does not exist, the path is
     * deleted and a warning is printed
     * @param paths: path's list
     * @return list of absolute paths to existing files
     */
    PathsList makeAbsolute(PathsList paths);

    /**
     * The method sets the eof bit, preventing the resumption
     * of dialogue with the user
     */
    void exit();
};

/**
 * The method must be defined in the declaration file
 * because it requires instantiation
 */
template<class... Args>
ConverterPtr ScreenWriter::makeConverter(Args&&... args) {
        try {
            auto converter = std::make_unique<ConverterJSON>(
                    std::forward<Args>(args)...);
            return converter;
        }
        catch (std::exception &ex) {
            custom::print_yellow(ex.what());
            std::cout << "Failed to automatically detect json files.\n"
                         "Pay attention to warnings and correct errors.\n"
                         "Try to find the files manually?[y/n]:";
            std::string input;
            std::getline(std::cin, input);
            format::utf::deleteExtraSpaces(input);
            format::utf::toLowerCase(input);
            if (input == "y" || input == "yes") {
                std::cout << std::endl;
                return handMakeConverter();
            }
        }
    return nullptr;
}

#endif //SCREEN_WRITER_H
