#include "mdtty/mdtty.hpp"

#include <array>
#include <cstdio>
#include <expected>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>

namespace {

std::expected<std::string, std::error_code> slurp(const char *path) {
  std::ifstream f(path, std::ios::binary);
  if (!f) {
    return std::unexpected(std::make_error_code(std::errc::no_such_file_or_directory));
  }
  std::ostringstream ss;
  ss << f.rdbuf();
  return ss.str();
}

} // namespace

int main(int argc, char **argv) {
  mdtty::Renderer r(auto([](std::string_view s) { std::fwrite(s.data(), 1, s.size(), stdout); }));

  if (argc > 1) {
    auto content = slurp(argv[1]);
    if (!content) {
      std::fprintf(stderr, "mdtty: %s: %s\n", argv[1], content.error().message().c_str());
      return 1;
    }
    // Stream in small chunks to exercise the line buffer across arbitrary splits.
    const std::string_view v{*content};
    constexpr std::size_t  chunk = 17;
    for (std::size_t i = 0; i < v.size(); i += chunk) {
      r.feed(v.substr(i, std::min(chunk, v.size() - i)));
    }
  } else {
    std::array<char, 4096> buf{};
    while (std::cin.read(buf.data(), buf.size()) || std::cin.gcount() > 0) {
      r.feed(std::string_view(buf.data(), static_cast<std::size_t>(std::cin.gcount())));
    }
  }
  r.flush();
  return 0;
}
