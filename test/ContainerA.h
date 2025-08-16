//
// Created by Wangsicong Wei on 2025-08-15.
//

#ifndef FUNCCIAFRAME_CONTAINERA_H
#define FUNCCIAFRAME_CONTAINERA_H
#include "Object1.h"
#include "Object2.h"
#include "../core/Resource.h"

namespace Funccia::Core {
    class ContainerA : public Resource {
    public:
        ContainerA() {
        }
        virtual ~ContainerA(){};
        auto TypeID() -> Core::TypeID override {return TypeID::ContainerA;};
        void Reset() override {};


        void Serialize(Serializer& serializer) override {serializer.write(m_Resources);};
        void Deserialize(Deserializer& deserializer) override {deserializer.read(m_Resources);};

        void Save() {
            for (auto& resource : m_Resources) {
                resource->ToString();
            }
            auto saver = Serializer("level1.bin");
            Serialize(saver);
        };
        void Load() {
            auto loader = Deserializer("level1.bin");
            Deserialize(loader);
            for (auto& resource : m_Resources) {
                resource->ToString();
            }
        };

        void AddResource(Resource* _resource) {
            m_Resources.push_back(_resource);
        }
    private:
        std::vector<Resource*> m_Resources;
    };
}

#endif //FUNCCIAFRAME_CONTAINERA_H