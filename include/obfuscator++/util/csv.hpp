#pragma once

#include <string>
#include <vector>
namespace csv {
    std::string stringify(const std::string &str, size_t off, size_t size);
    bool requiresQuotes(const std::string &str, size_t off, size_t size);
    inline std::string quoteIfNecessary(const std::string &str, size_t off, size_t size) {
        return (requiresQuotes(str, off, size)) ?
                        stringify(str, off, size) : str.substr(off, size);
    }
    class generator {
        std::vector<std::string> data;
        public:
            generator(const std::string &str);
            void add(const std::string &str);

            std::string get();

    };
}