#include "Event.hpp"

Event::Event(eventId_t id, orderId_t orderId, timestamp_t timestamp,
    price_t price, quantity_t quantity, type_t type, 
    symbol_t symbol, side_t side):
    id_{id}, orderId_{orderId}, timestamp_{timestamp}, 
    price_{price}, quantity_{quantity}, type_{type}, 
    symbol_{symbol}, side_{side}
    {};

void Event::print() {
    std::cout << std::format(
        "[{}] {:<15} id={:<5} order_id={:<5} symbol={:<5} side={:<5} price={:<5} qty={}\n",
        timestamp_, type_, id_, orderId_, symbol_, side_, price_, quantity_); 
}

timestamp_t Event::getTimestamp() {
    return timestamp_;
}