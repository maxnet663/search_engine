#include <iostream>
#include <vector>

#include "include/screen_writer.h"


int main() {
    std::string path;
    std::cout << "Prepare to run...\n"
                 "Input path (relative or absolute) to jsons dir\n"
                 "or type \"default\" to find jsons dir in current path:\n> ";

    while(!std::getline(std::cin, path).eof()) {
        if (path == "default")
            path = (std::filesystem::current_path() / "jsons").string();
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
                             " to jsons dir\n> ";
                continue;
            }
            else
                std::cin.setstate(std::ios_base::eofbit);
        }
    }
    return 0;
}
