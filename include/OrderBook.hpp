#pragma once

#include "Order.hpp"
#include <map>
#include <queue>
#include <vector>

struct Trade {
    timestamp_t timestamp;
    price_t price;
    quantity_t quantity;
    orderId_t aggressor;
};

class OrderBook {
  public:
    bool cancelOrder(orderId_t orderId);
    std::vector<Trade> addOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                quantity_t quantity, type_t type, symbol_t symbol, side_t side);
    bool modifyOrder(orderId_t orderId, quantity_t quantity);

  private:
    std::vector<Trade> addBuyOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                   quantity_t quantity, type_t type, symbol_t symbol);
    std::vector<Trade> addSellOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                    quantity_t quantity, type_t type, symbol_t symbol);
    std::map<int, std::queue<Order*>, std::greater<int>> buys;
    std::map<int, std::queue<Order*>> sells;
    std::map<orderId_t, std::unique_ptr<Order>> orders;
};