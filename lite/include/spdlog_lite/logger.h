// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <format>
#include <string>
#include <tuple>
#include <utility>

#include "common.h"
#include "details/log_msg.h"

namespace spdlog_lite {

template <typename... Sinks>
class logger {
public:
    // Construct with name only — sinks are default-constructed
    explicit logger(std::string name)
        : name_(std::move(name)) {}

    // Construct with name and explicit sink instances
    explicit logger(std::string name, Sinks &&...sinks)
        : name_(std::move(name)),
          sinks_{std::forward<Sinks>(sinks)...} {}

    // std::format overloads
    template <typename... Args>
    void trace(std::format_string<Args...> fmt, Args &&...args) noexcept {
        log_(level::trace, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(std::format_string<Args...> fmt, Args &&...args) noexcept {
        log_(level::debug, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(std::format_string<Args...> fmt, Args &&...args) noexcept {
        log_(level::info, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(std::format_string<Args...> fmt, Args &&...args) noexcept {
        log_(level::warn, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(std::format_string<Args...> fmt, Args &&...args) noexcept {
        log_(level::err, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void critical(std::format_string<Args...> fmt, Args &&...args) noexcept {
        log_(level::critical, fmt, std::forward<Args>(args)...);
    }

    // string_view overloads (no formatting)
    void trace(string_view_t msg) noexcept { log_(level::trace, msg); }
    void debug(string_view_t msg) noexcept { log_(level::debug, msg); }
    void info(string_view_t msg) noexcept { log_(level::info, msg); }
    void warn(string_view_t msg) noexcept { log_(level::warn, msg); }
    void error(string_view_t msg) noexcept { log_(level::err, msg); }
    void critical(string_view_t msg) noexcept { log_(level::critical, msg); }

    [[nodiscard]] bool should_log(level msg_level) const noexcept {
        return msg_level >= level_.load(std::memory_order_relaxed);
    }

    void set_level(level lvl) noexcept { level_.store(lvl, std::memory_order_relaxed); }
    [[nodiscard]] level log_level() const noexcept { return level_.load(std::memory_order_relaxed); }
    [[nodiscard]] const std::string &name() const noexcept { return name_; }

    void flush() noexcept {
        std::apply([](auto &...s) { (s.flush(), ...); }, sinks_);
    }

private:
    std::string name_;
    atomic_level_t level_{level::info};
    std::tuple<Sinks...> sinks_;

    void log_(level lvl, string_view_t msg) noexcept {
        if (!should_log(lvl)) return;
        details::log_msg log_msg(name_, lvl, msg);
        sink_it_(log_msg);
    }

    template <typename... Args>
    void log_(level lvl, std::format_string<Args...> fmt, Args &&...args) noexcept {
        if (!should_log(lvl)) return;
        try {
            auto formatted = std::format(fmt, std::forward<Args>(args)...);
            details::log_msg log_msg(name_, lvl, formatted);
            sink_it_(log_msg);
        } catch (...) {
            // swallow formatting errors
        }
    }

    void sink_it_(const details::log_msg &msg) noexcept {
        try {
            std::apply([&msg](auto &...s) { (s.log(msg), ...); }, sinks_);
        } catch (...) {
            // swallow sink errors
        }
    }
};

}  // namespace spdlog_lite
