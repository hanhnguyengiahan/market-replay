#include "OrderBook.hpp"
#include <algorithm>

bool OrderBook::cancelOrder(orderId_t orderId) {
    if (!orders.contains(orderId))
        return false;
    auto result = orders.erase(orderId);

    // todo: remove order out of the buys or sells map
    return result == 1 ? true : false;
}

std::vector<Trade> OrderBook::addOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                       quantity_t quantity, type_t type, symbol_t symbol,
                                       side_t side) {

    if (side == "BUY") {
        return addBuyOrder(orderId, timestamp, price, quantity, type, symbol);
    } else if (side == "SELL") {
        return addSellOrder(orderId, timestamp, price, quantity, type, symbol);
    }

    return std::vector<Trade>{};
}

std::vector<Trade> OrderBook::addBuyOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                          quantity_t quantity, type_t type, symbol_t symbol) {

    std::vector<Trade> trades{};

    while (quantity > 0 and !sells.empty()) {
        auto [lowestSellPrice, sellOrders] = *sells.begin();

        if (price >= lowestSellPrice) {
            Order* sellOrder = sellOrders.front();
            sellOrders.pop();
            auto tradeQuantity = std::min(sellOrder->quantity_, quantity);
            quantity -= tradeQuantity;

            Trade trade = {.timestamp = timestamp,
                           .price = price,
                           .quantity = tradeQuantity,
                           .aggressor = orderId};

            trades.push_back(trade);

            if (sellOrders.size() == 0) {
                sells.erase(lowestSellPrice);
            }
        }
    }

    if (quantity > 0) {
        std::unique_ptr<Order> order = std::make_unique<Order>(
            Order{orderId, timestamp, price, quantity, type, symbol, "BUY"});
        orders.emplace(std::make_pair(orderId, std::move(order)));

        if (!buys.contains(price)) {
            buys.emplace(std::make_pair(price, std::queue<Order*>{}));
        }

        auto buyOrders = buys.at(price);
        buyOrders.push(order.get());
    }

    return trades;
}

std::vector<Trade> OrderBook::addSellOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                           quantity_t quantity, type_t type, symbol_t symbol) {

    std::vector<Trade> trades{};

    while (quantity > 0 and !buys.empty()) {
        auto [highestBuyPrice, buyOrders] = *buys.begin();

        if (price <= highestBuyPrice) {
            Order* buyOrder = buyOrders.front();
            buyOrders.pop();
            auto tradeQuantity = std::min(buyOrder->quantity_, quantity);
            quantity -= tradeQuantity;

            Trade trade = {.timestamp = timestamp,
                           .price = price,
                           .quantity = tradeQuantity,
                           .aggressor = orderId};

            trades.push_back(trade);

            if (buyOrders.size() == 0) {
                buys.erase(highestBuyPrice);
            }
        }
    }

    if (quantity > 0) {
        std::unique_ptr<Order> order = std::make_unique<Order>(
            Order{orderId, timestamp, price, quantity, type, symbol, "SELL"});
        orders.emplace(std::make_pair(orderId, std::move(order)));

        if (!sells.contains(price)) {
            sells.emplace(std::make_pair(price, std::queue<Order*>{}));
        }

        auto sellOrders = sells.at(price);
        sellOrders.push(order.get());
    }

    return trades;
}
bool OrderBook::modifyOrder(orderId_t orderId, quantity_t quantity) {
    if (!orders.contains(orderId))
        return false;

    return true;
}