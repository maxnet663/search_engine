#include "file_helper.h"

#include <filesystem>
#include <fstream>
#include <algorithm>
#include <iterator>

#define MAX_WORDS_NUMBER 1000
#define MAX_WORD_LENGTH 100
#define MAX_REQUESTS_NUMBER 1000
#define MAX_REQUEST_WORDS_NUMBER 10

std::string FileHelper::getFileText(const std::string &filename) {
    std::ifstream ifs(filename);

    //check words number in file
    if (std::distance(std::istream_iterator<std::string>(ifs)
            , std::istream_iterator<std::string>()) > MAX_WORDS_NUMBER) {
        throw std::length_error(std::string("number of words in file ")
                                + filename  + std::string(" greater than ")
                                + std::to_string(MAX_WORDS_NUMBER));
    }

    // back to the start of file
    ifs.clear();
    ifs.seekg(0);

    std::string buf; // buffer for word processing
    std::string text;//storage for text
    text.reserve(std::filesystem::file_size(filename));
    while (ifs) { // read one word at a time
        ifs >> buf;

        //check word's length
        if (buf.length() > MAX_WORD_LENGTH) {
            throw std::length_error(std::string("one of words from file:")
                                    + filename
                                    + std::string( " has length greater than ")
                                    + std::to_string(MAX_WORD_LENGTH));
        }

        //put the word to the storage
        text += buf + " ";
    }
    ifs.close();
    text.pop_back(); // delete last space
    formatString(text);
    return text; // returns formated string
}

bool FileHelper::isFileExist(const std::string &path) {
    return std::filesystem::exists(path);
}

void FileHelper::formatString(std::string &s)  {
    if (s.length() == 0) {
        return;
    }
    //delete punctuation marks
    deletePunctuationMarks(s);

    //lowercase letters
    toLowerCase(s);

    //delete extra spaces
    deleteExtraSpaces(s);
}

int FileHelper::wordsCounter(const std::string &s) {
    bool is_word = false;
    int words_counter = 0;
    for (const auto &ch: s) {
        if (!isalnum(ch) && is_word) {
            words_counter++;
            is_word = false;
            continue;
        }
        if (isalnum(ch)) {
            is_word = true;
            continue;
        }
    }
    return words_counter + is_word;
}

void FileHelper::toLowerCase(std::string &s) {
    for (auto &ch : s) {
        if (isupper(ch)) {
            ch = tolower(ch);
        }
    }
}

void FileHelper::deletePunctuationMarks(std::string &s) {
    s.erase(std::remove_if(s.begin()
                    , s.end()
                    , [](char &ch){
                        return std::ispunct(ch); })
            , s.end());
}

void FileHelper::deleteExtraSpaces(std::string &s) {
    auto back = s.begin();
    auto front = s.begin();
    while (*front == ' ') {
        ++front;
    };
    while (*front) {
        if (*front != ' ' || *(front - 1) != ' ') {
            *back++ = *front;
        }
        front++;
    }
    *back = 0;
    s.resize(back - s.begin());
    if (*(s.end() -1) == ' ')
        s.pop_back();
}

void FileHelper::writeJsonToFile(const nlohmann::json &jsonText, const std::string &path) {
    if (isFileExist(path)) {
        remove(path.data());
    }
    std::ofstream dest(path, std::ios::out);
    dest << jsonText;
    dest.close();
}
