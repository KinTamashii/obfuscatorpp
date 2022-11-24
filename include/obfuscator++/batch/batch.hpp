#pragma once
#include <string>
#include <google.hpp>

#include <list>
#include <util/variadic.hpp>
#include <util/template.hpp>

#include <ui/ui.hpp>
#include <batch/split.hpp>



namespace batch {
    
    template <typename ...T>
    concept request_c = (
        (
            variadic::compare<T...>::template ends_with<std::list<data>&, size_t>() ||
            variadic::compare<T...>::template ends_with<std::list<data>&, int, size_t>()
        ) &&
        (sizeof...(T) <= 4)
    );
    template <typename ...T>
    using request_r = typename std::conditional<variadic::isType<mFrame*, 0, T...>(), bool, void>::type;
    template <typename ...T>
    requires request_c<T...>
    request_r<T...> request(T... args);


    /*
        Make a batch request to Google Translate.
        Large data is split into parallel requests.
        The lack of a destination language means the translated language will be chosen randomly.
        The limit controls the amount of bytes allowed per request.
    */
    inline void request(std::list<data> &texts, size_t limit) {
        return request<std::list<data>&, size_t>(texts, limit);
    }
    /*
        Make a batch request to Google Translate.
        Large data is split into parallel requests.
        The destination language is the language to translate into.
    */
    inline void request(std::list<data> &texts, int dest, size_t limit) {
       return request<std::list<data>&, int, size_t>(texts, dest, limit);
    }
    /*
        Make a batch request to Google Translate.
        Large data is split into parallel requests.
        The lack of a destination language means the translated language will be chosen randomly.
        The frame parameter corresponds to the UI handling the request.
        The limit controls the amount of bytes allowed per request.
    */
    inline bool request(mFrame* frame, std::list<data> &texts, size_t limit) {
        return request<mFrame*, std::list<data>&, size_t>(frame, texts, limit);
    }
    /*
        Make a batch request to Google Translate.
        Large data is split into parallel requests.
        The frame parameter corresponds to the UI handling the request.
        The destination language is the language to translate into.
        The limit controls the amount of bytes allowed per request.
    */
    inline bool request(mFrame* frame, std::list<data> &texts, int dest, size_t limit) {
        return request<mFrame*, std::list<data>&, int, size_t>(frame, texts, dest, limit);
    }

    template <typename ...T>
    concept translateAction_c = (
        variadic::compare<T...>::template ends_with<const std::string &, int, int>()
        &&
        (sizeof...(T) <= 4)
    );
    template <bool obfuscate, typename ...T>
    requires translateAction_c<T...>
    std::string translateAction(T... args);


    /*
        Translate text into a chosen language.
    */
    inline std::string translate(const std::string &text, int dest, int src = google::languages::Auto) {
        return translateAction<false, const std::string &, int, int>(text, dest, src);
    }

    /*
        Obfuscate text by translated it into multiple languages specified
        by the iterations parameter and then back into the src language.
    */
    inline std::string obfuscate(const std::string &text, int iterations, int src) {
        return translateAction<true, const std::string &, int, int>(text, iterations, src);
    }

    /*
        Translate text into a chosen language.
        The frame parameter corresponds to the UI handling the request.
    */
    inline std::string translate(mFrame *frame, const std::string &text, int dest, int src = google::languages::Auto) {
        return translateAction<false, mFrame*, const std::string &, int, int>(frame, text, dest, src);
    }
    /*
        Obfuscate text by translated it into multiple languages specified
        by the iterations parameter and then back into the src language.
        The frame parameter corresponds to the UI handling the request.
    */
    inline std::string obfuscate(mFrame *frame, const std::string &text, int iterations, int src) {
        return translateAction<true, mFrame*, const std::string &, int, int>(frame, text, iterations, src);
    }

};
#include "batch.tcc"