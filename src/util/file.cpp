#include "file.hpp"

namespace file {

    std::string read(fs::path path) {
        // Open the stream to 'lock' the file.
        
        std::ifstream f(path, std::ios::in | std::ios::binary);

        // Obtain the size of the file.
        const auto sz = fs::file_size(path);

        // Create a buffer.
        std::string result(sz, '\0');

        // Read the whole file into the buffer.
        f.read(result.data(), sz);

        return result;
    }
    

    void write(const std::string &str, fs::path path) {
        char buffer[256 * 1024];
        std::ofstream f(path);
        f.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
        f << str;
    }
};


