#ifndef PROJECT_LIMITS_H
#define PROJECT_LIMITS_H

//Constants
#define MAX_WORDS_NUMBER 1000
#define MAX_WORD_LENGTH 100
#define MAX_REQUESTS_NUMBER 1000
#define MAX_REQUEST_WORDS_NUMBER 10

#define CONFIG_FILE_NAME "config.json"
#define REQUESTS_FILE_NAME "requests.json"
#define RESULTS_FILE_NAME "answers.json"


// select paths to directories depending on the current build target
#ifdef TEST
#define JSONS_DIR TEST_JSONS_DIR
#define RESOURCES_DIR TEST_RESOURCES_DIR
#else
#define JSONS_DIR PROJECT_JSONS_DIR
#define RESOURCES_DIR PROJECT_RESOURCES_DIR
#endif

#endif //PROJECT_LIMITS_H
