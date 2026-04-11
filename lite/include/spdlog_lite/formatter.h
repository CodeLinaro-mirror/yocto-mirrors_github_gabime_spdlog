// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <chrono>
#include <format>
#include <string>

#include "details/log_msg.h"

namespace spdlog_lite {

// Simple fixed-format formatter.
// Output: [2024-01-15 10:30:45.123] [logger_name] [info] message\n
struct simple_formatter {
    void format(const details::log_msg &msg, std::string &dest) {
        auto tp = std::chrono::floor<std::chrono::milliseconds>(msg.time);
        std::format_to(std::back_inserter(dest), "[{:%Y-%m-%d %H:%M:%S}] [{}] [{}] {}\n",
                       tp, msg.logger_name, to_string_view(msg.log_level), msg.payload);
    }
};

}  // namespace spdlog_lite
