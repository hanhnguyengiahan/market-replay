#include "Application.hpp"

Application::Application(std::string filename)
    try
        : parser{filename},
        engine{parser.getLogs()}
    {
    }
    catch (const std::invalid_argument& e) {
        throw;
    }

void Application::parse() {
    std::cout << std::format("Loaded {} events.\n\n", parser.getLogsSize());
    std::string commandList = std::format(
                            "Commands:\n"
                            "  play\n"
                            "  pause\n"
                            "  step\n"
                            "  reset\n"
                            "  speed <x>\n"
                            "  seek <timestamp>\n" 
                            "  status\n"
                            "  quit\n\n"
                            "> "
                        );
    std::cout << commandList;
    std::string line;
    while(std::getline(std::cin, line)) {
        std::istringstream iss(line); 
        std::string command;
        iss >> command;
        if (command == "play") {
            engine.play();
        } else if (command == "reset") {
            engine.reset();
        } else if (command == "step") {
            int numSteps{DEFAULT_NUM_STEP};
            iss >> numSteps;
            engine.step(numSteps);
        } else if (command == "seek") {
            timestamp_t timestamp;
            iss >> timestamp;
            if (timestamp) {
                engine.seek(timestamp);
            }
        } else if (command == "reset") {
            engine.reset();
        } else if (command == "status") {
            // engine.status();
        } else if (command == "pause") {
            engine.pause();
        } else if (command == "quit") {
            break;
        } else {
            std::cout << "Usage: select one command out of this list:" << "\n";
            std::cout << commandList;
            continue;
        }
        std::cout << "> ";
    }
}

std::vector<std::string> Application::getLogs() {
    return parser.getLogs();
}
