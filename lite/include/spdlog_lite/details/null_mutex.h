// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

namespace spdlog_lite::details {

struct null_mutex {
    void lock() noexcept {}
    void unlock() noexcept {}
};

}  // namespace spdlog_lite::details
