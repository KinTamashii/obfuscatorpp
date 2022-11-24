#pragma once
#include <util/util.hpp>
#include <array>
namespace unicode {

    namespace u8 {

        constexpr uint32_t codepoint(const char *s) {
            uint8_t c = s[0], c1, c2, c3;
            const char *tmp = s;
            size_t size = 0;
            while (*tmp++) size++;
            if ((size) && (c < 0x80))
                return c;
            if (c < 0xC0)
                return 0;
            if ((size > 1) && (c < 0xE0)) {
                c1 = s[1];
                return ((c & 0x1F) << 6) | (c1 & 0x3F);
            }
            if ((size > 2) && (c < 0xF0)) {
                c1 = s[1];
                c2 = s[2];
                return ((c & 0xF) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
            }
            if ((size > 3) && (c < 0xF8)) {
                c1 = s[1];
                c2 = s[2];
                c3 = s[3];
                return ((c & 0x7) << 18) |
                    ((c1 & 0x3F) << 12) |
                    ((c2 & 0x3F) << 6) |
                    ((c3 & 0x3F));
            }
            return 0;
        }

        constexpr uint32_t codepoint(uint8_t *s) {
            if (s[0] < 0x80)
                return s[0];
            if (s[0] < 0xC0)
                return 0;
            if (s[0] < 0xE0) {
                return ((s[0] & 0x1F) << 6) | (s[1] & 0x3F);
            }
            if (s[0] < 0xF0) {
                return ((s[0] & 0xF) << 12) | ((s[1] & 0x3F) << 6) | (s[2] & 0x3F);
            }
            if (s[0] < 0xF8) {
                return ((s[0] & 0x7) << 18) |
                    ((s[1] & 0x3F) << 12) |
                    ((s[2] & 0x3F) << 6) |
                    ((s[3] & 0x3F));
            }
            return 0;
        }

        constexpr uint32_t codepoint(const char** input) {
            const char *& s = *input;
            uint8_t value = (*s);
            uint32_t code = 0;
            if (value < 0x80) {
                code |= value;
                s++;
                return value;
            }
                
            if (value < 0xC0) {
                s++;
                return 0;
            }
            if (value < 0xE0) {
                
                code |= ((value & 0x1F) << 6); s++; value = *s++;
                code |= (value & 0x3F);
                return code;
            }
            if (value < 0xF0) {
                
                code |= ((value & 0xF) << 12); s++; value = *s;
                code |= ((value & 0x3F) << 6); s++; value = *s++;
                code |= (value & 0x3F);
                return code;
            }
            if (value < 0xF8) {
                code |= ((value & 0x7) << 18); s++; value = *s;
                code |= ((value & 0x3F) << 12); s++; value = *s;
                code |= ((value & 0x3F) << 6); s++; value = *s++;
                code |= (value & 0x3F);
                return code;
            }
            s++;
            return 0;
        }

        constexpr uint32_t codepoint(uint8_t **input) {
            uint8_t *& s = *input;
            if (*s < 0x80)
                return *s++;
            if (*s < 0xC0) {
                s++;
                return 0;
            }
            if (*s < 0xE0) {
                return ((*s++ & 0x1F) << 6) | (*s++ & 0x3F);
            }
            if (*s < 0xF0) {
                return ((*s++ & 0xF) << 12) | ((*s++ & 0x3F) << 6) | (*s++ & 0x3F);
            }
            if (*s < 0xF8) {
                return ((*s++ & 0x7) << 18) |
                    ((*s++ & 0x3F) << 12) |
                    ((*s++ & 0x3F) << 6) |
                    ((*s++ & 0x3F));
            }
            s++;
            return 0;
        }

        constexpr uint32_t reverse_codepoint(uint8_t **input, uint8_t *begin) {
            uint8_t *& s = *input;
            uint8_t c;
            if ((s > begin + 2) && (0xF0 <= (c = *(s-3)) && c < 0xF8)) {
                s-=4;
                return (((*(s+1)) & 0x7) << 18) |
                    (((*(s+2)) & 0x3F) << 12) |
                    (((*(s+3)) & 0x3F) << 6) |
                    (((*(s+4)) & 0x3F));
            }
            ;
            if ((s > begin + 1) && (0xE0 <= (c = *(s-2)) && c < 0xF0)) {
                s-=3;
                return (((*(s+1)) & 0xF) << 12) | (((*(s+2)) & 0x3F) << 6) | ((*(s+3)) & 0x3F);
            }
            ;
            if ((s > begin) && (0xC0 <= (c = *(s-1)) && c < 0xE0)) {
                s-=2;
                return (((*(s+1)) & 0x1F) << 6) | ((*(s+2)) & 0x3F);
            }
            if (s > begin) {
                return *s--;
            }
            if (s == begin) {
                return *s--;
            }
            return 0;
        }

        struct codeRange {
            uint32_t low;
            uint32_t high;
        };


        constexpr codeRange codepointRange(const char *s) {
            uint32_t low = 0, high = 0;
            size_t off = 0;
            if ((uint8_t)s[off] < 0x80)
                low = s[off++];
            else if ((uint8_t)s[off] < 0xC0) {
                low = 0;
                off++;
            } else if ((uint8_t)s[off] < 0xE0) {
                low = (((uint8_t)s[off++] & 0x1F) << 6) | ((uint8_t)s[off++] & 0x3F);
            } else if ((uint8_t)s[off] < 0xF0) {
                low = (((uint8_t)s[off++] & 0xF) << 12) | (((uint8_t)s[off++] & 0x3F) << 6) | ((uint8_t)s[off++] & 0x3F);
            } else if ((uint8_t)s[off++] < 0xF8) {
                low = (((uint8_t)s[off++] & 0x7) << 18) |
                    (((uint8_t)s[off++] & 0x3F) << 12) |
                    (((uint8_t)s[off++] & 0x3F) << 6) |
                    (((uint8_t)s[off++] & 0x3F));
            } else {
                low = 0;
                off++;
            }
            if ((uint8_t)s[off] < 0x80)
                high = s[off++];
            else if ((uint8_t)s[off] < 0xC0) {
                high = 0;
                off++;
            } else if ((uint8_t)s[off] < 0xE0) {
                high = (((uint8_t)s[off++] & 0x1F) << 6) | ((uint8_t)s[off++] & 0x3F);
            } else if ((uint8_t)s[off] < 0xF0) {
                high = (((uint8_t)s[off++] & 0xF) << 12) | (((uint8_t)s[off++] & 0x3F) << 6) | ((uint8_t)s[off++] & 0x3F);
            } else if ((uint8_t)s[off++] < 0xF8) {
                high = (((uint8_t)s[off++] & 0x7) << 18) |
                    (((uint8_t)s[off++] & 0x3F) << 12) |
                    (((uint8_t)s[off++] & 0x3F) << 6) |
                    (((uint8_t)s[off++] & 0x3F));
            } else {
                high = 0;
                off++;
            }
            return {low, high};
        }

        template <util::StringLiteral str>
        constexpr uint32_t highestCodePoint() {
            const char * c = str.value;
            uint32_t cur = 0, max = 0;
            while ((cur = codepoint(&c))) {
                cur++;
                max = (max > cur) ? max : cur;
            }
                
            return max;
        }

        template <util::StringLiteral str, size_t size = highestCodePoint<str>()>
        constexpr std::array<bool, size> codePointRangeArray() {
            std::array<bool, size> arr{};
            
            for (const char *it = str.value; ;) {
                uint32_t low = codepoint(&it); if(!(low)) break;
                uint32_t high = codepoint(&it); if(!(high)) break;
                for (uint32_t index = low; index <= high; index++) {
                    arr[index] = true;
                }
                if(!(*it)) break;
            }
            return arr;
        }
        template <util::StringLiteral... str>
        constexpr bool inCodePointRange(uint32_t code) {
            return (
                    (
                        (codepointRange(str.value).low == codepointRange(str.value).high) ?
                            (codepointRange(str.value).low == code) :
                            (codepointRange(str.value).low <= code && code <= codepointRange(str.value).high)
                    ) || ...
                );
        }


        constexpr size_t numCodePoints(const char *c) {
            size_t num = 0;
            while (unicode::u8::codepoint(&c)) num++;
            return num;
        }

        template <util::StringLiteral str>
        constexpr size_t numCodePoints() {
            const char *c = str.value;
            size_t num = 0;
            while (unicode::u8::codepoint(&c)) num++;
            return num;
        }


        template <util::StringLiteral str, size_t N = numCodePoints<str>()>
        constexpr std::array<uint32_t, N> codepointArray() {
            const char *c = str.value;
            std::array<uint32_t, N> arr;
            for (size_t i = 0; i < N; i++) {
                arr[i] = unicode::u8::codepoint(&c);
            }
            return arr;
        }


        template <util::StringLiteral str>
        constexpr bool isAnyCodePoint(uint32_t code) {
            return std::apply([code](auto&&... c){return (... || (c == code));}, codepointArray<str>());
        }

        template <util::StringLiteral str>
        inline bool advance_ifIsAnyCodePoint(uint8_t **c) {
            uint8_t *temp = *c;
            //uint32_t code = unicode::u8::codepoint(&temp);
            bool condition;
            condition = isAnyCodePoint<str>(unicode::u8::codepoint(&temp));
            (*c) += (condition * (temp - *c));
            return condition;
        }

        template <util::StringLiteral str>
        inline bool advance_ifNotAnyCodePoint(uint8_t **c) {
            uint8_t *temp = *c;
            //uint32_t code = unicode::u8::codepoint(&temp);
            bool condition = !isAnyCodePoint<str>(unicode::u8::codepoint(&temp));
            (*c) += (condition * (temp - *c));
            return condition;
        }

        template <util::StringLiteral str>
        inline bool reverse_ifIsAnyCodePoint(uint8_t **c, uint8_t *begin) {
            uint8_t *temp = *c;
            bool condition = isAnyCodePoint<str>(unicode::u8::reverse_codepoint(&temp, begin));
            (*c) += (condition * (temp - *c));
            return condition && (begin != temp+1);
        }
#include <stdio.h>
        template <util::StringLiteral str>
        inline bool reverse_ifNotAnyCodePoint(uint8_t **c, uint8_t *begin) {
            uint8_t *temp = *c;
            bool condition = !isAnyCodePoint<str>(unicode::u8::reverse_codepoint(&temp, begin));
            (*c) += (condition * (temp - *c));
            return condition && (begin != temp+1);
        }

        inline void append_codepoint(std::string &str, uint32_t code) {
            
            if (code < 0x80) {
                str += code;
                return;
            }
            if (code < 0x800) {
                str += 0xC0 | ((code&0xFC0)>>6);
                str += 0x80 | (code&0x3F);
                return;
            }
            if (code < 0xD800 || (0xE000 < code && code < 0xFFFF)) {
                code = (0xE0 | (code&0xF000) >> 12) |
                            (0x8000 | ((code&0xFC0)<<2)) |
                            (0x800000 | ((code&0x3F)<<16));
                str += code&0xFF;
                str += (code&0xFF00) >> 8;
                str += (code&0xFFFF00) >> 16;
                return;
            }
            code = 0x808080F0 | (code&0x1C0000) >> 18 | (code&0x3F000) >> 4 | (code&0xFC0) << 10 | (code&0x3F) << 24;
            str += code & 0xFF;
            str += (code & 0xFF00) >> 8;
            str += (code & 0xFF0000) >> 16;
            str += (code & 0xFF000000) >> 24;
        }
    };
};

