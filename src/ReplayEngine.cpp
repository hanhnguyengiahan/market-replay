#include "ReplayEngine.hpp"

ReplayEngine::ReplayEngine(std::vector<std::string> logs)
    : events_{}, currentEvent_{0}, isPaused_{false} {
    for (size_t i = 1; i < logs.size(); i++) { // skip first line
        std::vector<std::string> tokens;
        for (const auto word : std::views::split(logs[i], ',')) {
            std::string token{word.begin(), word.end()};
            tokens.push_back(token);
        }

        auto eventId = std::stoull(tokens[static_cast<size_t>(EventParams::Id)]);
        MarketEvent event =
            MarketEvent(std::stoul(tokens[static_cast<size_t>(EventParams::Id)]),
                        std::stoul(tokens[static_cast<size_t>(EventParams::OrderId)]),
                        std::stoul(tokens[static_cast<size_t>(EventParams::Timestamp)]),
                        std::stoul(tokens[static_cast<size_t>(EventParams::Price)]),
                        std::stoul(tokens[static_cast<size_t>(EventParams::Quantity)]),
                        tokens[static_cast<size_t>(EventParams::Type)],
                        tokens[static_cast<size_t>(EventParams::Symbol)],
                        tokens[static_cast<size_t>(EventParams::Side)]);
        events_.push_back(event);
    }

    replayThread_ = std::jthread([this](std::stop_token st) { keepAlive(st); });
}

double ReplayEngine::getProgress() {
    if (events_.size() == 0)
        return 0;
    return static_cast<double>(currentEvent_) / static_cast<double>(events_.size());
}

std::string ReplayEngine::getLastEvent() {
    if (currentEvent_ == events_.size())
        return events_[currentEvent_ - 1].print();
    return events_[currentEvent_].print();
}

void ReplayEngine::keepAlive(std::stop_token st) {
    while (!st.stop_requested()) {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, st, [this] { return !paused && (playing || stepping); });

        if (st.stop_requested()) {
            break;
        }

        if (playing) {
            executePlay();
        } else if (stepping) {
            executeStep(numSteps_);
        }
        lk.unlock();
    }
}

void ReplayEngine::executePlay() {
    while (!paused && currentEvent_ < events_.size()) {
        executeStep(1);
        std::this_thread::sleep_for(std::chrono::seconds(SECONDS_TO_DELAY));
    }
    playing = false;
}

void ReplayEngine::play() {
    {
        std::unique_lock<std::mutex> lk(m);
        paused = false;
        playing = true;
    }
    cv.notify_one();
}

void ReplayEngine::executeStep(int numSteps) {
    for (int i = 0; i < numSteps; i++) {
        if (paused)
            break;
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
        paused = false;
        stepping = true;
        numSteps_ = numSteps;
    }
    cv.notify_one();
}

void ReplayEngine::pause() {
    paused = true;
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
