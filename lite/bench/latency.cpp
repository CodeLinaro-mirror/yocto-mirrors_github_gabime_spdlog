//
// Copyright(c) 2018 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

//
// latency.cpp : spdlog_lite latency benchmarks
//

#include "benchmark/benchmark.h"
#include "spdlog_lite/logger.h"
#include "spdlog_lite/sinks/basic_file_sink.h"
#include "spdlog_lite/sinks/null_sink.h"

using namespace spdlog_lite;

// Bench with a long C string (no formatting)
static void bench_null_sink_c_string(benchmark::State &state) {
    logger<sinks::null_sink_st> log("bench");
    const char *msg =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum pharetra metus cursus "
        "lacus placerat congue. Nulla egestas, mauris a tincidunt tempus, enim lectus volutpat mi, "
        "eu consequat sem "
        "libero nec massa. In dapibus ipsum a diam rhoncus gravida. Etiam non dapibus eros. Donec "
        "fringilla dui sed "
        "augue pretium, nec scelerisque est maximus. Nullam convallis, sem nec blandit maximus, "
        "nisi turpis ornare "
        "nisl, sit amet volutpat neque massa eu odio. Maecenas malesuada quam ex, posuere congue "
        "nibh turpis duis.";

    for (auto _ : state) {
        log.info(msg);
    }
}

// Bench with std::format formatting
static void bench_null_sink_formatted(benchmark::State &state) {
    logger<sinks::null_sink_st> log("bench");
    int i = 0;
    for (auto _ : state) {
        log.info("Hello logger: msg number {}...............", ++i);
    }
}

// Bench with logger disabled at runtime
static void bench_disabled_runtime(benchmark::State &state) {
    logger<sinks::null_sink_st> log("bench");
    log.set_level(level::off);
    int i = 0;
    for (auto _ : state) {
        log.info("Hello logger: msg number {}...............", ++i);
    }
}

// Bench null_sink_mt with multiple threads
static void bench_null_sink_mt(benchmark::State &state) {
    static logger<sinks::null_sink_mt> log("bench");
    int i = 0;
    for (auto _ : state) {
        log.info("Hello logger: msg number {}...............", ++i);
    }
}

// Bench basic file sink (single-threaded)
static void bench_basic_file_st(benchmark::State &state) {
    logger<sinks::basic_file_sink_st> log("bench", sinks::basic_file_sink_st{"latency_logs/basic_st.log", true});
    int i = 0;
    for (auto _ : state) {
        log.info("Hello logger: msg number {}...............", ++i);
    }
}

// Bench basic file sink (multi-threaded)
static void bench_basic_file_mt(benchmark::State &state) {
    static logger<sinks::basic_file_sink_mt> log("bench", sinks::basic_file_sink_mt{"latency_logs/basic_mt.log", true});
    int i = 0;
    for (auto _ : state) {
        log.info("Hello logger: msg number {}...............", ++i);
    }
}

int main(int argc, char *argv[]) {
    int n_threads = benchmark::CPUInfo::Get().num_cpus;

    auto full_bench = argc > 1 && std::string(argv[1]) == "full";

    benchmark::RegisterBenchmark("disabled-at-runtime", bench_disabled_runtime);
    benchmark::RegisterBenchmark("null_sink_st (500_bytes c_str)", bench_null_sink_c_string);
    benchmark::RegisterBenchmark("null_sink_st", bench_null_sink_formatted);

    if (full_bench) {
        benchmark::RegisterBenchmark("null_sink_mt", bench_null_sink_mt)->Threads(n_threads)->UseRealTime();
        benchmark::RegisterBenchmark("basic_file_st", bench_basic_file_st)->UseRealTime();
        benchmark::RegisterBenchmark("basic_file_mt", bench_basic_file_mt)->Threads(n_threads)->UseRealTime();
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
