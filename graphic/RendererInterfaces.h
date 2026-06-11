//
// Created by Codex on 2026-06-10.
//

#ifndef FUNCCIAFRAME_RENDERERINTERFACES_H
#define FUNCCIAFRAME_RENDERERINTERFACES_H

#include "RenderTypes.h"

namespace Funccia::Graphic {
    class IUiRenderer {
    public:
        virtual ~IUiRenderer() = default;

        virtual auto Initialize(const std::string& vertexShaderPath,
                                const std::string& fragmentShaderPath) -> bool = 0;
        virtual auto BeginFrame() -> void = 0;
        virtual auto Submit(const UiDrawRect& rect) -> void = 0;
        virtual auto Render(const glm::vec2& screenSize) -> void = 0;
    };

    class ITextRenderer {
    public:
        virtual ~ITextRenderer() = default;

        virtual auto Initialize(const std::string& vertexShaderPath,
                                const std::string& fragmentShaderPath) -> bool = 0;
        virtual auto BeginFrame() -> void = 0;
        virtual auto MeasureText(const TextDrawRequest& request) -> float = 0;
        virtual auto SubmitText(const TextDrawRequest& request) -> void = 0;
        virtual auto Render(const glm::vec2& screenSize) -> void = 0;
    };
}

#endif //FUNCCIAFRAME_RENDERERINTERFACES_H
