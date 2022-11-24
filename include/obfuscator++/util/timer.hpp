#pragma once
#include <chrono>


template <typename NUM>
class timer {

    std::chrono::time_point<std::chrono::high_resolution_clock> st;
    public:
    inline void start() {
        st = std::chrono::high_resolution_clock::now();
    }
    template <typename T>
    inline NUM time() {
        return duration_cast<T>(std::chrono::high_resolution_clock::now()-st).count();
    }
};