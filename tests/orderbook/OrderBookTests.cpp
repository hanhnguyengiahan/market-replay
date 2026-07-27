#include "OrderBook.hpp"
#include <gtest/gtest.h>

class OrderBookTest : public testing::Test {
  protected:
    OrderBookTest() = default;
    Order createOrder(orderId_t id, price_t price, quantity_t quantity, side_t side) {
        return Order(id, 1000000 + id, price, quantity, "AAPL", side);
    }
    OrderBook orderbook;
};

TEST_F(OrderBookTest, HighestBidIsReturned) {
    OrderBook book;

    Order order1 = createOrder(1, 150.25, 100, "BUY");
    Order order2 = createOrder(2, 150.30, 100, "BUY");

    book.addOrder(order1);
    book.addOrder(order2);

    std::vector<std::pair<price_t, quantity_t>> expected{{150.30, 100}, {150.25, 100}};
    EXPECT_EQ(book.getPriceLevels("BUY"), expected);
}