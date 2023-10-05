#include <iostream>

#include "include/shell.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Expected a command" << std::endl;
        return 0;
    }
    Shell shell(argc, argv);
    return shell();
}
