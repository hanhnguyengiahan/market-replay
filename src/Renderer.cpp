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
                       vbox({
                           filler(),
                           menu->Render(),
                           filler(),
                       }) | border |
                           size(WIDTH, EQUAL, MENU_TOTAL_WIDTH),
                   }),
               }) |
               size(WIDTH, EQUAL, APP_WIDTH) | size(HEIGHT, EQUAL, APP_HEIGHT) | border;
    });

    component = CatchEvent(component, [&](Event event) {
        if (event == Event::Return) {
            if (menu_selected == Command::QUIT) {
                screen.ExitLoopClosure()();
                return true;
            }
            app.parse(menu_selected);
            return true;
        }
        return false;
    });

    screen.Loop(component);
}

Element renderReplayInfo(Application& app) {
    auto status = app.getStatus();

    auto statusColor = ftxui::Color::DarkGreen;
    if (status == "In progress") {
        statusColor = ftxui::Color::DarkOrange;
    } else if (status == "Finished") {
        statusColor = ftxui::Color::IndianRed1;
    }

    return vbox({
               hbox({
                   text("File: ") | bold,
                   text(app.getFilename()) | color(Color::White),
               }),
               hbox({
                   text("State: ") | bold,
                   text(status) | bold | color(statusColor),
               }),

               hbox({
                   text("Timestamp: ") | bold,
                   text(app.getLastEventTimestamp()),
               }),

               separator(),

               text("Progress") | bold,
               gauge(app.getProgress()) | border | color(ftxui::Color::DarkGreen) |
                   size(WIDTH, EQUAL, REPLAY_INFO_TOTAL_WIDTH),

               separator(),

               text("Last Event") | bold,
               text(app.getLastEvent()) | border | center | color(Color::Yellow),
           }) |
           border | color(Color::White);
}

Element renderOrderBook(Application& app) {
    auto bids = app.getPriceLevels("BUY");
    Elements bidElements;

    for (const auto& [price, quantity] : bids) {
        bidElements.push_back(hbox({
                                  text(std::format("{:<10}", price)),
                                  text(std::format("{:>8}", quantity)),
                              }) |
                              color(Color::Green));
    }

    auto sells = app.getPriceLevels("SELL");
    Elements sellElements;

    for (const auto& [price, quantity] : sells) {
        sellElements.push_back(hbox({
                                   text(std::format("{:<10}", quantity)),
                                   text(std::format("{:>8}", price)),
                               }) |
                               color(Color::Red));
    }

    return vbox({text(" ORDER BOOK ") | bold | center,

                 separator(),

                 hbox({text(std::format("{:^{}}", "BID", ORDERBOOK_TOTAL_WIDTH / 2)) | bold,

                       separator(),

                       text(std::format("{:^{}}", "ASK", ORDERBOOK_TOTAL_WIDTH / 2)) | bold}),

                 separator(),

                 hbox({
                     vbox(bidElements) | size(WIDTH, EQUAL, ORDERBOOK_TOTAL_WIDTH / 2),

                     separator(),

                     vbox(sellElements) | size(WIDTH, EQUAL, ORDERBOOK_TOTAL_WIDTH / 2),
                 })

           }) |
           border | size(WIDTH, EQUAL, ORDERBOOK_TOTAL_WIDTH);
}