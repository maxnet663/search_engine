#ifndef SHELL_H
#define SHELL_H

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
#include "include/cmd.h"

namespace fs = std::filesystem;

typedef std::unique_ptr<ConverterJSON> conv_ptr;

/**
 *  Recording the last modified time of a file
 */
typedef std::filesystem::file_time_type update_t;

enum class Status {
    Initialized,
    Uninitialized,
    Exit
};

class Shell {
    Status status;
    Cmd cmd;
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

    std::vector<std::pair<std::string, void(Shell::*)()>> commands;
    std::vector<std::regex> commands_patterns;

    friend class Cmd;

public:

    Shell();

    int operator()();

private:

    void generateJsons();

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
    conv_ptr handMakeConverter();

    /**
     * constructs ConverterJSON
     * @return 0 if everything is fine, val <> 0 otherwise
     */
    int initialize();

    /**
     * Command handler
     * @param command: format string representing a command
     */
    int handler(Cmd &command);

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
    void find();

    /**
     * If the method finds a answers.json, it will printWithFormatting its
     * contents, otherwise it will issue a warning
     */
    void printAnswers();

    /**
     * Helper method for printWithFormatting. Print contents
     * of answers
     * @param answers json object in answers form
     */
    void printWithFormatting(const json &answers);

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
conv_ptr Shell::makeConverter(Args&&... args) {
        try {
            auto converter = std::make_unique<ConverterJSON>(
                    std::forward<Args>(args)...);
            return converter;
        }
        catch (std::exception &ex) {
            custom::print_yellow(ex.what());
        }
    return nullptr;
}

#endif //SHELL_H
