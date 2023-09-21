#ifndef FILE_READER_H
#define FILE_READER_H

#include <filesystem>
#include <fstream>
#include <string>

#include "include/formatting.h"

/**
 * size of buffer for reading from file
 */
#define BUFFER_SIZE

class FileReader {

    std::string path;
    std::ifstream read_stream;

public:

    FileReader() = delete;

    FileReader(const FileReader &other) = delete;

    FileReader &operator=(const FileReader &right) = delete;

    explicit FileReader(const std::string &file_path) noexcept
    : path(file_path), read_stream(file_path) {}

    /**
     * opens new filestream, if there is already an open fstream,
     * it will be closed
     * @param file_path path to file
     */
    void open(const std::string &file_path);

    /**
     * checks if fstream opened
     * @return true if fstream opened, false otherwise
     */
    bool is_open() const { return read_stream.is_open(); }

    /**
     * path getter
     * @return path to opened file
     */
    std::string getPath() const { return path; }

    /**
     * extension getter
     * @return extension of opened file
     */
    std::string getExtension() const
    { return std::filesystem::path(path).extension(); }

    /**
     * returns the contents of the file without formatting
     * @return text of the file
     */
    std::string getText();

    /**
     * returns file's text formatted as unicode:
     * deletes whitespaces, punctuation marks,
     * converts all letters to lowercase
     * @return utf-8 text
     */
    std::string getFormattedText();
};

#endif //FILE_READER_H
