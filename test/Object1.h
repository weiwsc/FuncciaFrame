//
// Created by Wangsicong Wei on 2025-08-15.
//

#ifndef FUNCCIAFRAME_OBJECT1_H
#define FUNCCIAFRAME_OBJECT1_H
#include "../core/Resource.h"

namespace Funccia::Core {
    class Object1 : public Resource{
    public:
        Object1(){m_name = "Object1";};

        ~Object1() override {};

        void Reset() override {};
        auto TypeID() -> Core::TypeID override {return TypeID::Object1;}

        auto GetName() const -> string { return m_name; }

        void SetName(const string &_name) { m_name = _name; }

        void ToString() override {std::cout << m_name << std::endl;}

        void Serialize(Serializer& serializer) override {serializer.write(m_name);};
        void Deserialize(Deserializer& deserializer) override {deserializer.read(m_name);};
    private:
        string m_name;
    };
}

#endif //FUNCCIAFRAME_OBJECT1_H