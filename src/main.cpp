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

    while(!std::getline(std::cin, path).eof()) {
        if (path == "default")
            path = std::filesystem::current_path() / "JSONs";
        try {
            ScreenWriter session(path);
            session.startSession();
        }
        catch (std::exception &ex) {
            std::cout << ex.what() << std::endl;
            std::cout << "Try again [y/n]: ";
            std::getline(std::cin, path);
            if (path == "y") {
                std::cout << "Input path (relative or absolute)"
                << " to JSONs dir\n> ";
                continue;
            }
            else
                std::cin.setstate(std::ios_base::eofbit);
        }
    }
    return 0;
}
