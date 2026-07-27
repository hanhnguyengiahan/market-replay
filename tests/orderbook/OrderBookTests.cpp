#include "OrderBook.hpp"
#include <gtest/gtest.h>

TEST(OrderBookTest, HighestBidIsReturned) {
    OrderBook book;

    Order order1(1, 1000000, 150.25, 100, "AAPL", "BUY");

    Order order2(2, 1000001, 150.30, 100, "AAPL", "BUY");

    book.addOrder(order1);
    book.addOrder(order2);

    std::vector<std::pair<price_t, quantity_t>> expected{{150.30, 100}, {150.25, 100}};

    EXPECT_EQ(book.getPriceLevels("BUY"), expected);
}