#include "Renderer.hpp"
#include "ftxui/component/app.hpp"            // for Component, App
#include "ftxui/component/captured_mouse.hpp" // for ftxui
#include "ftxui/component/component.hpp" // for Slider, Checkbox, Vertical, Renderer, Button, Input, Menu, Radiobox, Toggle
#include "ftxui/dom/elements.hpp"
#include <format>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

void renderApp(Application& app) {
    auto screen = ScreenInteractive::FitComponent();

    const std::vector<std::string> menu_entries = {
        "play", "step", "pause", "reset", "seek", "quit",
    };
    int menu_selected = -1;
    auto menu = Menu(&menu_entries, &menu_selected);

    auto component = Renderer(menu, [&] {
        return vbox({
                   text("Market Replay") | bold | center,
                   separator(),

                   hbox({
                       renderReplayInfo(app) | size(WIDTH, EQUAL, REPLAY_INFO_TOTAL_WIDTH),
                       renderOrderBook(app) | size(WIDTH, EQUAL, ORDERBOOK_TOTAL_WIDTH),
                       menu->Render() | border | size(WIDTH, EQUAL, MENU_TOTAL_WIDTH),
                   }),
               }) |
               size(WIDTH, EQUAL, APP_WIDTH) | size(HEIGHT, EQUAL, APP_HEIGHT) | border;
    });

    component = CatchEvent(component, [&](Event event) {
        if (event == Event::Return) {
            app.parse(menu_selected);
            return true;
        }
        return false;
    });

    screen.Loop(component);

    std::thread([&] {
        while (true) {
            std::this_thread::sleep_for(std::chrono::microseconds(500));
            screen.Post(Event::Custom);
        }
    }).detach();
}

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
    auto bids = app.getPriceLevels("BUY");
    Elements bidElements;
    for (const auto& [price, quantity] : bids) {
        bidElements.push_back(text(std::format("{:<10} {:>8}", price, quantity)));
    }

    auto sells = app.getPriceLevels("SELL");
    Elements sellElements;

    for (const auto& [price, quantity] : sells) {
        sellElements.push_back(text(std::format("{:<10} {:>8}", quantity, price)));
    }

    return vbox({text("Order Book") | bold | center, separator(),

                 hbox({
                     text(std::format("{:^{}}", "Bid", ORDERBOOK_TOTAL_WIDTH / 2)) | bold,
                     separator(),
                     text(std::format("{:^{}}", "Ask", ORDERBOOK_TOTAL_WIDTH / 2)) | bold,
                 }),

                 separator(),

                 hbox({
                     vbox(bidElements) | size(WIDTH, EQUAL, ORDERBOOK_TOTAL_WIDTH / 2),
                     separator(),
                     vbox(sellElements) | size(WIDTH, EQUAL, ORDERBOOK_TOTAL_WIDTH / 2),
                 })}) |
           border | size(WIDTH, EQUAL, ORDERBOOK_TOTAL_WIDTH);
}