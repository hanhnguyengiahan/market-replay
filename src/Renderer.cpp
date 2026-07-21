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
    // auto bids = app.getBids();
    // auto asks = app.getAsks();

    return vbox({
               text("Order Book") | bold | center,
               separator(),

               hbox({
                   text("Bid") | bold | flex,
                   text("Ask") | bold | flex,
               }),

               separator(),

               hbox({
                   vbox({
                       text("100.25   500"),
                       text("100.24   800"),
                       text("100.23   150"),
                   }) | flex,

                   separator(),

                   vbox({
                       text("300   100.30"),
                       text("200   100.31"),
                       text("600   100.32"),
                   }) | flex,
               }),
           }) |
           border;
}