#include <iostream>
#include "Event.hpp"
#include <string_view>
#include <vector>
#include <ranges>
#include <sstream>
#include <thread>
#include <chrono>

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
    ReplayEngine(std::vector<std::string> logs) {
        for (size_t i = 1; i < logs.size(); i++) { // skip first line
            std::vector<std::string> tokens;
            for (const auto word : std::views::split(logs[i], ',')) {
                std::string token {word.begin(), word.end()};
                tokens.push_back(token);
            }

            auto eventId = std::stoull(tokens[static_cast<size_t>(EventParams::Id)]);
            Event event = Event(
                std::stoul(tokens[static_cast<size_t>(EventParams::Id)]),
                std::stoul(tokens[static_cast<size_t>(EventParams::OrderId)]),
                std::stoul(tokens[static_cast<size_t>(EventParams::Timestamp)]),
                std::stoul(tokens[static_cast<size_t>(EventParams::Price)]),
                std::stoul(tokens[static_cast<size_t>(EventParams::Quantity)]),
                tokens[static_cast<size_t>(EventParams::Type)],
                tokens[static_cast<size_t>(EventParams::Symbol)],
                tokens[static_cast<size_t>(EventParams::Side)]
            );
            events_.push_back(event);
        }
    }

    void play() {
        for (Event event : events_) {
            std::this_thread::sleep_for(std::chrono::seconds(SECONDS_TO_DELAY));
            event.print();
        }
    }
    private:
    std::vector<Event> events_;
};