//
// Created by Wangsicong Wei on 2025-08-15.
//

#ifndef FUNCCIAFRAME_OBJECT1_H
#define FUNCCIAFRAME_OBJECT1_H
#include "../core/Resource.h"

namespace Funccia::Core {

    // Assume you already have:
    // struct Serializer { template<class T> Serializer& write(const T&); /*...*/ };
    // struct Deserializer { template<class T> Deserializer& read(T&); /*...*/ };
    // struct Resource { virtual ~Resource() = default; virtual void Serialize(Serializer&){}; virtual void Deserialize(Deserializer&){}; };
    // enum class TypeID { Object1 /* ... */ };

    class Object1 : public Resource {
    public:
        Object1() { m_name = "Object1"; }

        ~Object1() override = default;

        void Reset() override {
            m_name = "Object1";
            m_id = 0;
            m_hp = 100.0f;
            m_alive = true;
            //m_pos = {0.f, 0.f, 0.f};
            m_scores.clear();
        }

        auto TypeID() -> Core::TypeID override { return TypeID::Object1; }

        auto GetName() const -> std::string { return m_name; }
        void SetName(const std::string& _name) { m_name = _name; }

        void ToString() override {
            std::cout << "Object1{"
                      << "name=\"" << m_name << "\", "
                      << "id=" << m_id << ", "
                      << "hp=" << m_hp << ", "
                      << "alive=" << std::boolalpha << m_alive << ", "
                      //<< "pos=[" << m_pos[0] << "," << m_pos[1] << "," << m_pos[2] << "], "
                      << "scores=(";
            for (size_t i = 0; i < m_scores.size(); ++i) {
                std::cout << m_scores[i];
                if (i + 1 < m_scores.size()) std::cout << ",";
            }
            std::cout << ") }\n";
        }

        // ---- serialization ----
        void Serialize(Serializer& serializer) override {
            // NOTE: keep the same order for Deserialize
            serializer.write(m_name);           // string
            serializer.write(m_id);             // i32
            serializer.write(m_hp);             // f32
            serializer.write(m_alive);          // bool (your write(bool) should use u8 on wire)
            //serializer.write(m_pos);            // std::array<float,3> - your write(array<T,N>) overload
            serializer.write(m_scores);         // std::vector<i32>   - your write(vector<T>) overload
            Resource::Serialize(serializer);    // base last (or first—just mirror in Deserialize)
        }

        void Deserialize(Deserializer& deserializer) override {
            deserializer.read(m_name);
            deserializer.read(m_id);
            deserializer.read(m_hp);
            deserializer.read(m_alive);
            //deserializer.read(m_pos);
            deserializer.read(m_scores);
            Resource::Deserialize(deserializer);
        }

        // For the test
        bool Equals(const Object1& rhs) const {
            return m_name == rhs.m_name &&
                   m_id == rhs.m_id &&
                   m_hp == rhs.m_hp &&
                   m_alive == rhs.m_alive &&
                   //m_pos == rhs.m_pos &&
                   m_scores == rhs.m_scores;
        }

        // Sample setters to populate test data
        void SetId(std::int32_t id) { m_id = id; }
        void SetHp(float hp) { m_hp = hp; }
        void SetAlive(bool a) { m_alive = a; }
        //void SetPos(float x, float y, float z) { m_pos = {x,y,z}; }
        void SetScores(std::vector<std::int32_t> v) { m_scores = std::move(v); }

    private:
        std::string m_name;

        // Added fields to exercise serializer paths
        std::int32_t              m_id{0};
        float                     m_hp{100.f};
        bool                      m_alive{true};
        //std::array<float, 3> m_pos{0.f, 0.f, 0.f};
        std::vector<std::int32_t> m_scores {1, 2, 4};
    };

}


#endif //FUNCCIAFRAME_OBJECT1_H