#pragma once
#include <vector>
#include <map>
#include <chrono>
#include <string>

#include <template.hpp>
#include <variadic.hpp>

namespace util {

    template<size_t N>
    struct StringLiteral {
        constexpr StringLiteral(const char (&str)[N]) {
            std::copy_n(str, N, value);
        }
        
        char value[N];
        size_t size = N - 1;
    };

};