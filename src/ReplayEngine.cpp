#include "ReplayEngine.hpp"
#include <utility>

ReplayEngine::ReplayEngine(std::vector<std::string> logs)
    : events_{}, currentEvent_{-1}, isPaused_{false}, orderBook_{} {
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
                        std::stod(tokens[static_cast<size_t>(EventParams::Price)]),
                        std::stoul(tokens[static_cast<size_t>(EventParams::Quantity)]),
                        tokens[static_cast<size_t>(EventParams::Type)],
                        tokens[static_cast<size_t>(EventParams::Symbol)],
                        tokens[static_cast<size_t>(EventParams::Side)]);
        events_.push_back(event);
    }

    replayThread_ = std::jthread([this](std::stop_token st) { keepAlive(st); });
}

double ReplayEngine::getProgress() {
    return static_cast<double>(currentEvent_ + 1) / static_cast<double>(events_.size());
}

std::string ReplayEngine::getLastEvent() {
    if (currentEvent_ == -1)
        return "";
    return events_[currentEvent_].print();
}

std::string ReplayEngine::getLastEventTimestamp() {
    if (currentEvent_ == -1)
        return "";

    return std::to_string(events_[currentEvent_].getTimestamp());
}

std::vector<std::pair<std::string, std::string>> ReplayEngine::getPriceLevels(std::string side) {
    std::vector<std::pair<std::string, std::string>> res{};
    std::vector<std::pair<price_t, quantity_t>> priceLevels = orderBook_.getPriceLevels(side);
    for (const auto& priceLevel : priceLevels) {
        res.emplace_back(std::to_string(priceLevel.first), std::to_string(priceLevel.second));
    }
    return res;
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
    while (!paused && std::cmp_less(currentEvent_, events_.size() - 1)) {
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

        if (std::cmp_less(currentEvent_, events_.size() - 1)) {
            currentEvent_ += 1;
            auto event = events_[currentEvent_];
            event.print();
            updateOrderBook(event);
        }
    }
    stepping = false;
}

void ReplayEngine::updateOrderBook(MarketEvent& event) {
    orderBook_.addOrder(event.orderId_, event.timestamp_, event.price_, event.quantity_,
                        event.type_, event.symbol_, event.side_);
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
    currentEvent_ = -1;
}

void ReplayEngine::seek(timestamp_t timestampToSeek) {
    for (size_t i = 0; i < events_.size(); i++) {
        if (events_[i].getTimestamp() == timestampToSeek) {
            currentEvent_ = i;
            break;
        }
    }
}

std::string ReplayEngine::status() {
    if (currentEvent_ == events_.size() - 1) {
        return "Finished";
    }

    if (currentEvent_ != -1) {
        return "In progress";
    }
    return "Ready";
}
