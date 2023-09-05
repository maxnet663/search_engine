#include <iostream>
#include <vector>

#include "include/screen_writer.h"

int main() {
    std::string path;
    custom::print_green("Prepare to run...");
    std::cout << "Input path (relative or absolute) to jsons dir\n"
                 "or type \"default\" to find json files in current path:\n> ";

    while(!std::getline(std::cin, path).eof()) {
        if (path == "default")
            path = std::filesystem::current_path().string();
        try {
            ScreenWriter session(path);
            session.startSession();
        }
        catch (std::exception &ex) {
            custom::print_red(ex.what());
            std::cout << "\nLooks like something went wrong\nTry again [y/n]:";
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
