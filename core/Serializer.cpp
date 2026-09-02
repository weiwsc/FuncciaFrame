//
// Created by Wangsicong Wei on 2025-08-15.
//
#include <fstream>
#include <sstream>
#include "Serializer.h"

#include "AssetController.h"
#include "Resource.h"

namespace vva::core {
    Serializer::Serializer(const string& _file_name): _stream(_file_name, std::ios::out | std::ios::binary) {
    }

    Serializer::~Serializer() {
        Close();
    }

    void Serializer::Close() {
        if (_stream.is_open()) {
            _stream.close();
        }
    }


    auto Serializer::write(const std::string &var) -> Serializer& {
        auto length = static_cast<std::uint32_t>(var.size());
        _stream.write(reinterpret_cast<const char*>(&length), sizeof(length));
        if (length > 0) {
            _stream.write(var.data(), length);
        }
        return *this;
    }


    auto Serializer::write(const byte &var) -> Serializer & {
        _stream.write(reinterpret_cast<const char*>(&var), sizeof(var));
        return *this;
    }

    auto Serializer::write(const TypeID &var) -> Serializer & {
        _stream.write(reinterpret_cast<const char*>(&var), sizeof(var));
        return *this;
    }


    auto Serializer::write(Asset* _asset) -> Serializer& {
        if (_asset != nullptr) {
            const auto& guid = _asset->GetGUID();
            auto guidLength = static_cast<std::uint32_t>(guid.length());
            write(guidLength);
            if (guidLength > 0) {
                _stream.write(guid.c_str(), static_cast<std::streamsize>(guidLength));
            }
        } else {
            write(std::uint32_t{0});
        }
        return *this;
    }

    Deserializer::Deserializer(const string &_file_name): _stream(_file_name, std::ios::in | std::ios::binary) {
    }

    Deserializer::~Deserializer() {
        Close();
    }

    void Deserializer::Close() {
        if (_stream.is_open()) {
            _stream.close();
        }
    }

    auto Deserializer::read(std::string &var) -> Deserializer & {
        std::uint32_t length;
        read(length);
        var.resize(length);
        if (length > 0) {
            _stream.read(&var[0], length);
        }
        return *this;
    }

    auto Deserializer::read(byte &var) -> Deserializer & {
        _stream.read(reinterpret_cast<char*>(&var), sizeof(var));
        return *this;
    }

    auto Deserializer::read(TypeID &var) -> Deserializer & {
        _stream.read(reinterpret_cast<char*>(&var), sizeof(var));
        return *this;
    }


    auto Deserializer::read(Asset *&_asset) -> Deserializer & {
        uint32_t guidLength{ 0 };
        read(guidLength);
        if (static_cast<int>(guidLength) > 0) {
            std::string guid;
            guid.resize(guidLength);
            _stream.read(&guid[0], guidLength);
            _asset = AssetController::Instance().GetAsset(guid);
        }else {
            _asset = nullptr;
        }
        return *this;
    }
}
