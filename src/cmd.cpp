#include "include/cmd.h"

#include <iostream>
#include <sstream>

#include "include/formatting.h"

Cmd::Cmd(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        cmd.append(argv[i]);
        if (i < argc - 1)
            cmd.append(" ");
    }
}

std::queue<std::string> Cmd::parseArgs(const std::string &arg_line) {
    std::queue<std::string> args;
    std::stringstream ss(arg_line);
    std::string buf;
    while (ss >> buf)
        args.push(buf);
    return args;
}