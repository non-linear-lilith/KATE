#pragma once
#include <functional>

namespace kate{
        /**
     * @brief Combines the hash of multiple values into a single hash value, using the FNV-1a algorithm
     * 
     * @param seed  The seed value to start the hash 
     * @param v direcction in memory of the value to hash
     * @param rest The rest of the values to hash 
     * @return void
     */
    template<typename T, typename... Rest>
    void hashFNV1A(std::size_t& seed, const T& v, const Rest&... rest){
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        (void)std::initializer_list<int>{(hashFNV1A(seed, rest), 0)...};
    }

}