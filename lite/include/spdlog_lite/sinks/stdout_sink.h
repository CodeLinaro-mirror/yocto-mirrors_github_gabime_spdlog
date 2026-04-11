// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <cstdio>
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
    explicit stdout_sink(std::FILE *file = stdout)
        : mutex_(std::make_unique<Mutex>()),
          file_(file) {}

    void log(const details::log_msg &msg) {
        std::lock_guard<Mutex> lock(*mutex_);
        buf_.clear();
        formatter_.format(msg, buf_);
        std::fwrite(buf_.data(), 1, buf_.size(), file_);
    }

    void flush() {
        std::lock_guard<Mutex> lock(*mutex_);
        std::fflush(file_);
    }

private:
    std::unique_ptr<Mutex> mutex_;
    std::FILE *file_;
    simple_formatter formatter_;
    std::string buf_;
};

struct stdout_sink_mt : stdout_sink<std::mutex> {
    stdout_sink_mt() : stdout_sink(stdout) {}
};
struct stdout_sink_st : stdout_sink<details::null_mutex> {
    stdout_sink_st() : stdout_sink(stdout) {}
};
struct stderr_sink_mt : stdout_sink<std::mutex> {
    stderr_sink_mt() : stdout_sink(stderr) {}
};
struct stderr_sink_st : stdout_sink<details::null_mutex> {
    stderr_sink_st() : stdout_sink(stderr) {}
};

}  // namespace spdlog_lite::sinks
