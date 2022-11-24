#include "csv.hpp"
std::string csv::stringify(const std::string &str, size_t off, size_t size) {
    std::string s = "\"";
    const char *cur = str.c_str() + off, *end = cur + size;
    while (cur != end) {
        if (*cur == '"') {
            s += "\"\"";
        } else {
            s += *cur;
        }
        cur++;
    }
    return s + '"';
}
bool csv::requiresQuotes(const std::string &str, size_t off, size_t size) {
    const char *cur = str.c_str() + off, *end = cur + size;
    while (cur != end) {
        switch (*cur) {
            case '"':
            case ',':
            case '\n':
            case '\t':
            case ';':
                return true;
                break;
            default:
                break;
        }
        cur++;
    }
    return false;
}


csv::generator::generator(const std::string &str) {
    size_t pos = 0, epos = 0;
    while ((epos = str.find('\n', pos)) != std::string::npos) {
        data.push_back(
            quoteIfNecessary(str, pos, epos-pos)
        );
        pos = epos+1;
    }
    data.push_back(
        quoteIfNecessary(str, pos, str.size()-pos)
    );
}
void csv::generator::add(const std::string &str) {
    auto it = data.begin();
    size_t pos = 0, epos = 0;
    while ((epos = str.find('\n', pos)) != std::string::npos) {
        *it++ += "," + quoteIfNecessary(str, pos, epos-pos);
        pos = epos+1;
    }
    *it += "," + quoteIfNecessary(str, pos, str.size()-pos);
}

std::string csv::generator::get() {
    std::string result;
    auto it = data.begin();
    auto end = data.end()-1;
    while (it != end) {
        result += *it++ + '\n';
    }
    result += *it;
    return result;
}