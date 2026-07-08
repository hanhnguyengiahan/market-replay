#include <iostream>
#include "ReplayEngine.hpp"
#include "Application.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <data_file_name>" << "\n";
        return 1;
    }
    Application app = Application(argv[1]);
    app.parse();
    return 0;
}