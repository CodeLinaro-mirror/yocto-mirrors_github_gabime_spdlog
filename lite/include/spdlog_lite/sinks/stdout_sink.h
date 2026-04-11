// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <string>

#include "../details/log_msg.h"
#include "../details/null_mutex.h"
#include "../formatter.h"

namespace spdlog_lite::sinks {

template <typename Mutex>
class stdout_sink {
public:
    stdout_sink()
        : mutex_(std::make_unique<Mutex>()) {}

    void log(const details::log_msg &msg) {
        std::lock_guard<Mutex> lock(*mutex_);
        buf_.clear();
        formatter_.format(msg, buf_);
        std::cout.write(buf_.data(), static_cast<std::streamsize>(buf_.size()));
    }

    void flush() {
        std::lock_guard<Mutex> lock(*mutex_);
        std::cout.flush();
    }

private:
    std::unique_ptr<Mutex> mutex_;
    simple_formatter formatter_;
    std::string buf_;
};

template <typename Mutex>
class stderr_sink {
public:
    stderr_sink()
        : mutex_(std::make_unique<Mutex>()) {}

    void log(const details::log_msg &msg) {
        std::lock_guard<Mutex> lock(*mutex_);
        buf_.clear();
        formatter_.format(msg, buf_);
        std::cerr.write(buf_.data(), static_cast<std::streamsize>(buf_.size()));
    }

    void flush() {
        std::lock_guard<Mutex> lock(*mutex_);
        std::cerr.flush();
    }

private:
    std::unique_ptr<Mutex> mutex_;
    simple_formatter formatter_;
    std::string buf_;
};

using stdout_sink_mt = stdout_sink<std::mutex>;
using stdout_sink_st = stdout_sink<details::null_mutex>;
using stderr_sink_mt = stderr_sink<std::mutex>;
using stderr_sink_st = stderr_sink<details::null_mutex>;

}  // namespace spdlog_lite::sinks
