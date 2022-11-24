#pragma once
#include <type_traits>
#include <tuple>
namespace variadic {

    template <size_t I, class... Ts>
    constexpr decltype(auto) get(Ts&&... ts) {
        return std::get<I>(std::forward_as_tuple(ts...));
    }

    template<size_t N, typename... Ts> using getType =
        typename std::tuple_element<N, std::tuple<Ts...>>::type;

    template<typename U, int N, typename... Ts>
    inline constexpr bool isType() {
        if constexpr (N < sizeof...(Ts)) {
            return std::is_same<getType<N, Ts...>,U>::value;
        } else {
            return false;
        }
    }


    template<typename... First>
    struct compare {
        public:
            template <typename U, int N>
            static inline constexpr bool isType = isType<U, N, First...>();
        private:
            template<size_t Offset, typename... Second, size_t... TIndicies>
            static constexpr bool sub_comparison(std::index_sequence<TIndicies...>) {
            
                return (std::is_same<getType<TIndicies + Offset,First...>,getType<TIndicies,Second...>>::value && ...);
            }
            
        public:
            template<typename... Second>
            static constexpr bool same() {
                if constexpr (sizeof...(First) != sizeof...(Second)) {
                    return false;
                } else {
                    return (std::is_same<First, Second>::value && ...);
                }
            }
        

            template<typename... Second>
            static constexpr bool starts_with() {
                if constexpr (sizeof...(Second) <= sizeof...(First)) {
                        return sub_comparison<0, Second...>(std::make_index_sequence<sizeof...(Second)>{});
                } else {
                    return false;
                }
            }

            template<typename... Second>
            static constexpr bool ends_with() {
                if constexpr (sizeof...(Second) <= sizeof...(First)) {
                    return sub_comparison<sizeof...(First)-sizeof...(Second), Second...>(std::make_index_sequence<sizeof...(Second)>{});
                } else {
                    return false;
                }
            }
    };
    

    
};
