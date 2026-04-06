# mdtty

![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)
![build](https://github.com/0x9dhcf/mdtty/actions/workflows/ci.yml/badge.svg)
![license](https://img.shields.io/badge/license-MIT-green.svg)

Streaming markdown → ANSI terminal renderer in C++23. Standard library only,
zero external dependencies. Designed to sit in front of an LLM token stream
and style markdown on the fly without tearing output on arbitrary chunk
boundaries.

## Features

- **Streaming-safe**: line-buffered state machine tolerates chunks that split
  mid-word, mid-span, or mid-line
- **Block elements**: fenced code blocks (with language label), headings,
  blockquotes, unordered and ordered lists, horizontal rules
- **Inline spans**: bold (`**` / `__`), italic (`*` / `_`), inline code
  (`` ` ``), backslash escapes
- **TTY-aware**: auto-detects non-TTY sinks and strips ANSI so piped or
  logged output stays clean
- **Reusable**: `reset()` clears buffered state for the next response
- **One-shot helper**: `Renderer::render()` returns a styled `std::string`

## Dependencies

- C++23 compiler (GCC 14+, Clang 18+)
- CMake 3.21+

No external library dependencies.

## Build

```sh
cmake --preset debug
cmake --build --preset debug

cmake --preset release
cmake --build --preset release
```

## Install

```sh
cmake --preset release
cmake --build --preset release
cmake --install build/release --prefix /usr/local
```

## Usage

### Streaming

```cpp
#include "mdtty/mdtty.hpp"
#include <cstdio>

mdtty::Renderer r([](std::string_view s) {
  std::fwrite(s.data(), 1, s.size(), stdout);
});

r.feed("# Hel");
r.feed("lo\n\n**bold** and ");
r.feed("*italic*\n");
r.flush();
```

### One-shot

```cpp
std::string styled = mdtty::Renderer::render("## Title\n\n- a\n- b\n");
```

### Custom palette

```cpp
mdtty::Config cfg;
cfg.heading = "\033[1;35m"; // bright magenta
cfg.bold    = "\033[1;97m";
mdtty::Renderer r(sink, cfg);
```

### Demo CLI

```sh
./build/debug/examples/example < README.md
./build/debug/examples/example path/to/file.md
```

## API Overview

| Header | Description |
|--------|-------------|
| `mdtty/mdtty.hpp` | `Config`, `Renderer` |

| Type | Purpose |
|------|---------|
| `Config` | ANSI escape palette, `strip_ansi`, `terminal_width` |
| `Renderer` | Streaming renderer: `feed`, `flush`, `reset`, static `render` / `is_tty` |

## License

[MIT](LICENSE)
