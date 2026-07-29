#pragma once
#include "Application.hpp"
#include <ftxui/dom/elements.hpp>

constexpr int APP_WIDTH = 100;
constexpr int ORDERBOOK_TOTAL_WIDTH = 50;
constexpr int REPLAY_INFO_TOTAL_WIDTH = 40;
constexpr int MENU_TOTAL_WIDTH = 10;

constexpr int APP_HEIGHT = 17;

ftxui::Element renderReplayInfo(Application& app);
ftxui::Element renderOrderBook(Application& app);
ftxui::Element renderControls(Application& app);
void renderApp(Application& app);