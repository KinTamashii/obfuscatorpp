#pragma once
#include <future>
#include <chrono>
#include <list>
namespace future {

    template <typename T>
    inline bool is_ready(const std::future<T> &f) {
        return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    template <typename T>
    void until_size(std::list<std::future<T>> &L, size_t max) {
        auto it = L.begin();
        while (L.size() > max) {
            if (is_ready(*it)) {
                it = L.erase(it);
            } else it++;
            if (it == L.end()) it++;
        }
    }

    template <bool limit> struct pool_data;

    template<>
    struct pool_data<true> {
        std::list<std::future<void>> futures;
        size_t max;
    };
    template<>
    struct pool_data<false> {
        std::list<std::future<void>> futures;
    };
    
    


    template <bool limit, bool debug = false>
    class pool : public pool_data<limit> {
            
        public:
            template <bool U = !limit> requires U
            pool() {

            }
            template <bool U = limit> requires U
            pool(size_t max) {
                this->max = max;
            }
            
            void reduce(size_t max = 0) {
                auto it = this->futures.begin();
                while (this->futures.size() > max) {
                    if (is_ready(*it)) {
                        if constexpr (debug) {
                            std::cout << this->futures.size() << '\n';
                        }
                        it = this->futures.erase(it);
                    } else it++;
                    if (it == this->futures.end()) it++;
                }
            }


            inline void push(std::future<void> &&item) {
                if constexpr (limit) reduce(this->max);
                this->futures.push_back(std::move(item));
            }

            
            inline void push_after(std::future<void> &&item, size_t max) {
                reduce(max);
                this->futures.push_back(std::move(item));
            }   
    };
    template <bool debug>
    using pool_limited = class pool<true, debug>;
    template <bool debug>
    using pool_unlimited = class pool<false, debug>;

    

}