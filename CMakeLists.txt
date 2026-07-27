cmake_minimum_required(VERSION 3.25)
project(MarketReplay VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_executable(
    main
    src/main.cpp
    src/ReplayEngine.cpp
    src/Application.cpp
    src/Parser.cpp
    src/MarketEvent.cpp
    src/OrderBook.cpp
    src/Renderer.cpp
)
target_include_directories(main PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
# Testing
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  tests
  tests/orderbook/AddOrderTests.cpp
  src/OrderBook.cpp
  tests/orderbook/CancelOrderTests.cpp
)

target_include_directories(tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
  tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

# Terminal UI
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v7.0.0
)
 
FetchContent_MakeAvailable(ftxui)
target_link_libraries(main
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)