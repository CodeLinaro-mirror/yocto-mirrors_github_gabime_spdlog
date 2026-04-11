// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <filesystem>
#include <fstream>
#include <mutex>
#include <stdexcept>
#include <string>

#include "../details/log_msg.h"
#include "../details/null_mutex.h"
#include "../formatter.h"

namespace spdlog_lite::sinks {

template <typename Mutex>
class basic_file_sink {
public:
    explicit basic_file_sink(const std::filesystem::path &filename, bool truncate = false)
        : mutex_(std::make_unique<Mutex>()) {
        auto mode = std::ios::out;
        if (truncate) {
            mode |= std::ios::trunc;
        } else {
            mode |= std::ios::app;
        }

        // Create parent directories if needed
        if (auto parent = filename.parent_path(); !parent.empty()) {
            std::filesystem::create_directories(parent);
        }

        ofs_.open(filename, mode);
        if (!ofs_.is_open()) {
            throw std::runtime_error("spdlog_lite: failed to open file: " + filename.string());
        }
    }

    void log(const details::log_msg &msg) {
        std::lock_guard<Mutex> lock(*mutex_);
        buf_.clear();
        formatter_.format(msg, buf_);
        ofs_.write(buf_.data(), static_cast<std::streamsize>(buf_.size()));
    }

    void flush() {
        std::lock_guard<Mutex> lock(*mutex_);
        ofs_.flush();
    }

private:
    std::unique_ptr<Mutex> mutex_;
    simple_formatter formatter_;
    std::ofstream ofs_;
    std::string buf_;
};

using basic_file_sink_mt = basic_file_sink<std::mutex>;
using basic_file_sink_st = basic_file_sink<details::null_mutex>;

}  // namespace spdlog_lite::sinks
