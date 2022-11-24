#pragma once
#include <string>
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <google.hpp>
#include <batch/base122.hpp>






// Equivilant to preforming a json stringify operation twice.
std::string twiceStringify(const std::string &str);

// Make a request for a translation.
inline std::string makeRequest(const std::string &text, size_t order, int dest, int src=google::languages::Auto) {
    return fmt::format(R"(["MkEWBc","[[\"{}\",\"{}\",\"{}\"]]",,"{}"])", twiceStringify(text), google::getLanguageCode(src), google::getLanguageCode(dest), base122::encode(order));
}