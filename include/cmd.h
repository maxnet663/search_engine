#ifndef CMD_H
#define CMD_H

#include <string>
#include <queue>
#include <regex>

#include "include/formatting.h"

enum class AnswerCode {
    Unclear = -1,
    No = 0,
    Yes = 1
};
class Shell;
class Cmd {
    std::string cmd;
public:

    Cmd() = default;

    Cmd(std::string in_cmd);

    Cmd(const Cmd &other) = default;

    explicit operator std::string() const { return cmd; }

    void set(std::string value) { cmd = std::move(value); }

    std::string str() const { return cmd; }

    bool empty() const { return cmd.empty(); }

    bool eof() const;

    size_t countWords() const;

    static std::queue<std::string> parseArgs(const std::string &arg_line);

    std::queue<std::string> parseArgs() const { return parseArgs(cmd); }

    int getCommand();

    int trap(const std::string &msg, const std::vector<std::regex> &answers);

    static void printInvitation();

    static AnswerCode ask(const std::string &what);

private:

    int match(const std::vector<std::regex> &patterns);

};

#endif //CMD_H
