// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <array>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>

#include "../details/log_msg.h"
#include "../details/null_mutex.h"
#include "../formatter.h"

namespace spdlog_lite::sinks {

// ANSI color codes
namespace ansi_color {
constexpr std::string_view reset = "\033[m";
constexpr std::string_view bold = "\033[1m";
constexpr std::string_view red = "\033[31m";
constexpr std::string_view green = "\033[32m";
constexpr std::string_view yellow = "\033[33m";
constexpr std::string_view cyan = "\033[36m";
constexpr std::string_view white = "\033[37m";
constexpr std::string_view bold_red = "\033[1m\033[31m";
constexpr std::string_view bold_yellow = "\033[1m\033[33m";
constexpr std::string_view bold_red_on_white = "\033[1m\033[31m\033[47m";
}  // namespace ansi_color

template <typename Mutex>
class stdout_color_sink {
public:
    stdout_color_sink()
        : mutex_(std::make_unique<Mutex>()) {
        colors_[static_cast<std::size_t>(level::trace)] = ansi_color::white;
        colors_[static_cast<std::size_t>(level::debug)] = ansi_color::cyan;
        colors_[static_cast<std::size_t>(level::info)] = ansi_color::green;
        colors_[static_cast<std::size_t>(level::warn)] = ansi_color::bold_yellow;
        colors_[static_cast<std::size_t>(level::err)] = ansi_color::bold_red;
        colors_[static_cast<std::size_t>(level::critical)] = ansi_color::bold_red_on_white;
        colors_[static_cast<std::size_t>(level::off)] = ansi_color::reset;
    }

    void log(const details::log_msg &msg) {
        std::lock_guard<Mutex> lock(*mutex_);
        buf_.clear();
        formatter_.format(msg, buf_);

        // Write with color around the level name
        auto color = colors_[static_cast<std::size_t>(msg.log_level)];
        std::cout << color << buf_ << ansi_color::reset;
    }

    void flush() {
        std::lock_guard<Mutex> lock(*mutex_);
        std::cout.flush();
    }

    void set_color(level lvl, std::string_view color) {
        colors_[static_cast<std::size_t>(lvl)] = color;
    }

private:
    std::unique_ptr<Mutex> mutex_;
    simple_formatter formatter_;
    std::string buf_;
    std::array<std::string_view, levels_count> colors_{};
};

template <typename Mutex>
class stderr_color_sink {
public:
    stderr_color_sink()
        : mutex_(std::make_unique<Mutex>()) {
        colors_[static_cast<std::size_t>(level::trace)] = ansi_color::white;
        colors_[static_cast<std::size_t>(level::debug)] = ansi_color::cyan;
        colors_[static_cast<std::size_t>(level::info)] = ansi_color::green;
        colors_[static_cast<std::size_t>(level::warn)] = ansi_color::bold_yellow;
        colors_[static_cast<std::size_t>(level::err)] = ansi_color::bold_red;
        colors_[static_cast<std::size_t>(level::critical)] = ansi_color::bold_red_on_white;
        colors_[static_cast<std::size_t>(level::off)] = ansi_color::reset;
    }

    void log(const details::log_msg &msg) {
        std::lock_guard<Mutex> lock(*mutex_);
        buf_.clear();
        formatter_.format(msg, buf_);

        auto color = colors_[static_cast<std::size_t>(msg.log_level)];
        std::cerr << color << buf_ << ansi_color::reset;
    }

    void flush() {
        std::lock_guard<Mutex> lock(*mutex_);
        std::cerr.flush();
    }

    void set_color(level lvl, std::string_view color) {
        colors_[static_cast<std::size_t>(lvl)] = color;
    }

private:
    std::unique_ptr<Mutex> mutex_;
    simple_formatter formatter_;
    std::string buf_;
    std::array<std::string_view, levels_count> colors_{};
};

using stdout_color_sink_mt = stdout_color_sink<std::mutex>;
using stdout_color_sink_st = stdout_color_sink<details::null_mutex>;
using stderr_color_sink_mt = stderr_color_sink<std::mutex>;
using stderr_color_sink_st = stderr_color_sink<details::null_mutex>;

}  // namespace spdlog_lite::sinks
