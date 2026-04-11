// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <cstdio>
#include <filesystem>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>

#include "../details/log_msg.h"
#include "../details/null_mutex.h"
#include "../formatter.h"

namespace spdlog_lite::sinks {

namespace detail {
struct file_closer {
    void operator()(std::FILE *f) const {
        if (f) std::fclose(f);
    }
};
}  // namespace detail

template <typename Mutex>
class basic_file_sink {
public:
    explicit basic_file_sink(const std::filesystem::path &filename, bool truncate = false)
        : mutex_(std::make_unique<Mutex>()) {
        // Create parent directories if needed
        if (auto parent = filename.parent_path(); !parent.empty()) {
            std::filesystem::create_directories(parent);
        }

        const auto *mode = truncate ? "wb" : "ab";
#ifdef _WIN32
        file_.reset(_wfopen(filename.c_str(), std::filesystem::path(mode).c_str()));
#else
        file_.reset(std::fopen(filename.c_str(), mode));
#endif
        if (!file_) {
            throw std::runtime_error("spdlog_lite: failed to open file: " + filename.string());
        }
    }

    void log(const details::log_msg &msg) {
        std::lock_guard<Mutex> lock(*mutex_);
        buf_.clear();
        formatter_.format(msg, buf_);
        std::fwrite(buf_.data(), 1, buf_.size(), file_.get());
    }

    void flush() {
        std::lock_guard<Mutex> lock(*mutex_);
        std::fflush(file_.get());
    }

private:
    std::unique_ptr<Mutex> mutex_;
    std::unique_ptr<std::FILE, detail::file_closer> file_;
    simple_formatter formatter_;
    std::string buf_;
};

using basic_file_sink_mt = basic_file_sink<std::mutex>;
using basic_file_sink_st = basic_file_sink<details::null_mutex>;

}  // namespace spdlog_lite::sinks
