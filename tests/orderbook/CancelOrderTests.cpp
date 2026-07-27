#include "OrderBook.hpp"
#include <gtest/gtest.h>

class CancelOrderTest : public testing::Test {
  protected:
    OrderBook orderBook;

    Order buy1 = Order(1, 1000001, 150.00, 100, "AAPL", "BUY");
    Order buy2 = Order(2, 1000002, 150.50, 200, "AAPL", "BUY");
    Order sell1 = Order(3, 1000003, 151.00, 150, "AAPL", "SELL");

    void SetUp() override {
        orderBook.addOrder(buy1);
        orderBook.addOrder(buy2);
        orderBook.addOrder(sell1);
    }
};

TEST_F(CancelOrderTest, RemovesExistingBuyOrder) {
    EXPECT_TRUE(orderBook.cancelOrder(1));

    std::vector<std::pair<price_t, quantity_t>> expected{{150.50, 200}};

    EXPECT_EQ(orderBook.getPriceLevels("BUY"), expected);
}

TEST_F(CancelOrderTest, CancellingUnknownOrderReturnsFalse) {
    EXPECT_FALSE(orderBook.cancelOrder(999));

    EXPECT_EQ(orderBook.getPriceLevels("BUY"),
              (std::vector<std::pair<price_t, quantity_t>>{{150.50, 200}, {150.00, 100}}));
}

TEST_F(CancelOrderTest, CancellingOneOrderReducesQuantityAtPriceLevel) {
    Order order4 = Order(4, 1000004, 150.00, 50, "AAPL", "BUY");
    orderBook.addOrder(order4);

    EXPECT_TRUE(orderBook.cancelOrder(1));

    EXPECT_EQ(orderBook.getPriceLevels("BUY"),
              (std::vector<std::pair<price_t, quantity_t>>{{150.50, 200}, {150.00, 50}}));
}