#include "Application.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    using namespace ftxui;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <data_file_name>" << "\n";
        return 1;
    }

    try {
        std::cout << "Loading " << argv[1] << "...\n";
        Application app = Application(argv[1]);
        std::cout << std::format("Loaded {} events.\n\n", app.getLogs().size());
        std::cout << "Launching replay..." << "\n";
        // app.parse();
    } catch (std::exception& e) {
        std::cerr << "Exception thrown: " << e.what() << "\n";
    }

    // Define the document structure
    Element document = vbox({
        text("Market Replay") | bold | center,
        separator(),
        hbox({
            text("filename") | border,
            vbox({
                text("Command Area") | flex,
                separator(),
                text("Footer Information") | dim,
            }) | border |
                flex,
        }) | flex,
    });

    // Create the screen and render
    auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
    Render(screen, document);
    screen.Print();

    return 0;
}