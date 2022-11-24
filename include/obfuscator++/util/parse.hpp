#pragma once
#define WS_SPACE " 　"
#define WS_NONSPACE "\r\t\n"
#define WS_ALL WS_SPACE WS_NONSPACE
#include <util/unicode.hpp>
#include <abbr.hpp>





inline bool isWhitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}




template <bool toLower = false>
std::string lastWord(const char *c, const char *begin) {
    const char *end = c;
    while (unicode::u8::reverse_ifNotAnyCodePoint<WS_ALL>((uint8_t **)&c, (uint8_t*)begin));
    unicode::u8::codepoint(&c);
    std::string str;
    
    size_t size = end-c;
    if constexpr (toLower) {
        
        str.reserve(size);
        while (c < end) {
            str += std::tolower(*c);
        }
    } else {
        try {
            str.assign(c, size);
        } catch (std::length_error &err) {
            
        }
        
    }
    return str;
}

inline bool isNumeric(const char *c) {
    while (*c) {
        if (*c < '0' || '9' < *c) return false;
        c++;
    }
    return true;
}


inline bool isAbbreviation(const char *c, const char* begin, int language) {
    std::string s = lastWord(c, begin);
    return ((s.size() == 1 && ('A' <= s[0] && s[0] <= 'Z')) || abbreviations::match(s.c_str(), language) || isNumeric(s.c_str())) && s.size();
}

inline bool isAbbreviation(uint8_t *c, uint8_t* begin, int language) {
    return isAbbreviation((const char*)c, (const char*)begin, language);
}

inline bool checkNumericSubscript(const char *s) {
    while (*s != ']') {
        if (*s < '0' || '9' < *s) return false;
        s++;
    }
    return true;
}

inline bool isNumericSubscript(const char *s) {
    return ((*s++ == '[') && checkNumericSubscript(s));
}

inline bool checkNumericSubscript(uint8_t *s) {
    while (*s != ']') {
        if (*s < '0' || '9' < *s) return false;
        s++;
    }
    return true;
}

inline bool isNumericSubscript(uint8_t *s) {
    return ((*s++ == '[') && checkNumericSubscript(s));
}

inline bool checkNumericSubscript(uint8_t **s) {
    (*s)++;
    while (**s != ']') {
        if (**s < '0' || '9' < **s) return false;
        (*s)++;
    }
    (*s)++;
    return true;
}

inline bool isNumericSubscript(uint8_t **s) {
    return ((**s == '[') && checkNumericSubscript(s));
}





inline void skipWhitespace(uint8_t **c) {
    while (unicode::u8::advance_ifIsAnyCodePoint<WS_ALL>(c));
}

