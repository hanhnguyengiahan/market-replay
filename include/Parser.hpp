#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

class Parser {
    public:
        explicit Parser(std::string filename) {
            std::ifstream file(filename);

            if (!file.is_open()) {
                throw std::invalid_argument(std::format("Cannot open the file with this name: {}", filename));
            }
            
            std::string line;
            while (getline(file, line)) {
                logs.push_back(line);
            }
        }

        std::vector<std::string> getLogs() {
            return logs;
        }

        size_t getLogsSize() {
            return logs.size();
        }
    private:
        std::vector<std::string> logs;
};
