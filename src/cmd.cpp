#include "include/cmd.h"

#include <iostream>
#include <sstream>

#include "include/formatting.h"

Cmd::Cmd(std::string in_cmd) : cmd(std::move(in_cmd)) {
    format::utf::deleteExtraSpaces(cmd);
}

bool Cmd::eof() const {
    return std::cin.eof();
}

size_t Cmd::countWords() const {
    size_t words_counter = 0;
    std::string_view cmd_view = cmd;
    while (!cmd_view.empty()) {
        auto word_begin = std::min(cmd_view.find_first_not_of(' ')
                , cmd_view.length());
        auto word_end = std::min(cmd_view.find_first_of(' ', word_begin)
                , cmd_view.length());
        words_counter += word_begin < word_end;
        cmd_view.remove_prefix(word_end);
    }
    return words_counter;
}

std::queue<std::string> Cmd::parseArgs(const std::string &arg_line) {
    std::queue<std::string> args;
    std::stringstream ss(arg_line);
    std::string buf;
    while (ss >> buf)
        args.push(buf);
    return args;
}

int Cmd::getCommand() {
    cmd.clear();
    while (!eof() && cmd.empty()) {
        printInvitation();
        std::getline(std::cin, cmd);
        format::utf::deleteExtraSpaces(cmd);
    }
    return eof();
}

int Cmd::trap(const std::string &msg,
               const std::vector<std::regex> &answers) {
    if (answers.empty())
        return -1;
    std::cout << msg << std::endl;
    int matched;
    while(!eof()) {
        getCommand();
        matched = match(answers);
        if (matched >= 0)
            return matched;
    }
    return -1;
}

void Cmd::printInvitation() {
    std::cout << "> ";
}

AnswerCode Cmd::ask(const std::string &what) {
    std::regex yes_pattern("^(y|Y)(es|ES|e|E|s|S)?$");
    std::regex no_pattern("^(n|N)(o|O)?$");
    std::cout << what << std::endl;
    std::string answer;
    std::getline(std::cin, answer);
    if (std::regex_match(answer, yes_pattern)
        && !std::regex_match(answer, no_pattern)) {
        return AnswerCode::Yes;
    }
    if (std::regex_match(answer, no_pattern)
        && !std::regex_match(answer, yes_pattern)) {
        return AnswerCode::No;
    }
    return AnswerCode::Unclear;
}

int Cmd::match(const std::vector<std::regex> &patterns) {
    if (cmd.empty() || patterns.empty())
        return -1;
    for (int i = 0; i < patterns.size(); ++i) {
        if (std::regex_match(cmd, patterns[i]))
            return i;
    }
        std::cout << "Unknown command. Try again" << std::endl;
    return -1;
}