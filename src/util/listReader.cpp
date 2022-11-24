#include <listReader.hpp>
#include <util/formattedUnicode.hpp>


void to8(uint16_t code, std::string &str) {
    if (code < 0x80) {
        str += code;
        return;
    }
    if (code < 0x800) {
        str += 0xC0 | ((code&0xFC0)>>6);
        str += 0x80 | (code&0x3F);
        return;
    }
    if (code < 0xD800 || 0xE000 < code) {
        code = (0xE0 | (code&0xF000) >> 12) |
                    (0x8000 | ((code&0xFC0)<<2)) |
                    (0x800000 | ((code&0x3F)<<16));
        
        str += code&0xFF;
        str += (code&0xFF00) >> 8;
        str += (code&0xFFFF00) >> 16;
        return;
    }
}
void to8(uint16_t high, uint16_t low, std::string &str) {
    uint32_t code = 0x10000 | ((high & 0x3FF)<<10) | ((low & 0x3FF));
    code = 0x808080F0 | (code&0x1C0000) >> 18 | (code&0x3F000) >> 4 | (code&0xFC0) << 10 | (code&0x3F) << 24;
    str += code & 0xFF;
    str += (code & 0xFF00) >> 8;
    str += (code & 0xFF0000) >> 16;
    str += (code & 0xFF000000) >> 24;
}


listReader listReader::getStringObject() {
    
    cur += escape;
    uint8_t *b = cur;
    while (*cur != '"') {
        if (*cur == '\\') {
            if ((escape > 1) && cur[escape/2] == '"') {
                break;
            }
            cur += escape;
            if (*cur == '\\' && (escape > 1)) {
                cur++;
            }
        }
        cur++;
    }
    int bitlength = 0;
    size_t temp = escape;
    while ((temp >>= 1)) bitlength++;
    return listReader(b, cur++, bitlength+1);
}

void listReader::travel(const std::vector<size_t> &location) {
    auto l_it = location.begin();
    auto p_it = position.begin();
    
    auto l_end = location.end();
    auto p_end = position.end();

    while (p_it != p_end && l_it != l_end && *p_it == *l_it) {l_it++; p_it++;}
    exit((p_end - p_it - 1) * (p_end > p_it + 1));
    if (p_it != p_end) if (reach(l_it)) return;
    ssize_t dif = -1;
    while (valid() && l_it != l_end) {
        switch (*cur) {
            case '[':
                position.push_back(0); dif++;
                if (!dif && !(*l_it)) {
                    l_it++; dif--;
                    if (l_it == l_end) {
                        cur++;
                        skipWS();
                        return;
                    }
                }
                break;
            case ']':
                if (!dif) {
                    return;
                }
                position.pop_back(); dif--;
                break;
            case ',':
                if (dif < 0) {
                    return;
                }
                position.back()++;
                if (!dif && (position.back() == *l_it)) {
                    l_it++; dif--;
                    if (l_it == l_end) {
                        cur++;
                        skipWS();
                        return;
                    }
                }
                break;
            case '"':
                cur++;
                while (valid() && *cur != '"') {
                    if (*cur == '\\') {
                        if ((escape > 1) && cur[escape/2] == '"') {
                            cur += escape/2;
                            break;
                        }
                        cur += escape;
                        if (*cur == '\\' && (escape > 1)) {
                            cur++;
                        }
                    }
                    cur++;
                }
                break;
        }
        cur++;
    }
}
std::string listReader::getString(uint8_t *pos, uint8_t *end, size_t escape) {
    std::string str = "";
    pos += escape;
    while (pos != end && *pos != '"') {
        if (*pos == '\\') {
            if ((escape > 1) && pos[escape/2] == '"') {
                pos += escape/2;
                return str;
            }
            
            pos += escape;
            if (pos == end) return str;
            switch (*pos) {
                case 'b':
                    str += '\b';
                    pos++;
                    continue;
                case 'f':
                    str += '\f';
                    pos++;
                    continue;
                case 'n':
                    pos++;
                    str += '\n';
                    continue;
                case 'r':
                    str += '\r';
                    pos++;
                    continue;
                case 't':
                    str += '\t';
                    pos++;
                    continue;
                case 'u': {
                    uint16_t u16point = readFormattedUnicodePoint(pos);
                    if ((0xD800 <= u16point && u16point < 0xDC00) && (*pos == '\\' && pos[escape] == 'u')) {
                        pos += escape;
                        to8(u16point, readFormattedUnicodePoint(pos), str);
                        continue;
                    }
                    to8(u16point, str);
                    continue;
                }
                    
                case '\\':
                    if (escape < 2) break;
                    pos++;
                    break;
            }
        }
        str += *pos++;
    }
    return str;
}

void listReader::print() {
    std::cout << cur << '\n';
}

std::ostream& operator<<(std::ostream& os, const listReader& obj) {
    size_t dif = 0;
    uint8_t *p = obj.cur;
    while (p != obj.end) {
        switch (*p) {
            case '[':
                dif++;
                break;
            case ']':
                if (!dif) return os;
                dif--;
                break;
            case ',':
                if (!dif) return os;
                break;
            case '"':
                os << *p++;
                while (p != obj.end && *p != '"') {
                    if (*p == '\\') {
                        if ((obj.escape > 1) && p[obj.escape/2] == '"') {
                            for (size_t i = 0; i < obj.escape/2; i++) {
                                os << *p++;
                            }
                            //p += obj.escape/2;
                            break;
                        }
                        for (size_t i = 0; i < obj.escape; i++) {
                            os << *p++;
                        }
                        if (*p == '\\' && obj.escape > 1) {
                            os << *p++;
                        }
                    }
                    os << *p++;
                }
                break;
            case ' ':
            case '\t':
            case '\r':
            case '\n': {
                
                uint8_t c;
                while (
                    (p != obj.end) &&
                    (((c = *p) == '\t') ||
                    (c == '\n') || 
                    (c == '\r') ||
                    (c == ' '))
                ) p++;

                continue;
            }
                
        }
        os << *p++;
    } 
    return os;
}