#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

class Parser {
    public:
        explicit Parser(std::string filename);
        std::vector<std::string> getLogs();
        size_t getLogsSize();
    private:
        std::vector<std::string> logs;
};
