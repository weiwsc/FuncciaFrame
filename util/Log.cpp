//
// Created by Wangsicong Wei on 2026-07-30.
//

#include "Log.h"

#include <quill/Backend.h>
#include <quill/Frontend.h>
#include <quill/core/PatternFormatterOptions.h>
#include <quill/sinks/ConsoleSink.h>

namespace
{
    quill::PatternFormatterOptions formatter{
        ANSI_DIM "%(time)" ANSI_RESET
        " [%(log_level_short_code)]"
        " [" ANSI_CYAN "%(logger)" ANSI_RESET "] "
        "%(message:<65) "
        ANSI_DIM "%(short_source_location)" ANSI_RESET,
        "%H:%M:%S.%Qms"
    };

    quill::Logger* engine_logger = nullptr;
}

void Funccia::Engine::Log::init()
{
    if (engine_logger)
        return;

    quill::Backend::start();

    quill::ConsoleSinkConfig console_config;
    console_config.set_colour_mode(
        quill::ConsoleSinkConfig::ColourMode::Automatic);

    auto console_sink =
        quill::Frontend::create_or_get_sink<quill::ConsoleSink>(
            "console", console_config);

    engine_logger =
        quill::Frontend::create_or_get_logger(
            "Engine", console_sink, formatter);

    engine_logger->set_log_level(quill::LogLevel::TraceL3);
}

void Funccia::Engine::Log::shutdown()
{
    if (!engine_logger)
        return;

    engine_logger->flush_log();
    quill::Backend::stop();
    engine_logger = nullptr;
}

quill::Logger* Funccia::Engine::Log::get()
{
    return engine_logger;
}
