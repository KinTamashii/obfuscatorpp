#pragma once
#include <utility>
template <bool COND, typename TrueCase, typename FalseCase>
constexpr auto static_ternary(TrueCase &&t, FalseCase &&f) {
    if constexpr (COND) {
        return std::forward<TrueCase>(t);
    } else {
        return std::forward<FalseCase>(f);
    }
}


#define DEFINE_EMPTY_OP(op) \
    inline constexpr auto operator op (auto &&item) {}






#define DEF_OP_RETURN_SELF(op) \
    inline constexpr auto && operator op() { \
        return *this; \
    }

#define DEF_OP_RETURN_BOOL(op, result) \
    inline constexpr bool operator op() { \
        return result;\
    }

#define DEF_OP_OTHER_RETURN_BOOL(op, result) \
    inline constexpr bool operator op(auto &&other) { \
        return result;\
    }

#define DEF_OP_RETURN_OTHER(op) \
    inline constexpr auto &&operator op(auto &&other) {\
        return other;\
    }

#define DEF_OP_SELF_RETURN_INT(op) \
    inline constexpr auto &&operator op(int) { \
        return *this;\
    }
#define comma ,

struct empty_t {
    template <typename ...T>
    empty_t(T&&... args) {}
    DEF_OP_RETURN_OTHER(+)
    DEF_OP_RETURN_OTHER(-)
    DEF_OP_RETURN_SELF(+)
    DEF_OP_RETURN_SELF(-)
    DEF_OP_RETURN_OTHER(*)
    DEF_OP_RETURN_OTHER(/)
    DEF_OP_RETURN_OTHER(%)
    DEF_OP_RETURN_SELF(++)
    DEF_OP_SELF_RETURN_INT(++)
    DEF_OP_RETURN_SELF(--)
    DEF_OP_SELF_RETURN_INT(--)
    DEF_OP_OTHER_RETURN_BOOL(==, false)
    DEF_OP_OTHER_RETURN_BOOL(!=, true)
    DEF_OP_OTHER_RETURN_BOOL(>, false)
    DEF_OP_OTHER_RETURN_BOOL(<, true)
    DEF_OP_OTHER_RETURN_BOOL(>=, false)
    DEF_OP_OTHER_RETURN_BOOL(<=, true)
    inline constexpr auto && operator <=>(auto &&other) {return -1;}
    DEF_OP_RETURN_BOOL(!, true)
    DEF_OP_OTHER_RETURN_BOOL(&&, false)
    DEF_OP_OTHER_RETURN_BOOL(||, false)
    DEF_OP_RETURN_SELF(~)
    DEF_OP_RETURN_OTHER(&)
    DEF_OP_RETURN_OTHER(|)
    DEF_OP_RETURN_OTHER(^)
    DEF_OP_RETURN_OTHER(<<)
    DEF_OP_RETURN_OTHER(>>)
    DEF_OP_RETURN_OTHER(=)
    DEF_OP_RETURN_OTHER(+=)
    DEF_OP_RETURN_OTHER(-=)
    DEF_OP_RETURN_OTHER(/=)
    DEF_OP_RETURN_OTHER(%=)
    DEF_OP_RETURN_OTHER(&=)
    DEF_OP_RETURN_OTHER(|=)
    DEF_OP_RETURN_OTHER(^=)
    DEF_OP_RETURN_OTHER(<<=)
    DEF_OP_RETURN_OTHER(>>=)
    DEF_OP_RETURN_OTHER([])
    DEF_OP_RETURN_SELF(*)
    DEF_OP_RETURN_SELF(&)
    DEF_OP_RETURN_SELF(->)
    DEF_OP_RETURN_OTHER(->*)
    template <typename ...T>
    inline constexpr auto operator()(T&&... args) {}
    DEF_OP_RETURN_OTHER(comma)
    template <typename T>
    inline constexpr operator T() {return T();}
    inline constexpr void *operator new(size_t x) noexcept {return nullptr;}
    inline constexpr void *operator new[](size_t x) noexcept {return nullptr;}
    inline constexpr void operator delete(void *a) {}
    inline constexpr void operator delete[](void *a) {}
};

template <bool COND, typename T>
using use_if = typename std::conditional<COND, T, empty_t>::type;


#define DEFINE_OPERATOR_IF(op_name, op) \
    template <bool COND> \
    inline constexpr auto op_name##_if (auto &&item, auto &&assignment) { \
        if constexpr (COND) { \
            return item op assignment; \
        } \
    }



template <bool COND, bool isArray = false>
inline constexpr void delete_if(auto &&item) {
    if constexpr (COND) {
        if constexpr (isArray) {
            delete[] item;
        } else {
            delete item;
        }
    }
}

template <bool isArray = false, typename T>
inline constexpr void delete_if_ptr(T &&item) {
    delete_if<std::is_pointer<T>::value, isArray>(item);
}



template <class T, class... Ts>
struct is_any : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};