constexpr auto letterRangeArray = unicode::u8::codePointRangeArray<"09AZazÀÖØöøʯ\u0370\u0377\u037A\u037F\u0384\u0386\u0388\u038A\u038C\u038C\u038E\u03A1\u03A3\u052F\u0531\u0556\u0559\u055F\u0560\u058A\u058D\u058F\u0591\u05A1\u05A2\u05B9\u05BA\u05C7\u05D0\u05EA\u05EF\u05F4\u0600\u061A\u0620\u06FF\u0780\u07B1\u0900\u0983\u0985\u098C\u098F\u0990\u0993\u09A8\u09AA\u09B0\u09B2\u09B2\u09B6\u09B9\u09BC\u09BD\u09BE\u09C4\u09C7\u09C8\u09CB\u09CE\u09D7\u09D7\u09DC\u09DD\u09DF\u09E3\u09E6\u09FE\u0A01\u0A03\u0A05\u0A0A\u0A0F\u0A10\u0A13\u0A28\u0A2A\u0A30\u0A32\u0A33\u0A35\u0A36\u0A38\u0A39\u0A3C\u0A3C\u0A3E\u0A42\u0A47\u0A48\u0A4B\u0A4D\u0A51\u0A51\u0A59\u0A5C\u0A5E\u0A5E\u0A66\u0A76\u0A81\u0A83\u0A85\u0A8D\u0A8F\u0A91\u0A93\u0AA8\u0AAA\u0AB0\u0AB2\u0AB3\u0AB5\u0AB9\u0ABC\u0AC5\u0AC7\u0AC9\u0ACB\u0ACD\u0AD0\u0AD0\u0AE0\u0AE3\u0AE6\u0AF1\u0AF9\u0AF9\u0AFA\u0AFF\u0B01\u0B03\u0B05\u0B0C\u0B0F\u0B10\u0B13\u0B28\u0B2A\u0B30\u0B32\u0B33\u0B35\u0B39\u0B3C\u0B44\u0B47\u0B48\u0B4B\u0B4D\u0B55\u0B57\u0B5C\u0B5D\u0B5F\u0B63\u0B66\u0B77\u0B82\u0B83\u0B85\u0B8A\u0B8E\u0B90\u0B92\u0B95\u0B99\u0B9A\u0B9C\u0B9C\u0B9E\u0B9F\u0BA3\u0BA4\u0BA8\u0BAA\u0BAE\u0BB5\u0BB6\u0BB9\u0BBE\u0BC2\u0BC6\u0BC8\u0BCA\u0BCD\u0BD0\u0BD0\u0BD7\u0BD7\u0BE6\u0BFA\u0C00\u0C0C\u0C0E\u0C10\u0C12\u0C28\u0C2A\u0C39\u0C3C\u0C44\u0C46\u0C48\u0C4A\u0C4D\u0C55\u0C56\u0C58\u0C63\u0C66\u0C6F\u0C77\u0C8C\u0C8E\u0C90\u0C92\u0CA8\u0CAA\u0CB3\u0CB5\u0CB9\u0CBC\u0CC4\u0CC6\u0CC8\u0CCA\u0CCD\u0CD5\u0CD6\u0CDD\u0CDE\u0CE0\u0CE3\u0CE6\u0CEF\u0CF1\u0CF3\u0D00\u0D0C\u0D0E\u0D10\u0D12\u0D48\u0D4A\u0D4F\u0D54\u0D63\u0D66\u0D7F\u0D81\u0D83\u0D85\u0D96\u0D9A\u0DB1\u0DB3\u0DBB\u0DBD\u0DBD\u0DC0\u0DC6\u0DCA\u0DCA\u0DCF\u0DD4\u0DD6\u0DD6\u0DD8\u0DDF\u0DE6\u0DEF\u0DF2\u0DF4\u0E01\u0E3A\u0E3F\u0E5B\u0E81\u0E82\u0E84\u0E84\u0E86\u0E8A\u0E8C\u0EA3\u0EA5\u0EA5\u0EA7\u0EA9\u0EAA\u0EBA\u0EBB\u0EBD\u0EC0\u0EC4\u0EC6\u0EC6\u0EC8\u0ECE\u0ED0\u0ED9\u0EDC\u0EDF\u1000\u109F\u10A0\u10C5\u10C7\u10C7\u10CD\u10CD\u10D0\u10FF\u1200\u1248\u124A\u124D\u1250\u1256\u1258\u1258\u125A\u125D\u1260\u1288\u128A\u128D\u1290\u12B0\u12B2\u12B5\u12B8\u12BE\u12C0\u12C0\u12C2\u12C5\u12C8\u12D6\u12D8\u1310\u1780\u17DD\u17E0\u17E9\u17F0\u17F9\u1F00\u1F15\u1F18\u1F1D\u1F20\u1F45\u1F48\u1F4D\u1F50\u1F57\u1F59\u1F59\u1F5B\u1F5B\u1F5D\u1F5D\u1F5F\u1F7D\u1F80\u1FB4\u1FB6\u1FC4\u1FC6\u1FD3\u1FD6\u1FDB\u1FDD\u1FEF\u1FF2\u1FF4\u1FF6\u1FFE\u1312\u1315\u1318\u135A\u135D\u137C\u3041\u3096\u3099\u30FF\u4000\u9FFF\uABC0\uABED\uABF0\uABF9">();

#include <list>

constexpr bool isLetter(uint32_t code) {
    return (code < letterRangeArray.size() && letterRangeArray[code]);
}


bool findString(auto *&cur, auto *end) {
    while (cur != end) {
        if (*cur == '"') return true;
        cur++;
    }
    return false;
}

std::string getString(auto *&cur, auto *end) {
    std::string res = "";
    cur++;
    while (cur != end && *cur != '"') {
        if (*cur == '\\') {
            cur++;
        }
        res += *cur++;
    }
    cur++;
    return res;
}