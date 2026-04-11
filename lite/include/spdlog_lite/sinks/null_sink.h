// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <mutex>

#include "../details/log_msg.h"
#include "../details/null_mutex.h"

namespace spdlog_lite::sinks {

template <typename Mutex>
class null_sink {
public:
    void log(const details::log_msg &) {}
    void flush() {}
};

using null_sink_mt = null_sink<std::mutex>;
using null_sink_st = null_sink<details::null_mutex>;

}  // namespace spdlog_lite::sinks
