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

class Shell;

namespace fs = std::filesystem;

/**
 * Pointer to a ConverterJSON
 */
typedef std::unique_ptr<ConverterJSON> conv_ptr;

/**
 * Arguments for shell commands
 */
typedef std::queue<std::string> args_t;

/**
 * Wrapper over shell commands
 */
typedef std::function<int(Shell*, std::queue<std::string>&) > cmd_t;

/**
 * The shell provides an interface
 * for interacting with program capabilities
 */
class Shell {
    Cmd cmd; // arguments handler
    std::vector<std::pair<std::regex, cmd_t>> commands; // list of commands
    json helper; // file with help information
public:

    Shell(int argc, char **argv);

    /**
     * Launches a shell
     * @return 0 if everything is okay, code <> 0 otherwise
     */
    int operator()();

private:

    /**
     * Prints the command's list
     */
    int showHelp(std::queue<std::string> &args);

    /**
     * Print out help about a specific command
     * or similar information about all
     * @param command command to get info, if not specified,
     * prints info about all commands
     * @return 0 if everything is okay, code <> 0 otherwise
     */
    int help(const std::string& command = "");

    /**
     * Generates default json files
     * @param args arguments
     * @return 0 if everything is okay, code <> 0 otherwise
     */
    int generateJsons(args_t &args);

    /**
     * Helper command for generateJson
     * @param config_name
     * @param requests_name
     * @param dest_dir
     * @return 0 if everything is okay, code <> 0 otherwise
     */
    int generate(const std::string& config_name
                 , const std::string &requests_name
                 , const std::string &dest_dir);

     /**
      * Print out information about this programm
      * @param args arguments
      * @return 0 if everything is okay, code <> 0 otherwise
      */
    int showConfig(std::queue<std::string> &args);

    /**
     * Helper method for showConfig
     * Print out general info
     * @param config
     */
    void printInfo(const json &config);

    /**
     * Helper method for showConfig.
     * Check indexing docs
     * @param paths
     */
    void printIndexingPath(const std::vector<std::string> &path);

    /**
     * Make search in indexed docs config.json upon
     * request from requests.json
     * @param args arguments
     * @return 0 if everything is okay, code <> 0 otherwise
     */
    int find(args_t &args);

    /**
     * Helper method for find
     * @param config_path
     * @param requests_path
     * @param explicit_flag
     * @param recursive_flag
     * @return 0 if everything is okay, code <> 0 otherwise
     */
    int makeSearch(std::string &config_path
                   , std::string &requests_path
                   , bool explicit_flag
                   , bool recursive_flag);

    int printAnswers();

    void printWithFormatting(const json &answers);

    /**
     * Writes .txt files from specified dir in config.json
     * @param args arguments
     * @return 0 if everything is okay, code <> 0 otherwise
     */
    int index(std::queue<std::string> &args);

    /**
     * Helper method for index
     * @param dir
     * @param config_json
     * @return 0 if everything is okay, code <> 0 otherwise
     */
    int addFiles(const std::string &dir, json &config_json);

    /**
     * Print out specified json file
     * @param args arguments
     * @return 0 if everything is okay, code <> 0 otherwise
     */
    int printJson(std::queue<std::string> &args);

    /**
     * Method for extracting arguments from args list
     * @param args queue of args
     * @return front of queue if !args.empty(),
     * empty string otherwise
     */
    std::string getNext(args_t &args);

    /**
     * Method constructs ConverterJSON
     * @tparam Args: variadic parameters
     * @param args: arguments will be passed to the constructor ConverterJSON
     * can be (), (string), (string, string)
     * @return unique_ptr to a ConverterJson
     */
    template<class... Args>
    static conv_ptr makeConverter(Args&&... args);
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
