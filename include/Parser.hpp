#include <string>
#include <fstream>
#include <iostream>
class Parser {
    public:
        explicit Parser(std::string filename) {
            std::ifstream file(filename);

            if (!file.is_open()) {
                // TODO: throw exception?
                std::cerr << "Cannot open file!";
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
