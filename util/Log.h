//
// Created by Wangsicong Wei on 2026-07-30.
//

#pragma once

#include <quill/Logger.h>
#include <quill/LogMacros.h>

namespace Funccia::Engine::Log {
    void init();
    void shutdown();
    quill::Logger* get();
};

#define vva_log_trace(...) \
    QUILL_LOG_TRACE_L1(::Funccia::Engine::Log::get(), __VA_ARGS__)

#define fe_log_debug(...) \
    QUILL_LOG_DEBUG(::Funccia::Engine::Log::get(), __VA_ARGS__)

#define vva_log_info(...) \
    QUILL_LOG_INFO(::Funccia::Engine::Log::get(), __VA_ARGS__)

#define vva_log_warning(...) \
    QUILL_LOG_WARNING(::Funccia::Engine::Log::get(), __VA_ARGS__)

#define vva_log_error(...) \
    QUILL_LOG_ERROR(::Funccia::Engine::Log::get(), __VA_ARGS__)

#define vva_log_critical(...) \
    QUILL_LOG_CRITICAL(::Funccia::Engine::Log::get(), __VA_ARGS__)