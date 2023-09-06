#include "include/custom_functions.h"

#include <filesystem>
#include <fstream>
#include <list>
#include <iostream>

#include "include/project_constants.h"
#include "termcolor/termcolor.hpp"

std::string custom::getFileText(const std::string &file_name) {

    if (!std::filesystem::exists(file_name)) {
        throw std::filesystem::filesystem_error(
                "File does not exist"
                , file_name
                , std::make_error_code(std::errc::no_such_file_or_directory));
    }
    if (!isReadable(file_name)) {
        throw std::filesystem::filesystem_error(
                "Could not open the file"
                , file_name
                , std::make_error_code(std::errc::permission_denied));
    }

    // open stream for reading
    std::ifstream ifs(file_name);

    if (!ifs.is_open()) {
        throw std::filesystem::filesystem_error(
                "Could not open the file"
                , file_name
                , std::make_error_code(std::errc::bad_file_descriptor));
    }

    //check words number in file
    if (std::distance(std::istream_iterator<std::string>(ifs)
            , std::istream_iterator<std::string>()) > MAX_WORDS_NUMBER) {

        throw std::length_error("Number of words in file "
                                + file_name + " greater than "
                                + std::to_string(MAX_WORDS_NUMBER));
    }

    // back to the start of file
    ifs.clear();
    ifs.seekg(0);

    std::string buf; // buffer for word processing
    std::string text; //storage for text

    // reserve memory
    text.reserve(std::filesystem::file_size(file_name));

    while (ifs >> buf) {

        //check word's length
        if (buf.length() > MAX_WORD_LENGTH) {
            throw std::length_error("one of words from file:"
                                    + file_name
                                    + " has length greater than "
                                    + std::to_string(MAX_WORD_LENGTH));
        }

        text += buf + " ";
    }

    ifs.close();

    formatString(text);

    return text;
}

void custom::formatString(std::string &s)  {
    if (s.length() == 0) {
        return;
    }

    deletePunctuationMarks(s);

    toLowerCase(s);

    deleteExtraSpaces(s);
}

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

void custom::toLowerCase(std::string &s) {

    std::for_each(s.begin()
                  , s.end()
                  , [](char &ch){ ch = tolower(ch); });

}

void custom::deletePunctuationMarks(std::string &s) {
    s.erase(std::remove_if(s.begin()
                    , s.end()
                    , [](char &ch){return std::ispunct(ch);})
            , s.end());
}

void custom::deleteExtraSpaces(std::string &s) {

    // use two pointers
    auto back = s.begin();
    auto front = s.begin();

    // search first non space ch
    while (*front == ' ') {
        ++front;
    }

    while (*front) {

        // if met one space and prev ch not space
        // write the ch
        if (*front != ' ' || *(front - 1) != ' ') {
            *back++ = *front;
        }

        // move on the string
        front++;
    }

    // delete extra ch
    s.resize(back - s.begin());

    // if last ch is space delete it
    if (*(s.end() - 1) == ' ') {
        s.pop_back();
    }
}

void custom::writeJsonToFile(const nlohmann::json &json_text
                             , const std::string &path) {
    if (std::filesystem::exists(path)) {
        if (!isWriteable(path)) {
            throw std::filesystem::filesystem_error(
                    "Can not write to the file " + path
                    , path
                    , std::make_error_code(std::errc::permission_denied));
        }
    }

    std::ofstream dest(path, std::ios::out | std::ios::trunc);
    dest << std::setw(2) << json_text;
    dest.close();

}

std::string custom::getFileName(std::string s) {

    // pointers to the end of the string
    auto walk = s.end() - 1;

    // if its directory
    if (*walk == '/' || *walk == '\\') {
        walk--;
    }

    // search for the first slash
    while(*walk != '/' && *walk != '\\' && walk != s.begin()) {
        walk--;
    }

    s.erase(s.begin(), walk + 1);

    return s;
}

size_t custom::countOccurrences(const std::string &text, const std::string &word) {
    size_t walk = 0;
    size_t occur_counter = 0;

    // while we can detect occurrence
    while (walk <= text.length() - word.length()) {

        // get the index of occurrence's start
        auto occur_begin = text.find(word, walk);

        // if there is not any occurrence
        if (occur_begin == std::string::npos)
            return occur_counter;

        auto occur_end = occur_begin + word.length();

        // if the occurrence is a single word, not part of a word
        if ( (occur_begin == 0 || text[occur_begin - 1] == ' ')
             && (text[occur_end] == ' ' || occur_end == text.length()) ) {

            occur_counter++;
        }

        // move the current index
        walk = occur_end;
    }

    return occur_counter;
}

std::vector<std::string> custom::getUniqueWords(const std::string &text) {
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

bool custom::isReadable(const std::string &file_name) {
    std::filesystem::path path(file_name);
    if (!exists(path))
        return false;

    auto file_perms = std::filesystem::status(path).permissions();
    auto read_perms = std::filesystem::perms::owner_read;
    return std::filesystem::perms::none != (file_perms & read_perms);
}

bool custom::isWriteable(const std::string &file_name) {
    std::filesystem::path path(file_name);
    if (!exists(path))
        return false;

    auto file_perms = std::filesystem::status(path).permissions();
    auto write_perms = std::filesystem::perms::owner_write;
    return std::filesystem::perms::none != (file_perms & write_perms);
}

void custom::print_red(const std::string &msg) {
    std::cout << termcolor::red << msg << termcolor::reset << std::endl;
}

void custom::print_green(const std::string &msg) {
    std::cout << termcolor::green << msg << termcolor::reset << std::endl;
}

void custom::print_yellow(const std::string &msg) {
    std::cout << termcolor::yellow << msg << termcolor::reset << std::endl;
}

void custom::print_blue(const std::string &msg) {
    std::cout << termcolor::blue << msg << termcolor::reset << std::endl;
}
