//
// Created by Wangsicong Wei on 2025-09-05.
//

#ifndef FUNCCIAFRAME_SHADER_H
#define FUNCCIAFRAME_SHADER_H
#include "../../core/Resource.h"

namespace vva::gfx::GL {
    class Shader : core::Resource {
    public:
        Shader();

        virtual ~Shader();

        void initialize(const std::string &vertexPath, const std::string &fragmentPath);

        void Serialize(core::Serializer &serializer) override;

        void Deserialize(core::Deserializer &deserializer) override;

        auto TypeID() -> core::TypeID override { return core::TypeID::Shader; }

        // use/activate the shader
        void use();

        // utility uniform functions
        void setBool(const std::string &name, bool value) const;

        void setInt(const std::string &name, int value) const;

        void setFloat(const std::string &name, float value) const;

        auto getID() const -> unsigned int { return ID; }

    private:
        unsigned int ID;

        core::Asset *m_vertexShaderSource;
        core::Asset *m_fragmentShaderSource;
    };
}

#endif //FUNCCIAFRAME_SHADER_H