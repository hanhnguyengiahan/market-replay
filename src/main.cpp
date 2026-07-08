#include <iostream>
#include "Parser.hpp"
#include "ReplayEngine.hpp"
using namespace std;

int main() {
    Parser parser = Parser("../data/sample_events.csv");
    ReplayEngine replayEngine = ReplayEngine(parser.getLogs());
    replayEngine.play();
    return 0;
}