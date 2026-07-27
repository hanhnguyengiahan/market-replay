#pragma once

#include "Order.hpp"
#include <map>
#include <queue>
#include <vector>

struct Trade {
    timestamp_t timestamp_;
    price_t price_;
    quantity_t quantity_;
    orderId_t aggressor_;
};

using orders_queue_t = std::queue<Order*>;
template <typename Compare> using price_levels_t = std::map<price_t, quantity_t, Compare>;

class OrderBook {
  public:
    OrderBook() = default;
    bool cancelOrder(orderId_t orderId);
    std::vector<Trade> addOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                quantity_t quantity, symbol_t symbol, side_t side);
    std::vector<Trade> addOrder(Order& order);
    bool modifyOrder(orderId_t orderId, quantity_t quantity);
    std::vector<std::pair<price_t, quantity_t>> getPriceLevels(std::string side);

  private:
    std::vector<Trade> addBuyOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                   quantity_t quantity, symbol_t symbol);
    std::vector<Trade> addSellOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                    quantity_t quantity, symbol_t symbol);
    template <typename Compare>
    void updatePriceLevels(price_levels_t<Compare>& priceLevels, price_t& price,
                           quantity_t& quantity, bool add);
    std::map<price_t, orders_queue_t, std::greater<price_t>> buys;
    std::map<price_t, orders_queue_t> sells;
    std::map<orderId_t, std::unique_ptr<Order>> orders;
    price_levels_t<std::greater<price_t>> buyPriceLevels;
    price_levels_t<std::less<price_t>> sellPriceLevels;
};