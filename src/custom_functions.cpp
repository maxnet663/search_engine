#include "include/custom_functions.h"

#include <cstdint> // uint8_t
#include <list>
#include <iostream>

#include "termcolor/termcolor.hpp"

size_t custom::wordsCounter(const std::string &s) {
    bool is_word = false;
    int words_counter = 0;

    // go through all the string
    for (const auto &ch: s) {

        // if letters and numbers run out and we met space
        if (!isalnum(ch) && is_word) {
            words_counter++;
            is_word = false;
            continue;
        }

        // if its letter or digit
        if (isalnum(ch)) {
            is_word = true;
            continue;
        }
    }

    // if the string ends with a word
    return words_counter + is_word;
}

std::vector<std::string> custom::getUniqueWords(const std::string &text) {
    if (text.empty()) return { };
    std::list<std::string> unique_words;
    std::stringstream data(text);
    std::string buf;
    while (data >> buf) {
        unique_words.push_back(buf);
    }
    return { unique_words.begin(), unique_words.end() };
}

double custom::round(double num, int precision) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(precision) << num;
    return std::stod(os.str());
}

void custom::print_red(const std::string &msg, bool new_line) {
    std::cout << termcolor::red << msg << termcolor::reset;
    if (new_line) std::cout << std::endl;
}

void custom::print_green(const std::string &msg, bool new_line) {
    std::cout << termcolor::green << msg << termcolor::reset;
    if (new_line) std::cout << std::endl;
}

void custom::print_yellow(const std::string &msg, bool new_line) {
    std::cout << termcolor::yellow << msg << termcolor::reset;
    if (new_line) std::cout << std::endl;
}

void custom::print_blue(const std::string &msg, bool new_line) {
    std::cout << termcolor::blue << msg << termcolor::reset;
    if (new_line) std::cout << std::endl;
}

void custom::print_magenta(const std::string &msg, bool new_line) {
    std::cout << termcolor::magenta << msg << termcolor::reset;
    if (new_line) std::cout << std::endl;
}

void custom::print_cyan(const std::string &msg, bool new_line) {
    std::cout << termcolor::cyan << msg << termcolor::reset;
    if (new_line) std::cout << std::endl;
}
