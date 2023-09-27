#ifndef PROJECT_TYPES_H
#define PROJECT_TYPES_H

#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <queue>
#include <memory>

#include "nlohmann/json.hpp"

class ConverterJSON;
struct DocRelevance;
struct RelativeIndex;

/**
 * data structure for frequency dictionary
 */
typedef
std::unordered_map<std::string, std::unordered_map<size_t, size_t>> DictionaryType;

/**
 * type of text representation extracted from text files
 */
typedef std::string Text;

/**
 * Texts List representation
 */
typedef std::vector<std::string> TextsList;

/**
 * The list of paths to files
 */
typedef std::vector<std::string> PathsList;

/**
 *  The representation of words frequency
 *  std::unordered_map<doc_id, occurrences>
 */
typedef std::unordered_map<size_t, size_t> Frequency;

/**
 * Representing a path to a file
 */
typedef std::string PathType;

/**
 *  Recording the last modified time of a file
 */
typedef std::filesystem::file_time_type UpdatedTime;

/**
 *  List of arguments of standard input
 */
typedef std::queue<std::string> ArgsList;

/**
 * Smart pointer to ConverterJSON
 */
typedef std::unique_ptr<ConverterJSON> ConverterPtr;

/**
 * List of requests
 */
typedef std::vector<std::string> RequestsList;

/**
 *  List of docs with their relevance
 *  DocRelevance { doc_id, doc_relevance }
 */
typedef std::vector<DocRelevance> RelevantDocs;

/**
 * List of the structures which represents answers to requests.
 * RelativeIndex { doc_id, relevance }
 */
typedef std::vector<RelativeIndex> Answer;

/**
 * List of Answers.
 * Uses by class SearchServer for generating responses
 */
typedef std::vector<Answer> AnswersLists;

/**
 * Represents json object
 */
typedef nlohmann::json json;

#endif //PROJECT_TYPES_H
