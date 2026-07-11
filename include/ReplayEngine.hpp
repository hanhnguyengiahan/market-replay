#include "Event.hpp"
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

enum class EventParams {
  Timestamp = 0,
  Id,
  Type,
  Symbol,
  OrderId,
  Side,
  Price,
  Quantity
};

const int SECONDS_TO_DELAY = 1;

class ReplayEngine {
public:
  ReplayEngine(std::vector<std::string> logs);
  void keepAlive();
  void executePlay();
  void play();
  void executeStep(int numSteps);
  void step(int numSteps);
  void pause();
  void reset();
  void seek(timestamp_t timestampToSeek);
  void status();

private:
  std::vector<Event> events_;
  size_t currentEvent_;
  bool isPaused_;
  std::thread replayThread_;
  int numSteps_;

  std::mutex m;
  std::condition_variable cv;
  bool playing = false;
  bool stepping = false;
  bool stop = false;
};