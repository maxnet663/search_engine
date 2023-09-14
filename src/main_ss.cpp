#include <iostream>
#include <vector>

#include "include/screen_writer.h"

int main() {
    std::string input;
    custom::print_green("Prepare to run...");
    auto help_msg = "Input path (relative or absolute) to json files."
                    "Type:\n\"-d path/where/search/jsons\" or \"default\" "
                    "[-def] to find json files in current dir.\n"
                    "\"-p /path/to/config.json /path/to/requests.json\" "
                    "to set paths explicitly";
    std::cout << help_msg << std::endl;
    PRINT_INVITATION

    while(!std::getline(std::cin, input).eof()) {
        custom::deleteExtraSpaces(input);
        custom::toLowerCase(input);
        if (input.empty()) {
            custom::print_yellow("Arguments required");
            PRINT_INVITATION;
            continue;
        }
        auto args = ScreenWriter::commandParser(input);
        if ( (args.front() == "-d" && args.size() < 2)
        || (args.front() == "-p" && args.size() < 3)
        || (args.front() != "-d" && args.front() != "-p"
        && args.front() != "-def") ) {
            custom::print_yellow("Wrong input. Try again");
            PRINT_INVITATION
            continue;
        }
        try {
            ScreenWriter session(args);
            session();
        }
        catch (std::exception &ex) {
            custom::print_red(ex.what());
            std::cout << "Looks like something went wrong\n"
                         "Continue work?[y/n]:";
            std::getline(std::cin, input);
            custom::deleteExtraSpaces(input);
            custom::toLowerCase(input);
            if (input == "y" || input == "yes") {
                std::cout << help_msg << std::endl;
                PRINT_INVITATION
                continue;
            }
            else
                std::cin.setstate(std::ios_base::eofbit);
        }
    }
    return 0;
}
