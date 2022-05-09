#pragma once

#include <typeinfo>

namespace mono {

    using RuntimeTypeId = size_t;

    template<typename T>
    RuntimeTypeId getRuntimeTypeId()
    {
        return reinterpret_cast<const RuntimeTypeId>(&typeid(T));
    }
}