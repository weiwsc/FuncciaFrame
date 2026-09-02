add_library(vva_platform STATIC
        SDLWindow.cpp
        SDLWindow.h
        WindowInterface.h
        RenderBackend.h
        MouseState.cpp
        MouseState.h
)
add_library(vva::platform ALIAS vva_platform)

target_link_libraries(vva_platform
        PUBLIC vva::core
               SDL3::SDL3          # SDLWindow.h includes <SDL3/SDL.h>; make PRIVATE once the header hides SDL
)
vva_configure_target(vva_platform)
