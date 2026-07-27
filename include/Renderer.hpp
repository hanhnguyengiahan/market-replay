#pragma once
#include "Application.hpp"
#include <ftxui/dom/elements.hpp>

constexpr int APP_WIDTH = 130;
constexpr int ORDERBOOK_TOTAL_WIDTH = 40;
constexpr int REPLAY_INFO_TOTAL_WIDTH = 80;
constexpr int MENU_TOTAL_WIDTH = 10;

constexpr int APP_HEIGHT = 15;

ftxui::Element renderReplayInfo(Application& app);
ftxui::Element renderOrderBook(Application& app);
ftxui::Element renderControls(Application& app);
void renderApp(Application& app);