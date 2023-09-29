#ifndef FILE_READER_H
#define FILE_READER_H

#include <filesystem>
#include <fstream>
#include <string>

#include "include/formatting.h"
#include "include/project_types.h"

/**
 * size of buffer for reading from file
 */
#define BUFFER_SIZE 50

/**
 * Wrapper over standard file reading stream
 */
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
     * Path getter
     * @return path to opened file
     */
    PathType getPath() const { return path; }

    /**
     * Extension getter
     * @return extension of opened file
     */
    std::string getExtension() const
    { return std::filesystem::path(path).extension(); }

    /**
     * Returns the contents of the file without formatting
     * @return text of the file
     */
    Text getText();

    /**
     * Returns file's text formatted as unicode:
     * deletes whitespaces, punctuation marks,
     * converts all letters to lowercase
     * @return utf-8 text
     */
    Text getFormattedText();

    /**
     * Checks permission to read from a file.
     * Only owner rights are checked
     * @param file_path: path to file
     * @return true if the file is readable by the owner
     * false otherwise
     */
    static bool isReadable(const std::string &file_path);

    /**
     * Checks permission to write to a file.
     * Only owner rights are checked
     * @param file_path: path to file
     * @return true if the file is writeable by the owner
     * false otherwise
     */
    static bool isWriteable(const std::string &file_path);

    bool operator>>(std::string &dest);
};

#endif //FILE_READER_H
