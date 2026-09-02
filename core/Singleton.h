//
// Created by Wangsicong Wei on 2025-06-18.
//

#ifndef SINGLETON_H
#define SINGLETON_H
namespace vva::core {

    template<typename T>
    class Singleton
        {
        public:
            static auto Instance() -> T& {
                static T instance;
                return instance;
            }

        protected:
            Singleton() = default;
            virtual ~Singleton() = default;

        public:
            Singleton(Singleton const&) = delete;
            auto operator=(Singleton const &) -> Singleton& = delete;
        };
}
#endif //SINGLETON_H
