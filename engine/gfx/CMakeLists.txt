add_library(vva_gfx STATIC
        Transform.cpp
        Transform.h
        MathTypes.h
)
add_library(vva::gfx ALIAS vva_gfx)
target_link_libraries(vva_gfx PUBLIC vva::core)   # include root comes along
vva_configure_target(vva_gfx)

add_subdirectory(vulkan)
