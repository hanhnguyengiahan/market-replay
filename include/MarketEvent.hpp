#pragma once

#include "Order.hpp"
#include <iostream>
#include <unistd.h>

class MarketEvent {
  public:
    MarketEvent() = default;
    MarketEvent(eventId_t id, orderId_t orderId, timestamp_t timestamp, price_t price,
                quantity_t quantity, type_t type, symbol_t symbol, side_t side);

    std::string print();

    timestamp_t getTimestamp();

  private:
    eventId_t id_;
    orderId_t orderId_;
    timestamp_t timestamp_;
    type_t type_;
    symbol_t symbol_;
    side_t side_;
    price_t price_;
    quantity_t quantity_;
};