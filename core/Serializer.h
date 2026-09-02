//
// Created by Wangsicong Wei on 2025-08-15.
//

#ifndef FUNCCIAFRAME_SERIALIZER_H
#define FUNCCIAFRAME_SERIALIZER_H
#include <fstream>
#include <ostream>
#include <concepts>
#include <type_traits>

#include "ISerializable.h"
#include "TypeRegistry.h"
class Resource;
#include "Asset.h"

namespace vva::core {
    using std::byte, std::string;

    //===========> template for containers with pointers of Resource types <================
    template<typename T, typename Base>
    concept DerivedPointer = std::is_pointer_v<T> &&
                        std::is_base_of_v<Base, std::remove_pointer_t<T>>;

    template<class R, class Base>
    concept PointerIterableContainer =
        std::ranges::range<R> &&
        std::is_pointer_v<std::remove_cvref_t<std::ranges::range_value_t<R>>> &&
        std::is_base_of_v<
            Base,
            std::remove_cv_t<std::remove_pointer_t<
                std::remove_cvref_t<std::ranges::range_value_t<R>>
            >>
        >;

    template<typename R>
    concept PrimitiveIterableContainer =
        std::ranges::range<R> &&
        std::is_arithmetic_v<std::remove_cvref_t<std::ranges::range_value_t<R>>>;

    //=============> Serializer class <=================
    class Serializer {
    public:
        explicit Serializer(const string& _file_name);
        virtual ~Serializer();
        void Close();

        //==========> primitive types <=================
        template<class T>
        requires std::is_arithmetic_v<T>
        auto write(const T& var) -> Serializer&;

        template<PrimitiveIterableContainer Container>
        auto write(const Container& _container) -> Serializer&;
        /*
        auto write(const int& var) -> Serializer&;
        auto write(const uint32_t &var) -> Serializer&;
        auto write(const float &var) -> Serializer&;
        auto write(const bool &var) -> Serializer&;
        auto write(const char &var) -> Serializer&;
        */

        auto write(const byte &var) -> Serializer&;
        auto write(const std::string &var) -> Serializer&;
        auto write(const TypeID &var) -> Serializer&;


        //============> Pointer types <=============
        template<class T>
        requires std::derived_from<T, ISerializable>
        auto write(T *_pointer) -> Serializer&;

        template<PointerIterableContainer<ISerializable> Container>
        auto write(const Container &_container) -> Serializer&;

        auto write(Asset *_asset) -> Serializer&;

    private:
        std::ofstream _stream;
    };

    template<class T> requires std::is_arithmetic_v<T>
    auto Serializer::write(const T &var) -> Serializer & {
        _stream.write(reinterpret_cast<const char*>(&var), sizeof(var));
        return *this;
    }

    template<PrimitiveIterableContainer Container>
    auto Serializer::write(const Container &_container) -> Serializer & {
        write(static_cast<std::uint32_t>(_container.size()));
        for (auto& var : _container) {
            write(var);
        }
        return *this;
    }

    template<class T> requires std::derived_from<T, ISerializable>
    auto Serializer::write(T *_pointer) -> Serializer& {
        byte exists {1} ;
        if (_pointer != nullptr) {
            write(exists);
            write(_pointer->TypeID());
            _pointer->Serialize(*this);
        }
        else {
            exists = std::byte{0};
            write(exists);
        }
        return *this;
    }

    template<PointerIterableContainer<ISerializable> Container>
    auto Serializer::write(const Container& _container) -> Serializer& {
        auto size = static_cast<std::uint32_t>(_container.size());
        write(size);
        for (auto& resource : _container) {
            write(resource);
        }
        return *this;
    }

    //=============> Deserializer class <=================
    class Deserializer {
    public:
        explicit Deserializer(const string& _file_name);
        virtual ~Deserializer();
        void Close();

        //============>primitive types <================
        template<class T> requires std::is_arithmetic_v<T>
        auto read(T& var) -> Deserializer&;

        template<PrimitiveIterableContainer Container>
        auto read(Container& _container) -> Deserializer&;
        /*
        auto read(int& var) -> Deserializer&;
        auto read(uint32_t &var) -> Deserializer&;
        auto read(float &var) -> Deserializer&;
        auto read(bool &var) -> Deserializer&;
        auto read(char &var) -> Deserializer&;
        */

        auto read(byte &var) -> Deserializer&;
        auto read(std::string &var) -> Deserializer&;
        auto read(TypeID &var) -> Deserializer&;

        template<class T>
        requires std::derived_from<T, ISerializable>
        auto read(T *&_pointer) -> Deserializer&;

        template<PointerIterableContainer<ISerializable> Container>
        auto read(Container &_container) -> Deserializer&;

        auto read(Asset *&_asset) -> Deserializer&;
    private:
        std::ifstream _stream;
    };

    template<class T> requires std::is_arithmetic_v<T>
    auto Deserializer::read(T &var) -> Deserializer & {
        _stream.read(reinterpret_cast<char*>(&var), sizeof(var));
        return *this;
    }

    template<PrimitiveIterableContainer Container>
    auto Deserializer::read(Container &_container) -> Deserializer & {
        std::uint32_t size {};
        read(size);
        _container.clear();
        _container.resize(size);
        for (auto& var : _container) {
            read(var);
        }
        return *this;
    }

    template<class T> requires std::derived_from<T, ISerializable>
    auto Deserializer::read(T *&_pointer) -> Deserializer & {
        std::byte exists{0};
        read(exists);
        if (static_cast<int>(exists) == 1) {
            TypeID typeID;
            read(typeID);
            _pointer = static_cast<T*>(TypeRegistry::GetTypeDefaultObject(typeID));
            if (_pointer) {
                _pointer->Deserialize(*this);
            }
        } else {
            _pointer = nullptr;
        }
        return *this;
    }

    template<PointerIterableContainer<ISerializable> Container>
    auto Deserializer::read(Container &_container) -> Deserializer & {
        std::uint32_t size {};
        read(size);
        _container.clear();
        for (std::uint32_t i = 0; i < size; ++i) {
            typename Container::value_type resource = nullptr;
            read(resource);
            _container.push_back(resource);
        }
        return *this;
    }
}


#endif //FUNCCIAFRAME_SERIALIZER_H