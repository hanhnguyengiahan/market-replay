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
    ReplayEngine(std::vector<std::string> logs): events_{}, currentEvent_{0}, isPaused_{false} {
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
        while (currentEvent_ < events_.size()) {
            step();
            std::this_thread::sleep_for(std::chrono::seconds(SECONDS_TO_DELAY));
        }
    }

    void step() {
        if (currentEvent_ >= 0 and currentEvent_ < events_.size()) {
            events_[currentEvent_].print();
            currentEvent_ += 1;
        }
    }

    void step(int numSteps) {
        for (int i = 0; i < numSteps; i++) {
            step();
        }
    }

    void reset() {
        currentEvent_ = 0;
    }

    void seek(timestamp_t timestampToSeek) {
        for (size_t i = 0; i < events_.size(); i++) {
            if (events_[i].getTimestamp() == timestampToSeek) {
                currentEvent_ = i;
                break;
            }
        }
    }

    /*
    > status

        State: Paused
        Current event: 12539 / 1532840
        Timestamp: 09:30:00.000024
        Speed: 1x
    */
    void status() {
        // std::string statusFormat = std::format(
        //     "State: {}\n"
        //     "Current event: {}\n"
        //     "Timestamp: {}"
        //     "Speed: {}",
        //     isPaused_,
        //     currentEvent_,
        //     events_[currentEvent_].timestamp_,
        //     current_speed_,
        // );
    }
    
    private:
    std::vector<Event> events_;
    size_t currentEvent_;
    bool isPaused_;
};