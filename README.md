# Market Replay

A multithreaded market replay engine written in modern C++20.

The application replays historical market events from a CSV file, allowing users to play, pause, step through, and reset the replay from an interactive terminal interface.

## Features

- Load market events from a CSV file
- Interactive terminal UI (FTXUI) with replay controls
  - Play: replaying the market events
  - Pause: pause the replay at any point to further inspect event or order
  - Step: step one step forward to the next available event
  - Reset: go back to the first event
  - Quit

The replay engine is independent of the user interface, allowing different frontends (CLI, TUI, GUI) to be built on top of the same core engine.

## Building

```bash
cmake -B build
```

## Running

specify a dataset:

```bash
./build/main data/sample_events.csv
```

## Controls

| Command | Description |
|---------|-------------|
| Play | Begin replay |
| Pause | Pause replay |
| Step | Process one event |
| Reset | Return to the beginning |
| Quit | Exit the application |

## TODO List

### Control features
- Playback speed control
- Seek to timestamp

### Expansion
- Display the limit order book instead of events for further inspection (diff, PnL, etc) of relevant orders. 
- Best bid / ask visualization
- Multiple market data formats (different file formats, unordered arrival time events, etc)

### Others
- Unit tests
- Qt frontend
