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
    OrderBook() = default;
    bool cancelOrder(orderId_t orderId);
    std::vector<Trade> addOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                quantity_t quantity, type_t type, symbol_t symbol, side_t side);
    bool modifyOrder(orderId_t orderId, quantity_t quantity);
    std::vector<std::pair<price_t, quantity_t>> getPriceLevels(std::string side);

  private:
    std::vector<Trade> addBuyOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                   quantity_t quantity, type_t type, symbol_t symbol);
    std::vector<Trade> addSellOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                    quantity_t quantity, type_t type, symbol_t symbol);
    std::map<price_t, std::queue<Order*>, std::greater<price_t>> buys;
    std::map<price_t, std::queue<Order*>> sells;
    std::map<orderId_t, std::unique_ptr<Order>> orders;
    std::map<price_t, quantity_t> buyPriceLevels;
    std::map<price_t, quantity_t> sellPriceLevels;
};