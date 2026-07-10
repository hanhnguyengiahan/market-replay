#include "ReplayEngine.hpp"

ReplayEngine::ReplayEngine(std::vector<std::string> logs):
    events_{}, currentEvent_{0}, isPaused_{false}, replayThread_{std::thread(&ReplayEngine::keepAlive, this)} {
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

void ReplayEngine::executePlay() {
    while (!stop && currentEvent_ < events_.size()) {
        executeStep(1);
        std::this_thread::sleep_for(std::chrono::seconds(SECONDS_TO_DELAY));
    }
    playing = false;
}

void ReplayEngine::keepAlive() {
    while (1) {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [this]{ return !stop && (playing | stepping); });
        if (playing) {
            executePlay();
        } else if (stepping) {
            executeStep(numSteps_);
        }
        lk.unlock();
    }
}

void ReplayEngine::play() {
    {
        std::unique_lock<std::mutex> lk(m);
        stop = false;
        playing = true;
    }
    cv.notify_one();
}

void ReplayEngine::executeStep(int numSteps) {
    for (int i = 0; i < numSteps; i++) {
        if (stop) break;
        if (currentEvent_ >= 0 and currentEvent_ < events_.size()) {
            events_[currentEvent_].print();
            currentEvent_ += 1;
        }
    }
    stepping = false;
}

void ReplayEngine::step(int numSteps) {
    {
        std::unique_lock<std::mutex> lk(m);
        stop = false;
        stepping = true;
        numSteps_ = numSteps;
    }
    cv.notify_one();
}

void ReplayEngine::pause() {
    // set the 'stop' flag to true so the current thread can stop
    stop = true;
    cv.notify_one();
}

void ReplayEngine::reset() {
    currentEvent_ = 0;
}

void ReplayEngine::seek(timestamp_t timestampToSeek) {
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
void ReplayEngine::status() {
    // std::string statusFormat = std::format(
    //     "State: {}\n"
    //     "Current event: {}\n"
    //     "Timestamp: {}"
    //     "Speed: {}",
    //     stop,
    //     currentEvent_,
    //     events_[currentEvent_].timestamp_,
    //     current_speed_,
    // );
}
