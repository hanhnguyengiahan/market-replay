#include "Application.hpp"
#include <ftxui/component/event.hpp>

Application::Application(std::string filename) try
    : parser{filename}, filename{filename}, engine{parser.getLogs()} {
} catch (const std::invalid_argument& e) {
    throw;
}

std::string Application::getFilename() {
    return filename;
}

void Application::parse(int command) {
    if (command == Command::PLAY) {
        engine.play();
    } else if (command == Command::RESET) {
        engine.reset();
    } else if (command == Command::STEP) {
        int numSteps{DEFAULT_NUM_STEP};
        engine.step(numSteps);
    } else if (command == Command::SEEK) {
        timestamp_t timestamp;
        if (timestamp) {
            engine.seek(timestamp);
        }
    } else if (command == Command::STATUS) {
        // engine.status();
    } else if (command == Command::PAUSE) {
        engine.pause();
    } else if (command == Command::QUIT) {

    } else {
    }
}

double Application::getProgress(ftxui::App& screen) {
    screen.PostEvent(ftxui::Event::Custom);
    return engine.getProgress();
}

std::string Application::getLastEvent(ftxui::App& screen) {
    screen.PostEvent(ftxui::Event::Custom);
    return engine.getLastEvent();
}

std::vector<std::string> Application::getLogs() {
    return parser.getLogs();
}
