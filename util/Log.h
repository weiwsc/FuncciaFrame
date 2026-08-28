//
// Created by Wangsicong Wei on 2026-07-30.
//

#pragma once

#include <quill/Logger.h>
#include <quill/LogMacros.h>
#define ANSI_DIM          "\033[2m"
#define ANSI_CYAN         "\033[36m"
#define ANSI_YELLOW       "\033[33m"
#define ANSI_RED          "\033[31m"
#define ANSI_BRIGHT_RED   "\033[1;91m"
#define ANSI_RESET        "\033[0m"
namespace Funccia::Engine::Log {
    void init();
    void shutdown();
    quill::Logger* get();
};

#define vva_log_trace(fmt, ...) \
    QUILL_LOG_TRACE_L1(          \
        ::Funccia::Engine::Log::get(), \
        ANSI_DIM fmt ANSI_RESET __VA_OPT__(,) __VA_ARGS__)

#define vva_log_debug(fmt, ...) \
    QUILL_LOG_DEBUG(            \
        ::Funccia::Engine::Log::get(), \
        ANSI_DIM fmt ANSI_RESET __VA_OPT__(,) __VA_ARGS__)

#define vva_log_info(...) \
    QUILL_LOG_INFO(::Funccia::Engine::Log::get(), __VA_ARGS__)

#define vva_log_warning(fmt, ...) \
    QUILL_LOG_WARNING(             \
        ::Funccia::Engine::Log::get(), \
        ANSI_YELLOW fmt ANSI_RESET __VA_OPT__(,) __VA_ARGS__)

#define vva_log_error(fmt, ...) \
    QUILL_LOG_ERROR(            \
        ::Funccia::Engine::Log::get(), \
        ANSI_RED fmt ANSI_RESET __VA_OPT__(,) __VA_ARGS__)

#define vva_log_critical(fmt, ...) \
    QUILL_LOG_CRITICAL(             \
        ::Funccia::Engine::Log::get(), \
        ANSI_BRIGHT_RED fmt ANSI_RESET __VA_OPT__(,) __VA_ARGS__)
