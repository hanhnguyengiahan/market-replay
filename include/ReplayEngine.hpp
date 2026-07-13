#include "MarketEvent.hpp"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <ranges>
#include <sstream>
#include <string_view>
#include <thread>
#include <vector>

enum class EventParams { Timestamp = 0, Id, Type, Symbol, OrderId, Side, Price, Quantity };

const int SECONDS_TO_DELAY = 1;

class ReplayEngine {
  public:
    ReplayEngine(std::vector<std::string> logs);
    ~ReplayEngine() {
        replayThread_.request_stop(); // request for engine thread's termination
        cv.notify_one();      // wake the engine thread up if it's waiting for incoming commands or
                              // termination request
        replayThread_.join(); // make the main thread waits for the engine thread to finish
                              // termination before destructing mutexes
    };
    void executePlay();
    void play();
    void executeStep(int numSteps);
    void step(int numSteps);
    void pause();
    void reset();
    void seek(timestamp_t timestampToSeek);
    void status();

  private:
    void keepAlive(std::stop_token st);

    std::vector<MarketEvent> events_;
    size_t currentEvent_;
    bool isPaused_;
    std::jthread replayThread_;
    int numSteps_;

    std::mutex m;
    std::condition_variable_any cv;
    bool playing = false;
    bool stepping = false;
    bool paused = false;
};