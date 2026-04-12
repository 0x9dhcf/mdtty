# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [0.2.0] - 2026-04-11

### Added
- GFM table rendering with box-drawing borders.
- Unicode-aware visual width: CJK and emoji are sized correctly via utf8proc
  and a vendored `widechar_width`, so table and horizontal-rule alignment
  survive wide glyphs.
- `mdtty/version.hpp` header exposing `MDTTY_VERSION_MAJOR`,
  `MDTTY_VERSION_MINOR`, `MDTTY_VERSION_PATCH` and `MDTTY_VERSION_STRING`
  for downstream consumers.

### Changed
- Build now requires GCC 15 for full C++23 `<print>` / `std::println` support
  in the example CLI; CI updated accordingly. The library itself still builds
  with older toolchains.
- The library is now built as both a static and a shared library. The shared
  library carries `VERSION` and `SOVERSION` properties, producing a proper
  `libmdtty.so` -> `libmdtty.so.0` -> `libmdtty.so.0.2.0` symlink chain.

[Unreleased]: https://github.com/0x9dhcf/mdtty/compare/v0.2.0...HEAD
[0.2.0]: https://github.com/0x9dhcf/mdtty/releases/tag/v0.2.0
