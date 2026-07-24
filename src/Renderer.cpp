#include "Renderer.hpp"

using namespace ftxui;

Element renderReplayInfo(Application& app) {
    return vbox({
               text("Filename: " + app.getFilename()),
               text("State: " + app.getStatus()),
               text("Progress") | bold,
               gauge(app.getProgress()) | border,
               text("Timestamp: " + app.getLastEventTimestamp()),
               separator(),
               text("Last Event") | bold,
               text(app.getLastEvent()),
           }) |
           border;
}

Element renderOrderBook(Application& app) {
    using namespace ftxui;

    auto bids = app.getPriceLevels("BUY");
    Elements bidElements;

    for (const auto& [price, quantity] : bids) {
        std::cout << text(std::format("{:<10} {:>8}", price, quantity)) << '\n';
        bidElements.push_back(text(std::format("{:<10} {:>8}", price, quantity)));
    }

    auto sells = app.getPriceLevels("SELL");
    Elements sellElements;

    for (const auto& [price, quantity] : sells) {
        std::cout << text(std::format("{:<10} {:>8}", price, quantity)) << '\n';
        sellElements.push_back(text(std::format("{:<10} {:>8}", price, quantity)));
    }

    return vbox({
               text("Order Book") | bold | center,
               separator(),

               hbox({
                   text("Bid") | bold | flex,
                   text("Ask") | bold | flex,
               }),

               separator(),

               hbox({
                   vbox(bidElements) | flex,

                   separator(),

                   vbox(sellElements) | flex,
               }),
           }) |
           border;
}