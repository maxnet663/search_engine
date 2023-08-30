#include <iostream>
#include <vector>

#include "include/converter_json.h"
#include "include/inverted_index.h"
#include "include/search_server.h"
#include "include/screen_writer.h"


int main() {
    std::string path;
    std::cout << "Prepare to run...\n";
    std::cout << "Input path (relative or absolute) to JSONs dir\n";
    std::cout << "or type \"default\" to find JSONs dir in current path:\n> ";
    std::getline(std::cin, path);

    if (std::cin.eof())
        return 1;

    if (path == "default")
        path = std::filesystem::current_path() / "JSONs";

    ScreenWriter session(path);
    session.startSession();
    return 0;
}
