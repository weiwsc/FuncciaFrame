add_library(vva_core STATIC
        Log.cpp
        Log.h
        FileWatcher.h
        FpsCounter.cpp
        FpsCounter.h
        CpuProfiler.h
)
add_library(vva::core ALIAS vva_core)

target_include_directories(vva_core PUBLIC ${CMAKE_SOURCE_DIR}/engine)
target_link_libraries(vva_core PUBLIC quill::quill glm::glm)
target_compile_definitions(vva_core PUBLIC QUILL_DISABLE_NON_PREFIXED_MACROS)

# Log.h uses __VA_OPT__; consumers need MSVC's conforming preprocessor too.
if(MSVC)
    target_compile_options(vva_core PUBLIC /Zc:preprocessor)
endif()
vva_configure_target(vva_core)
