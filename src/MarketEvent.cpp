#include "MarketEvent.hpp"

MarketEvent::MarketEvent(eventId_t id, orderId_t orderId, timestamp_t timestamp, price_t price,
                         quantity_t quantity, type_t type, symbol_t symbol, side_t side)
    : id_{id}, orderId_{orderId}, timestamp_{timestamp}, price_{price}, quantity_{quantity},
      type_{type}, symbol_{symbol}, side_{side} {};

std::string MarketEvent::print() {
    return std::format("{} {} {} @ {:.2f} x {}", side_, type_, symbol_, price_, quantity_);
}

timestamp_t MarketEvent::getTimestamp() {
    return timestamp_;
}