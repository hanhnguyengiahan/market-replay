#include "Application.hpp"
#include "Renderer.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <data_file_name>" << "\n";
        return 1;
    }

    try {
        std::cout << "Loading " << argv[1] << "...\n";
        Application app = Application(argv[1]);
        std::cout << std::format("Loaded {} events.\n\n", app.getLogs().size());
        std::cout << "Launching replay..." << "\n";
        renderApp(app);
    } catch (std::exception& e) {
        std::cerr << "Exception thrown: " << e.what() << "\n";
    }

    return 0;
}