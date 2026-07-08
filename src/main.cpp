#include <iostream>
#include "ReplayEngine.hpp"
#include "CommandParser.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <data_file_name>" << "\n";
        return 1;
    }
    CommandParser commandParser = CommandParser(argv[1]);
    commandParser.parse();
    return 0;
}