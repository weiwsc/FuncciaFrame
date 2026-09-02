//
// Created by Wangsicong Wei on 2025-08-14.
//

#ifndef FUNCCIAFRAME_OBJECTPOOL_H
#define FUNCCIAFRAME_OBJECTPOOL_H
#include <cstddef>
#include <iostream>
#include <ostream>
#include <vector>
#include <typeinfo>
#include <concepts>

namespace vva::core {
    using std::cout, std::endl;

    template<typename T>
    concept PoolableObject = requires(T t) {
        { t.Reset() } -> std::same_as<void>;
    };

    template<class T>
    requires PoolableObject<T>
    class ObjectPool
    {
    public:
        ObjectPool() {}
        ~ObjectPool() {
            for (std::size_t count = 0; count < m_all.size(); count++) {
                cout << "[" << typeid(ObjectPool).name() << "] " << "Deleting pool object: " << typeid(T).name() << endl;
                delete m_all[count];
            }
            m_all.clear();
            m_available.clear();
        }

        T* GetResource() {
            if (m_available.empty()) {

                cout << "[" << typeid(ObjectPool).name() << "] " << "Creating new pool object. " << typeid(T).name() << endl;
                T* resource = new T();
                m_all.push_back(resource);
                return resource;
            }
            else {
                cout << "[" << typeid(ObjectPool).name() << "] " << "Reusing existing pool object. " << typeid(T).name() << endl;
                T* resource = m_available[0];
                m_available.erase(m_available.begin());
                return resource;
            }
        }

        void ReleaseResource(T* _object) {
            if (_object) {
                _object->Reset();
                m_available.push_back(_object);
            }
        }

    private:
        std::vector<T*> m_available;
        std::vector<T*> m_all;
    };
}

#endif //FUNCCIAFRAME_OBJECTPOOL_H