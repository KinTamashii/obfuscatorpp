#include "base122.hpp"
#include <util/formattedUnicode.hpp>

std::string base122::encode(size_t n) {
        std::string str = "";
        do {
            str += digits[n % base];
            n /= base;
        } while (n);
        std::reverse(str.begin(), str.end());
        return str;
    }

size_t base122::decode(const std::string & str) {
    size_t n = 0;
    for (uint8_t c : str) {
        n *= base;
        n += digitValues[c];
    }
    return n;
}

size_t base122::decodeString(uint8_t *pos, uint8_t *end) {
    size_t n = 0;
    pos++;
    while (pos != end && *pos != '"') {
        n *= base;
        if (*pos == '\\') {
            pos++;
            switch (*pos) { /* Handle escaped characters from a returned json string. */
                case 'b':
                    n += digitValues['\b'];
                    pos++;
                    continue;
                case 'f':
                    n += digitValues['\f'];
                    pos++;
                    continue;
                case 'n':
                    n += digitValues['\n'];
                    pos++;
                    continue;
                case 'r':
                    n += digitValues['\b'];
                    pos++;
                    continue;
                case 't':
                    n += digitValues['\t'];
                    pos++;
                    continue;
                case 'u':
                    n += digitValues[readFormattedUnicodePoint(pos)];
                    continue;

            }
        }
        n += digitValues[*pos++];
    }
    return n;
}