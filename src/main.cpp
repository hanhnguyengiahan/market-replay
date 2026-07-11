#include "Application.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <data_file_name>" << "\n";
        return 1;
    }

    try {
        Application app = Application(argv[1]);
        app.parse();
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }
    return 0;
}