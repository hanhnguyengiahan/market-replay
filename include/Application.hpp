#include "ReplayEngine.hpp"
#include "Parser.hpp"
#include <iostream>
#include <format>
#include <thread>

const int DEFAULT_NUM_STEP = 1;

class Application {
    public:
    Application(std::string filename);
    
    void parse();

    std::vector<std::string> getLogs();

    private:
        Parser parser;
        ReplayEngine engine;
};