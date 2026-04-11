// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <array>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>

namespace spdlog_lite {

using log_clock = std::chrono::system_clock;
using string_view_t = std::string_view;

enum class level : std::uint8_t {
    trace = 0,
    debug = 1,
    info = 2,
    warn = 3,
    err = 4,
    critical = 5,
    off = 6,
    n_levels = 7
};

using atomic_level_t = std::atomic<level>;

constexpr auto levels_count = static_cast<std::size_t>(level::n_levels);

constexpr std::array<std::string_view, levels_count> level_string_views{
    "trace", "debug", "info", "warning", "error", "critical", "off"};

constexpr std::array<std::string_view, levels_count> short_level_names{
    "T", "D", "I", "W", "E", "C", "O"};

[[nodiscard]] constexpr std::string_view to_string_view(level lvl) noexcept {
    return level_string_views[static_cast<std::size_t>(lvl)];
}

[[nodiscard]] constexpr std::string_view to_short_string_view(level lvl) noexcept {
    return short_level_names[static_cast<std::size_t>(lvl)];
}

}  // namespace spdlog_lite
