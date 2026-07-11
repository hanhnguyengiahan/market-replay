#include <format>
#include <iostream>
#include <unistd.h>

enum class Side { BUY, SELL };

enum class EventType { ADD_ORDER, CANCEL_ORDER, TRADE };

using timestamp_t = size_t;
using eventId_t = size_t;
using orderId_t = size_t;
using price_t = size_t;
using quantity_t = size_t;
using symbol_t = std::string;
using type_t = std::string;
using side_t = std::string;

class Event {
public:
  Event() = default;
  Event(eventId_t id, orderId_t orderId, timestamp_t timestamp, price_t price,
        quantity_t quantity, type_t type, symbol_t symbol, side_t side);

  void print();

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