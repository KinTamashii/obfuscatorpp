#pragma once
#include <bit>


template <typename INT>
constexpr INT arrayToInt(auto *ptr) {
    INT val = 0;
    if constexpr (std::endian::native == std::endian::little) {
        
        for (unsigned long i = 0; i < sizeof(INT)*sizeof(ptr); i+=sizeof(ptr)) {
            val |= static_cast<INT>(*ptr++) << i;
        }
        
    } else if constexpr (std::endian::native == std::endian::big) {
        
        for (unsigned long i = sizeof(INT)*sizeof(ptr); i; i-=sizeof(ptr)) {
            val |= static_cast<INT>(*ptr++) << i;
        }

    }
    return val;
}
#include <util/util.hpp>




template <util::StringLiteral str, typename INT>
inline bool arrayIntCompare(auto *a) {
    return (*(INT*)a) == arrayToInt<INT>(str.value);
}

template <util::StringLiteral str>
inline bool arrayStrCompare(auto *a) {
    if constexpr (str.size == 1)
        return (*(uint8_t*)a) == arrayToInt<uint8_t>(str.value);
    if constexpr (str.size == 2)
        return (*(uint16_t*)a) == arrayToInt<uint16_t>(str.value);
    if constexpr (str.size == 3)
        return ((*(uint32_t*)a)&0x00FFFFFF) == (arrayToInt<uint32_t>(str.value)&0x00FFFFFF);
    if constexpr (str.size == 4)
        return (*(uint32_t*)a) == arrayToInt<uint32_t>(str.value);
    if constexpr (str.size == 5)
        return ((*(uint64_t*)a)&0x000000FFFFFFFFFF) == (arrayToInt<uint64_t>(str.value)&0x000000FFFFFFFFFF);
    if constexpr (str.size == 6)
        return ((*(uint64_t*)a)&0x0000FFFFFFFFFFFF) == (arrayToInt<uint64_t>(str.value)&0x0000FFFFFFFFFFFF);
    if constexpr (str.size == 7)
        return ((*(uint64_t*)a)&0x00FFFFFFFFFFFFFF) == (arrayToInt<uint64_t>(str.value)&0x00FFFFFFFFFFFFFF);
    if constexpr (str.size == 8)
        return (*(uint64_t*)a) == arrayToInt<uint64_t>(str.value);
}

template <util::StringLiteral str>
bool findArray(auto *&cur, auto* end) {
    while (cur != end) {
        if (arrayStrCompare<str>(cur)) {
            cur += str.size;
            return true;
        }
        cur++;
    }
    return false;
}

