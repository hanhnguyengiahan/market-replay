#include <iostream>
#include "Parser.hpp"
#include <format>

class CommandParser {
    public:
    CommandParser() = delete;
    CommandParser(std::string filename): parser{filename}, engine{parser.getLogs()} {};

    void parse() {
        std::cout << std::format("Loaded {} events.\n\n", parser.getLogsSize());
        std::string commandList = std::format("Commands:\n"
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
        std::string command;
        while(std::cin >> command) {
            if (command == "play") {
                engine.play();
            } else if (command == "reset") {
                engine.reset();
            } else if (command == "step") {
                engine.step();
            } else if (command == "seek") {
                // std::string timestamp;
                // std::cin >> timestamp;
                // engine.seek(timestamp);
            } else if (command == "reset") {
                engine.reset();
            } else if (command == "status") {
                // engine.status();
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

    std::vector<std::string> getLogs() {
        return parser.getLogs();
    }

    private:
        Parser parser;
        ReplayEngine engine;
};