#pragma once
#include <cstdint>

inline auto hexCharToByte(auto b) {
    return b - '0' - (7*(b>'9')) - (0x20*(b > 'F'));
}

inline uint16_t readFormattedUnicodePoint(uint8_t *&ptr) {
    ptr++;
    uint16_t val = (
        (
            (hexCharToByte(ptr[0])) << 12
        ) | (
            (hexCharToByte(ptr[1])) << 8
        ) | (
            (hexCharToByte(ptr[2])) << 4
        ) | (
            (hexCharToByte(ptr[3]))
        )
    );
    ptr += 4;
    return val;
}