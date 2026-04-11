// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include "../common.h"

namespace spdlog_lite::details {

struct log_msg {
    log_clock::time_point time;
    string_view_t logger_name;
    level log_level{level::off};
    string_view_t payload;

    log_msg() = default;

    log_msg(log_clock::time_point log_time, string_view_t name, level lvl, string_view_t msg)
        : time(log_time),
          logger_name(name),
          log_level(lvl),
          payload(msg) {}

    log_msg(string_view_t name, level lvl, string_view_t msg)
        : time(log_clock::now()),
          logger_name(name),
          log_level(lvl),
          payload(msg) {}
};

}  // namespace spdlog_lite::details
