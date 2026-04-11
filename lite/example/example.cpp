// spdlog_lite example

#include "spdlog_lite/logger.h"
#include "spdlog_lite/sinks/basic_file_sink.h"
#include "spdlog_lite/sinks/stdout_sink.h"

int main() {
    using namespace spdlog_lite;

    // Single sink — stdout
    logger<sinks::stdout_sink_mt> console("app");
    console.info("Hello {}", "world");
    console.info("Value: {}", 42);
    console.debug("This should not appear (level is info)");

    console.set_level(level::trace);
    console.trace("Now trace is visible");

    // File sink
    logger<sinks::basic_file_sink_mt> file_logger("file", sinks::basic_file_sink_mt{"logs/example.txt", true});
    file_logger.info("Written to file");
    file_logger.error("Error written to file: {}", 404);

    // Multiple sinks — both stdout and file
    logger<sinks::stdout_sink_mt, sinks::basic_file_sink_mt> multi(
        "multi", sinks::stdout_sink_mt{}, sinks::basic_file_sink_mt{"logs/multi.txt", true});
    multi.info("Goes to both console and file");
    multi.warn("Warning: {}", "something happened");

    // String view overload (no formatting)
    console.info("Plain string, no formatting");

    return 0;
}
