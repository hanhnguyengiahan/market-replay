#pragma once
#include "Parser.hpp"
#include "ReplayEngine.hpp"
#include "ftxui/component/app.hpp"
#include <format>
#include <iostream>
#include <thread>

const int DEFAULT_NUM_STEP = 1;

enum Command { PLAY = 0, STEP, PAUSE, RESET, SEEK, QUIT };

class Application {
  public:
    Application(std::string filename);
    void parse(int command);

    std::vector<std::string> getLogs();
    std::string getFilename();
    double getProgress();
    std::string getLastEvent();
    std::string getLastEventTimestamp();
    std::string getStatus();

  private:
    Parser parser;
    ReplayEngine engine;
    std::string filename;
};