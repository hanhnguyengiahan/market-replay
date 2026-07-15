# Market Replay

A multithreaded market replay engine written in modern C++20.

The application replays historical market events from a CSV file, allowing users to play, pause, step through, and reset the replay from an interactive terminal interface.

## Features

- Load market events from a CSV file
- Interactive terminal UI (FTXUI)
- Replay controls
  - Play
  - Pause
  - Step
  - Reset
  - Quit
- Multithreaded replay engine
- Replay progress tracking
- Current replay timestamp
- Current replay state
- Last processed event display

The replay engine is independent of the user interface, allowing different frontends (CLI, TUI, GUI) to be built on top of the same core engine.


## Building

```bash
mkdir build
cd build

cmake ..
cmake --build .
```

## Running

specify a dataset:

```bash
./market_replay data/events.csv
```


## Controls

| Command | Description |
|---------|-------------|
| Play | Begin replay |
| Pause | Pause replay |
| Step | Process one event |
| Reset | Return to the beginning |
| Quit | Exit the application |

## Future Improvements

- Playback speed control
- Seek to timestamp
- Progress bar
- Limit order book reconstruction
- Best bid / ask visualization
- Multiple market data formats
- Benchmarking
- Unit tests
- Qt frontend

## Example
