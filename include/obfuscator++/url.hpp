#pragma once
#include <iostream>
#include <array>

#define WL_ACCESS(whitelist, chr) whitelist[(chr) - 33]



namespace url {


inline void encodeHexCode(std::string &str, char c) {
    str += '%';
    str += (c&0xF0) >> 4;
    str.back() += (str.back() > 9) ? 0x37 : '0';
    str += c&0xF;
    str.back() += (str.back() > 9) ? 0x37 : '0';
}



template <std::array<bool, 94> whitelist>
std::string encode(const std::string &input) {
    std::string output;
    for (auto it = input.begin(); it != input.end(); it++) {
        uint8_t c = *it;
        
        if ((0x20 < c && c < 0x7F) && (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9') || WL_ACCESS(whitelist, c))) {
            if constexpr (WL_ACCESS(whitelist, '%')) {
                uint8_t c1 = *(it+1);
                uint8_t c2 = *(it+2);
                if (c == '%' && (
                        ('0' <= c1 && c1 <= '9') || ('A' <= c1 && c1 <= 'F') || ('a' <= c1 && c1 <= 'f')
                    ) && (
                        ('0' <= c2 && c2 <= '9') || ('A' <= c2 && c2 <= 'F') || ('a' <= c2 && c2 <= 'f')
                    )
                ) {
                    encodeHexCode(output, '%');
                    continue;
                }
            }
            output += c;
            continue;
        }
        if constexpr (!WL_ACCESS(whitelist, '+')) {
            if (c == ' ') {
                output += '+';
                continue;
            }
        }
        
        encodeHexCode(output, c);
    }
    return output;

}





inline char decodeHexCode(std::string::const_iterator &it) {
    uint8_t c1 = *(it + 1), c2 = *(it + 2);
    uint8_t cond =
    (('0' <= c1 && c1 <= '9')) |
    (('A' <= c1 && c1 <= 'F')<<1) |
    (('a' <= c1 && c1 <= 'f')<<2) | 
    (('0' <= c2 && c2 <= '9')<<3) |
    (('A' <= c2 && c2 <= 'F')<<4) |
    (('a' <= c2 && c2 <= 'f')<<5);
    bool isHexCode = (((cond&0x1) || (cond&0x2) || (cond&0x4)) && ((cond&0x8) || (cond&0x10) || (cond&0x20)));
    it += isHexCode << 1;

    return ((((c1 - 0x30 - (7* (!(cond&0x1) && ((cond&0x2) || (cond&0x4))) )) << 4) |
        (c2 - 0x30 - (7* (!(cond&0x8) && ((cond&0x10) || (cond&0x20))) )))*(
            isHexCode
        )) + ('%'*(
            !isHexCode
        ));

}


template <bool plusWhitelisted = false>
std::string decode(const std::string &input) {
    std::string output;
    for (auto it = input.begin(); it != input.end(); it++) {
        uint8_t c = *it;
        if (c == '%') {
            output += decodeHexCode(it);
            continue;
        }
        if constexpr (!plusWhitelisted) {
            if (c == '+') {
                output += ' ';
                continue;
            }
        }
        output += c;
        
    }
    return output;
}


inline uint32_t decodeHexUnicode(std::string::const_iterator &it) {
    uint8_t c = decodeHexCode(it);
    if (c < 0x80) return c;
    if ((c < 0xC0) || (it[1] != '%')) return 0;
    if (c < 0xE0) return ((c & 0x1F) << 6) |
                        (decodeHexCode(it) & 0x3F);
    if (it[4] != '%') return 0;
    if (c < 0xF0) return ((c & 0xF) << 12) |
                        ((decodeHexCode(it) & 0x3F) << 6) |
                        decodeHexCode(it) & 0x3F;
    if (it[7] != '%') return 0;
    if (c < 0xF8) return ((c & 0x7) << 18) |
                        ((decodeHexCode(it) & 0x3F) << 12) |
                        ((decodeHexCode(it) & 0x3F) << 6) |
                        decodeHexCode(it) & 0x3F;
    return 0;
}

inline uint32_t decodeHexUnicodeTk(std::string::const_iterator &it, size_t &size) {
    uint32_t c = decodeHexCode(it)&0xFF;
    if (c < 0x80) return c;
    if ((c < 0xC0) || (it[1] != '%')) return 0;
    it++; size++;
    if (c < 0xE0) {
        return c + decodeHexCode(it)&0xFF;
    }
    if (it[3] != '%') return 0;
    if (c < 0xF0) {
        c += decodeHexCode(it)&0xFF;
        it++; size++;
        c += decodeHexCode(it)&0xFF;
        return c;
    }
    if (it[6] != '%') return 0;
    if (c < 0xF8) {
        c += decodeHexCode(it)&0xFF;
        it++; size++;
        c += decodeHexCode(it)&0xFF;
        it++; size++;
        c += decodeHexCode(it)&0xFF;
        return c;
    }
    return 0;
}





constexpr auto WhiteList(const char * str) {
    std::array<bool, 94> whitelist {{false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false}};
    //    -._~()'!*:@,;
    WL_ACCESS(whitelist, '-') = true;
    WL_ACCESS(whitelist, '.') = true;
    WL_ACCESS(whitelist, '_') = true;
    WL_ACCESS(whitelist, '~') = true;
    WL_ACCESS(whitelist, '(') = true;
    WL_ACCESS(whitelist, ')') = true;
    WL_ACCESS(whitelist, '\'') = true;
    WL_ACCESS(whitelist, '!') = true;
    WL_ACCESS(whitelist, '*') = true;
    WL_ACCESS(whitelist, ':') = true;
    WL_ACCESS(whitelist, '@') = true;
    WL_ACCESS(whitelist, ',') = true;
    WL_ACCESS(whitelist, ':') = true;

    //?/#+&=%<>[]{}|\^
    while (*str) {
        if(!(
            *str == '?' ||
            *str == '/' ||
            *str == '#' ||
            *str == '+' ||
            *str == '&' ||
            *str == '=' ||
            *str == '%' ||
            *str == '<' ||
            *str == '>' ||
            *str == '[' ||
            *str == ']' ||
            *str == '{' ||
            *str == '}' ||
            *str == '|' ||
            *str == '\\' ||
            *str == '^'
        )) throw std::logic_error("Whitelisted character must be one of the following \"?/#+&=%<>[]{}|\\^\"\n");
        WL_ACCESS(whitelist, *str++) = true;
    }
    return whitelist;
}







template <std::array<bool, 94> whitelist>
void encodeChar(std::string &output, std::string::const_iterator &it, uint16_t &codePoints) {
    uint8_t c = *it;
    
    if ((c < 0x7F) && (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9') || WL_ACCESS(whitelist, c))) {
        if constexpr (WL_ACCESS(whitelist, '%')) {
            uint8_t c1 = *(it+1);
            uint8_t c2 = *(it+2);
            if (c == '%' && (
                    ('0' <= c1 && c1 <= '9') || ('A' <= c1 && c1 <= 'F') || ('a' <= c1 && c1 <= 'f')
                ) && (
                    ('0' <= c2 && c2 <= '9') || ('A' <= c2 && c2 <= 'F') || ('a' <= c2 && c2 <= 'f')
                )
            ) {
                encodeHexCode(output, '%');
                codePoints++;
                return;
            }
        }
        output += c;
        codePoints++;
        return;
    }
    if constexpr (!WL_ACCESS(whitelist, '+')) {
        if (c == ' ') {
            output += '+';
            codePoints++;
            return;
        }
    }

    if (0xC0 <= c && c < 0xE0) {
        encodeHexCode(output, c);
        c = *(++it);
        encodeHexCode(output, c);
        codePoints++;
        return;
    }

    if (c < 0xF0) {
        encodeHexCode(output, c);
        c = *(++it);
        encodeHexCode(output, c);
        c = *(++it);
        encodeHexCode(output, c);
        codePoints++;
        return;
    }

    if (c < 0xF8) {
        encodeHexCode(output, c);
        c = *(++it);
        encodeHexCode(output, c);
        c = *(++it);
        encodeHexCode(output, c);
        c = *(++it);
        encodeHexCode(output, c);
        codePoints+=2;
        return;
    }
}

int unicodePointCounter(std::string::const_iterator &it);


//max 5000 code points (4 byte long count as a length of two)
//max url extension 16384 bytes long. (16 kilobytes)

template <std::array<bool, 94> whitelist>
int encode_count(std::string::const_iterator &it, int &codePoints) {
    uint8_t c = *it;
    if ((0x20 < c && c < 0x7F) && (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9') || WL_ACCESS(whitelist, c))) {
        if constexpr (WL_ACCESS(whitelist, '%')) {
            uint8_t c1 = *(it+1);
            uint8_t c2 = *(it+2);
            if (c == '%' && (
                    ('0' <= c1 && c1 <= '9') || ('A' <= c1 && c1 <= 'F') || ('a' <= c1 && c1 <= 'f')
                ) && (
                    ('0' <= c2 && c2 <= '9') || ('A' <= c2 && c2 <= 'F') || ('a' <= c2 && c2 <= 'f')
                )
            ) {
                it++;
                codePoints++;
                return 3;
            }
        }
        
        it++;
        codePoints++;
        return 1;
    }
    
    if constexpr (!WL_ACCESS(whitelist, '+')) {
        if (c == ' ') {
            it++;
            codePoints++;
            return 1;
        }
    }

    if (c < 0xC0) {
        it++;
        codePoints++;
        return 3;
    }

    if (c < 0xE0) {
        it += 2;
        codePoints++;
        return 6;
    }

    if (c < 0xF0) {
        it += 3;
        codePoints++;
        return 9;
    }

    if (c < 0xF8) {
        it += 4;
        codePoints+=2;
        return 12;
    }
    
    it++;
    codePoints++;
    return 3;
}

}


#undef WL_ACCESS