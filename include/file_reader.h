#ifndef FILE_READER_H
#define FILE_READER_H

#include <filesystem>
#include <fstream>
#include <string>
#include <mutex>

#include "include/formatting.h"

/**
 * Wrapper over standard file reading stream
 * with formatting extension for unicode strings
 */
class FileReader {

    std::string path;
    std::ifstream read_stream;
    std::mutex read_access;

public:

    FileReader() = delete;

    FileReader(const FileReader &other) = delete;

    FileReader &operator=(const FileReader &right) = delete;

    explicit FileReader(const std::string &file_path)
    : path(file_path), read_stream(file_path) {}

    /**
     * Opens new filestream, if there is already an open fstream,
     * it will be closed
     * @param file_path: path to file
     */
    void open(const std::string &file_path);

    /**
     * Checks if fstream opened
     * @return true if fstream opened, false otherwise
     */
    bool is_open() const { return read_stream.is_open(); }

    /**
     * Returns the contents of the file without formatting
     * @return text of the file
     */
    std::string getText();

    /**
     * Returns file's text formatted as unicode:
     * deletes whitespaces, punctuation marks,
     * converts all letters to lowercase
     * @return utf-8 text
     */
    std::string getFormattedText();

    /**
     * Checks permission to read from a file.
     * Only owner rights are checked
     * if the program is not launched by the
     * owner of the file, the result is incorrect
     * @param file_path: path to file
     * @return true if the file is readable by the owner,
     * false otherwise
     */
    static bool isReadable(const std::string &file_path);

    /**
     * Checks permission to write to a file.
     * Only owner rights are checked
     * if the program is not launched by the
     * owner of the file, the result is incorrect
     * @param file_path: path to file
     * @return true if the file is writeable by the owner,
     * false otherwise
     */
    static bool isWriteable(const std::string &file_path);

    /**
     * Extracts values from stored stream.
     * String formatted as unicode chars
     * @param dest where to extract
     * @return true if stream is not empty, false otherwise
     */
    bool operator>>(std::string &dest);
};

#endif //FILE_READER_H
