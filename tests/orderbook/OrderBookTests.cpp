#include "OrderBook.hpp"
#include <gtest/gtest.h>

class OrderBookTest : public testing::Test {
  protected:
    Order createOrder(orderId_t id, price_t price, quantity_t quantity, side_t side) {
        return Order(id, 1000000 + id, price, quantity, "AAPL", side);
    }
    OrderBook orderBook;
};

TEST_F(OrderBookTest, EmptyBookHasNoPriceLevels) {
    EXPECT_TRUE(orderBook.getPriceLevels("BUY").empty());
    EXPECT_TRUE(orderBook.getPriceLevels("SELL").empty());
}

TEST_F(OrderBookTest, BuyAndSellOrdersStoredSeparately) {
    auto buy = createOrder(1, 150.25, 100, "BUY");
    auto sell = createOrder(2, 150.30, 100, "SELL");

    orderBook.addOrder(buy);
    orderBook.addOrder(sell);

    EXPECT_EQ(orderBook.getPriceLevels("BUY"),
              (std::vector<std::pair<price_t, quantity_t>>{{150.25, 100}}));

    EXPECT_EQ(orderBook.getPriceLevels("SELL"),
              (std::vector<std::pair<price_t, quantity_t>>{{150.30, 100}}));
}

TEST_F(OrderBookTest, QuantityAccumulatedIfSamePrice) {
    auto order1 = createOrder(1, 150.25, 100, "BUY");
    auto order2 = createOrder(2, 150.25, 100, "BUY");
    auto order3 = createOrder(3, 150.30, 100, "BUY");

    orderBook.addOrder(order1);
    orderBook.addOrder(order2);
    orderBook.addOrder(order3);

    EXPECT_EQ(orderBook.getPriceLevels("BUY"),
              (std::vector<std::pair<price_t, quantity_t>>{{150.30, 100}, {150.25, 200}}));
}

TEST_F(OrderBookTest, HighestBidsReturnFromPriceLevels) {
    Order order1 = createOrder(1, 150.25, 100, "BUY");
    Order order2 = createOrder(2, 150.30, 100, "BUY");

    orderBook.addOrder(order1);
    orderBook.addOrder(order2);

    std::vector<std::pair<price_t, quantity_t>> expected{{150.30, 100}, {150.25, 100}};
    EXPECT_EQ(orderBook.getPriceLevels("BUY"), expected);
}

TEST_F(OrderBookTest, LowestSellsReturnFromPriceLevels) {
    Order order1 = createOrder(1, 150.20, 100, "SELL");
    Order order2 = createOrder(1, 150.25, 100, "SELL");
    Order order3 = createOrder(2, 150.30, 100, "SELL");

    orderBook.addOrder(order1);
    orderBook.addOrder(order2);
    orderBook.addOrder(order3);

    std::vector<std::pair<price_t, quantity_t>> expected{
        {150.20, 100}, {150.25, 100}, {150.30, 100}};
    EXPECT_EQ(orderBook.getPriceLevels("SELL"), expected);
}

TEST_F(OrderBookTest, NoTradeWhenPricesDoNotCross) {
    auto buy = createOrder(1, 150.00, 100, "BUY");
    auto sell = createOrder(2, 151.00, 100, "SELL");

    orderBook.addOrder(buy);
    orderBook.addOrder(sell);

    EXPECT_EQ(orderBook.getPriceLevels("BUY"),
              (std::vector<std::pair<price_t, quantity_t>>{{150.00, 100}}));

    EXPECT_EQ(orderBook.getPriceLevels("SELL"),
              (std::vector<std::pair<price_t, quantity_t>>{{151.00, 100}}));
}

TEST_F(OrderBookTest, FullyMatchedOrdersAreRemoved) {
    auto sell = createOrder(1, 150.00, 100, "SELL");
    auto buy = createOrder(2, 150.00, 100, "BUY");

    orderBook.addOrder(sell);
    orderBook.addOrder(buy);

    EXPECT_TRUE(orderBook.getPriceLevels("BUY").empty());
    EXPECT_TRUE(orderBook.getPriceLevels("SELL").empty());
}

TEST_F(OrderBookTest, IncomingOrderCanBePartiallyFilled) {
    auto sell = createOrder(1, 150.00, 100, "SELL");
    auto buy = createOrder(2, 150.00, 200, "BUY");

    orderBook.addOrder(sell);
    orderBook.addOrder(buy);

    EXPECT_EQ(orderBook.getPriceLevels("BUY"),
              (std::vector<std::pair<price_t, quantity_t>>{{150.00, 100}}));

    EXPECT_TRUE(orderBook.getPriceLevels("SELL").empty());
}