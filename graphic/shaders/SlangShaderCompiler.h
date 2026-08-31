//
// Created by Wangsicong Wei on 2026-08-29.
//

#pragma once
#include <vector>
#include "slang.h"
#include "slang-com-ptr.h"
#include "slang-com-helper.h"

namespace Funccia::Graphics::Shader {
    struct SlangShaderCompiler {
        static auto Create() -> SlangShaderCompiler;

        SlangShaderCompiler(Slang::ComPtr<slang::IGlobalSession> global_session,
                            Slang::ComPtr<slang::ISession> session) : global_session_(std::move(global_session)),
                                                                      session_(std::move(session)) {
        }

        SlangShaderCompiler(const SlangShaderCompiler& other) = delete;
        auto operator=(const SlangShaderCompiler& other) -> SlangShaderCompiler& = delete;

        SlangShaderCompiler(SlangShaderCompiler&& other) noexcept :
            global_session_(std::move(other.global_session_)),
            session_(std::move(other.session_)) {
        }

        auto operator=(SlangShaderCompiler&& other) noexcept -> SlangShaderCompiler& {
            global_session_ = std::move(other.global_session_);
            session_ = std::move(other.session_);
        }
        auto compile(const std::string& module_name, const std::string& entry_point) const ->std::vector<uint32_t>;
    private:
        Slang::ComPtr<slang::IGlobalSession> global_session_;
        Slang::ComPtr<slang::ISession> session_;
    };
}
