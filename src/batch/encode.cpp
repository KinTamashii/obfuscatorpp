#include "encode.hpp"
#include <util/unicode.hpp>



inline uint8_t halfByteToHexDigit(uint8_t b) {
    return b + '0' + (0x7 * (b > 0x9));
}

void addEscapedCodepoint(std::string &str, uint32_t code) {
    str += "\\u";
    str += halfByteToHexDigit((code&0xF000)>>12);
    str += halfByteToHexDigit((code&0x0F00)>>8);
    str += halfByteToHexDigit((code&0x00F0)>>4);
    str += halfByteToHexDigit((code&0x000F));
}

std::string twiceStringify(const std::string &str) {
    std::string result = "";
    uint8_t *cur = (uint8_t*)str.c_str();
    uint32_t code;
    while ((code = unicode::u8::codepoint(&cur))) {
        switch (code) {
            case '&':
                result += R"(\\u0026)";
                break;
            case '\b':
                result += R"(\\b)";
                break;
            case '\f':
                result += R"(\\f)";
                break;
            case '\n':
                result += R"(\\n)";
                break;
            case '\r':
                result += R"(\\r)";
                break;
            case '\t':
                result += R"(\\t)";
                break;
            case '\"':
                result += R"(\\\")";
                break;
            case '\\':
                result += R"(\\\\)";
                break;
            default:
                if (0x1F < code && code < 0x7F) {
                    result += code;
                } else if (code < 0x20 || code < 0x10000) {
                    addEscapedCodepoint(result, code);
                } else {
                    addEscapedCodepoint(result, 0xD7C0 + (code >> 10));
                    addEscapedCodepoint(result, 0xDC00 + (code & 0x3FF));
                }
                break;
        }
    }
    return result;
}