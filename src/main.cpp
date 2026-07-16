#include "Application.hpp"
#include "ftxui/component/app.hpp"            // for Component, App
#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp" // for Slider, Checkbox, Vertical, Renderer, Button, Input, Menu, Radiobox, Toggle
#include "ftxui/dom/elements.hpp"
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>
void renderApp(Application& app) {
    using namespace ftxui;

    auto screen = ScreenInteractive::FitComponent();

    const std::vector<std::string> menu_entries = {
        "play", "step", "pause", "reset", "seek", "quit",
    };
    int menu_selected = -1;
    auto menu = Menu(&menu_entries, &menu_selected);

    auto component = Renderer(menu, [&] {
        return vbox({
                   text("Market Replay") | bold | center,
                   separator(),

                   hbox({
                       vbox({
                           text("Filename: " + app.getFilename()),
                           text("State: " + app.getStatus(screen)),
                           text("Progress") | bold,
                           gauge(app.getProgress(screen)) | flex | border,
                           text("Timestamp: " + app.getLastEventTimestamp(screen)),
                           separator(),
                           text("Last Event") | bold,
                           text(app.getLastEvent(screen)),
                       }) | flex,

                       separator(),

                       menu->Render() | size(WIDTH, EQUAL, 10),
                   }) | flex,
               }) |
               size(WIDTH, GREATER_THAN, 110) | size(HEIGHT, GREATER_THAN, 10) | border;
    });

    component = CatchEvent(component, [&](Event event) {
        if (event == Event::Return) {
            app.parse(menu_selected);
            return true;
        }
        return false;
    });

    screen.Loop(component);
}

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