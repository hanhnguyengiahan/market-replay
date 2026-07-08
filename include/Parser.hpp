#include <string>
#include <fstream>
#include <iostream>
class Parser {
    public:
        Parser(std::string filename) {
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
    private:
        std::vector<std::string> logs;
};
