#include <spdlog/sinks/stdout_color_sinks.h>
#include "log.h"

std::shared_ptr<spdlog::logger> Log::logger;


void Log::Init() {
    using wincolor_sink_ptr = std::shared_ptr<spdlog::sinks::stdout_color_sink_mt>;

    std::vector<wincolor_sink_ptr> sinks;  // pointers to sinks that support setting custom color
    sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());  // console sink
    sinks[0]->set_pattern("%^%T > [%L] %v%$");  // e.g. 23:55:59 > [I] sample message
    //sinks[0]->set_color(spdlog::level::trace, sinks[0]->CYAN);
    //sinks[0]->set_color(spdlog::level::debug, sinks[0]->BOLD);

    logger = std::make_shared<spdlog::logger>("opengl", begin(sinks), end(sinks));
    spdlog::register_logger(logger);
    logger->set_level(spdlog::level::trace);  // log level less than this will be silently ignored
    logger->flush_on(spdlog::level::trace);   // the minimum log level that will trigger automatic flush
}

void Log::Shutdown() {
    spdlog::shutdown();
}
