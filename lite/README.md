# spdlog_lite

Minimal, header-only C++20 logging library. Inspired by [spdlog](https://github.com/gabime/spdlog).

## Features
* **~400 lines** of code total. Header-only, nothing to compile.
* **C++20 `std::format`** — no external dependencies.
* **Zero virtual dispatch** — sinks are compile-time template parameters.
* **Thread-safe** — mutex-templated sinks (`_mt` / `_st` variants).

## Quick start
```c++
#include "spdlog_lite/logger.h"
#include "spdlog_lite/sinks/stdout_color_sink.h"

int main() {
    using namespace spdlog_lite;
    logger<sinks::stdout_color_sink_mt> log("app", sinks::stdout_color_sink_mt(std::cout));

    log.info("Hello {}", "world");
    log.info("Value: {}", 42);
    log.warn("Something happened");
    log.error("Failed with code {}", -1);
}
```

Output:
```
[2026-04-11 10:30:45.123] [app] [info] Hello world
[2026-04-11 10:30:45.123] [app] [info] Value: 42
[2026-04-11 10:30:45.123] [app] [warning] Something happened
[2026-04-11 10:30:45.123] [app] [error] Failed with code -1
```

## Sinks

| Sink | Header | Description |
|------|--------|-------------|
| `stdout_sink_mt` | `sinks/stdout_sink.h` | Write to stdout |
| `stderr_sink_mt` | `sinks/stdout_sink.h` | Write to stderr |
| `stdout_color_sink_mt` | `sinks/stdout_color_sink.h` | Colored stdout (ANSI) |
| `stderr_color_sink_mt` | `sinks/stdout_color_sink.h` | Colored stderr (ANSI) |
| `basic_file_sink_mt` | `sinks/basic_file_sink.h` | Write to file (`std::ofstream`) |

All sinks have `_st` (single-threaded) variants.

## Multiple sinks
```c++
#include "spdlog_lite/sinks/stdout_color_sink.h"
#include "spdlog_lite/sinks/basic_file_sink.h"

using namespace spdlog_lite;
logger<sinks::stdout_color_sink_mt, sinks::basic_file_sink_mt> log(
    "app",
    sinks::stdout_color_sink_mt(std::cout),
    sinks::basic_file_sink_mt{"logs/app.txt"});

log.info("Goes to both console and file");
```

## Log levels
```c++
log.set_level(level::trace);  // show all messages
log.set_level(level::warn);   // show only warn, error, critical
```

Levels: `trace`, `debug`, `info`, `warn`, `err`, `critical`, `off`.

## File logging
```c++
#include "spdlog_lite/sinks/basic_file_sink.h"

using namespace spdlog_lite;
logger<sinks::basic_file_sink_mt> log("file", sinks::basic_file_sink_mt{"logs/app.txt"});
log.info("Written to file");
```

Pass `true` as second argument to truncate the file on open:
```c++
sinks::basic_file_sink_mt{"logs/app.txt", true}
```

## Build
```console
$ cmake -B build .
$ cmake --build build
```

Requires a C++20 compiler (MSVC 2022, GCC 13+, Clang 16+).

## Design

```
logger<Sink1, Sink2, ...>
  │
  │  std::format(fmt, args...)     ← compile-time format check
  │  log_msg{time, name, level, payload}
  │
  ├──► Sink1.log(msg)              ← direct call, no virtual dispatch
  └──► Sink2.log(msg)              ← resolved at compile time via std::tuple
```

## spdlog_lite vs spdlog

| | spdlog | spdlog_lite |
|---|---|---|
| Code size | ~3,000 lines | ~400 lines |
| Dependencies | fmt 12.x | none |
| C++ standard | C++17 | C++20 |
| Virtual dispatch | yes | no |
| Header-only | no | yes |
| Pattern formatter | yes (1,225 lines) | fixed format |
| Sink selection | runtime (`shared_ptr`) | compile-time (templates) |
| Async logging | yes | no |
| Registry | no | no |

## License
MIT
