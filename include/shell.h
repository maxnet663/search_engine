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
#include "include/formatting.h"

/**
 *  Recording the last modified time of a file
 */
typedef std::filesystem::file_time_type update_t;

/**
 * Smart pointer to ConverterJSON
 */
typedef std::unique_ptr<ConverterJSON> conv_ptr;

/**
 * invite the user to enter
 */
#define PRINT_INVITATION std::cout << "> ";

class ScreenWriter {
    conv_ptr pconverter;
    InvertedIndex document_base;
    SearchServer srv;

    std::string engine_name;
    std::string engine_version;
    std::vector<std::string> indexed_documents;
    std::string config_path;
    std::string requests_path;
    update_t last_changes_config;
    update_t last_changes_requests;

    std::map<std::string, void(ScreenWriter::*)()> commands;

public:

    ScreenWriter() : ScreenWriter(std::queue<std::string>()) {}

    explicit ScreenWriter(std::queue<std::string> args);

    void operator()();

    /**
     * Method constructs ConverterJSON
     * @tparam Args: variadic parameters
     * @param args: arguments will be passed to the constructor ConverterJSON
     * can be (), (string), (string, string)
     * @return unique_ptr to a ConverterJson
     */
    template<class... Args>
    static conv_ptr makeConverter(Args&&... args);

    /**
     * An alternative way to construct ConverterJSON
     * through dialogue with the user
     * @return unique_ptr to created ConverterJSON
     */
    static conv_ptr handMakeConverter();

    /**
     * Turns a string into a sequence of commands
     * @param cmd: string containing commands
     * @return queue containing commands
     */
    static std::queue<std::string> commandParser(const std::string &cmd);

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
    std::vector<std::string> makeAbsolute(std::vector<std::string> paths);

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
conv_ptr ScreenWriter::makeConverter(Args&&... args) {
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
