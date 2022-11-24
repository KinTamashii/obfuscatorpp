#include "url.hpp"
int url::unicodePointCounter(std::string::const_iterator &it) {
    uint8_t c = *it;
    if (c < 0xC0) {
        it++;
        return 1;
    }

    if (c < 0xE0) {
        it += 2;
        return 1;
    }

    if (c < 0xF0) {
        it += 3;
        return 1;
    }

    if (c < 0xF8) {
        it += 4;
        return 2;
    }
    it++;
    return 1;
}