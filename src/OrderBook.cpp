#include "OrderBook.hpp"
#include <algorithm>

void OrderBook::reset() {
    buys.clear();
    sells.clear();
    orders.clear();
    buyPriceLevels.clear();
    sellPriceLevels.clear();
}

bool OrderBook::cancelOrder(orderId_t orderId) {
    if (!orders.contains(orderId))
        return false;

    Order* order = orders[orderId].get();

    order->cancelled_ = true;

    if (order->side_ == "BUY") {
        updatePriceLevels(buyPriceLevels, order->price_, order->quantity_, false);
    } else if (order->side_ == "SELL") {
        updatePriceLevels(sellPriceLevels, order->price_, order->quantity_, false);
    }

    return true;
}

template <typename Compare>
void OrderBook::updatePriceLevels(price_levels_t<Compare>& priceLevels, price_t& price,
                                  quantity_t& quantity, bool add) {
    if (!priceLevels.contains(price)) {
        priceLevels.emplace(price, 0);
    }

    if (add) {
        priceLevels[price] += quantity;
    } else {
        priceLevels[price] -= quantity;
        if (priceLevels[price] == 0) {
            priceLevels.erase(price);
        }
    }
}

std::vector<Trade> OrderBook::addOrder(Order& order) {
    return addOrder(order.orderId_, order.timestamp_, order.price_, order.quantity_, order.symbol_,
                    order.side_);
}

std::vector<Trade> OrderBook::addOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                       quantity_t quantity, symbol_t symbol, side_t side) {

    if (side == "BUY") {
        return addBuyOrder(orderId, timestamp, price, quantity, symbol);
    } else if (side == "SELL") {
        return addSellOrder(orderId, timestamp, price, quantity, symbol);
    }

    return std::vector<Trade>{};
}

std::vector<Trade> OrderBook::addBuyOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                          quantity_t quantity, symbol_t symbol) {

    std::vector<Trade> trades{};

    while (quantity > 0 and !sells.empty()) {
        auto& [lowestSellPrice, sellOrders] = *sells.begin();

        if (price >= lowestSellPrice) {
            Order* sellOrder = sellOrders.front();
            sellOrders.pop();

            if (!sellOrder->cancelled_) {
                auto tradeQuantity = std::min(sellOrder->quantity_, quantity);
                quantity -= tradeQuantity;

                updatePriceLevels(sellPriceLevels, sellOrder->price_, tradeQuantity, false);

                Trade trade = {.timestamp_ = timestamp,
                               .price_ = price,
                               .quantity_ = tradeQuantity,
                               .aggressor_ = orderId};

                trades.push_back(trade);
            } else {
                orders.erase(sellOrder->orderId_);
            }

            if (sellOrders.size() == 0) {
                sells.erase(lowestSellPrice);
            }
        } else
            break;
    }

    if (quantity > 0) {
        std::unique_ptr<Order> order =
            std::make_unique<Order>(Order{orderId, timestamp, price, quantity, symbol, "BUY"});

        if (!buys.contains(price)) {
            buys.emplace(std::make_pair(price, std::queue<Order*>{}));
        }

        auto& buyOrders = buys.at(price);
        buyOrders.push(order.get());
        orders.emplace(std::make_pair(orderId, std::move(order)));

        if (!buyPriceLevels.contains(price)) {
            buyPriceLevels.emplace(std::make_pair(price, 0));
        }

        buyPriceLevels[price] += quantity;
    }

    return trades;
}

std::vector<Trade> OrderBook::addSellOrder(orderId_t orderId, timestamp_t timestamp, price_t price,
                                           quantity_t quantity, symbol_t symbol) {

    std::vector<Trade> trades{};

    while (quantity > 0 and !buys.empty()) {
        auto& [highestBuyPrice, buyOrders] = *buys.begin();

        if (price <= highestBuyPrice) {
            Order* buyOrder = buyOrders.front();
            buyOrders.pop();

            if (!buyOrder->cancelled_) {
                auto tradeQuantity = std::min(buyOrder->quantity_, quantity);
                quantity -= tradeQuantity;
                buyPriceLevels[buyOrder->price_] -= tradeQuantity;

                Trade trade = {.timestamp_ = timestamp,
                               .price_ = price,
                               .quantity_ = tradeQuantity,
                               .aggressor_ = orderId};

                trades.push_back(trade);
            } else {
                orders.erase(buyOrder->orderId_);
            }

            if (buyOrders.size() == 0) {
                buys.erase(highestBuyPrice);
            }
        } else
            break;
    }

    if (quantity > 0) {
        std::unique_ptr<Order> order =
            std::make_unique<Order>(Order{orderId, timestamp, price, quantity, symbol, "SELL"});

        if (!sells.contains(price)) {
            sells.emplace(std::make_pair(price, std::queue<Order*>{}));
        }

        auto& sellOrders = sells.at(price);
        sellOrders.push(order.get());
        orders.emplace(std::make_pair(orderId, std::move(order)));

        if (!sellPriceLevels.contains(price)) {
            sellPriceLevels.emplace(std::make_pair(price, 0));
        }

        sellPriceLevels[price] += quantity;
    }

    return trades;
}

std::vector<std::pair<price_t, quantity_t>> OrderBook::getPriceLevels(std::string side) {
    std::vector<std::pair<price_t, quantity_t>> priceLevels{};

    if (side == "BUY") {
        for (const auto& [price, totalQuantity] : buyPriceLevels) {
            priceLevels.emplace_back(price, totalQuantity);
        }
    } else if (side == "SELL") {
        for (const auto& [price, totalQuantity] : sellPriceLevels) {
            priceLevels.emplace_back(price, totalQuantity);
        }
    }

    return priceLevels;
}

bool OrderBook::modifyOrder(orderId_t orderId, quantity_t quantity) {
    if (!orders.contains(orderId))
        return false;

    return true;
}