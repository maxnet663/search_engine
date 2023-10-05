#ifndef CMD_H
#define CMD_H

#include <string>
#include <queue>
#include <regex>

#include "include/formatting.h"


/**
 * Simple wrapper over the standard string
 * Designed to store arguments and parse
 * strings into individual arguments
 */
class Cmd {

    std::string cmd;

public:

    /**
     * Make a string of args in C - style
     * @param argc amount of arguments
     * @param argv pointer to args
     */
    Cmd(int argc, char **argv);

    /**
     * Getter to storage
     * @return Stored string
     */
    std::string str() const & { return cmd; }

    /**
     * Check if storage is empty
     * @return true if stored string is empty, false otherwise
     */
    bool empty() const { return cmd.empty(); }

    /**
     * Make a queue of arguments from the specified string
     * @param arg_line string to parse
     * @return queue with separated words
     */
    static std::queue<std::string> parseArgs(const std::string &arg_line);

    /**
     * Make a queue of arguments from the stored string
     * @return queue with separated words
     */
    std::queue<std::string> parseArgs() const { return parseArgs(cmd); }
};

#endif //CMD_H
