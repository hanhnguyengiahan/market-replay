#include <unistd.h>
#include <iostream>
#include <format>

enum class Side {
    BUY,
    SELL
};

enum class EventType {
    ADD_ORDER,
    CANCEL_ORDER,
    TRADE
};

using timestamp_t = size_t;
using eid_t = size_t;
using price_t = size_t;
using quantity_t = size_t;
using symbol_t = std::string;
using type_t = std::string;
using side_t = std::string;

class Event {
    public:
        Event() = default;
        Event(eid_t id, eid_t orderId, timestamp_t timestamp,
            price_t price, quantity_t quantity, type_t type, 
            symbol_t symbol, side_t side):
            id_{id}, orderId_{orderId}, timestamp_{timestamp}, 
            price_{price}, quantity_{quantity}, type_{type}, 
            symbol_{symbol}, side_{side}
            {};
        
        void print() {
            std::cout << std::format(
                "[{}] {:<15} id={:<5} order_id={:<5} symbol={:<5} side={:<5} price={:<5} qty={}\n",
                timestamp_, type_, id_, orderId_, symbol_, side_, price_, quantity_); 
        }
    private: 
        eid_t id_;
        eid_t orderId_;
        timestamp_t timestamp_;
        type_t type_;
        symbol_t symbol_;
        side_t side_;
        price_t price_;
        quantity_t quantity_;
};