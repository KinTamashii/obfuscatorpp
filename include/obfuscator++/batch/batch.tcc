#pragma once
#define MAX_POOL 11
#define REQUEST_SIZE_LIMIT 500000
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include "execute.hpp"
#include "encode.hpp"
#include <util/future.hpp>

namespace {
    /*
        Builds the result string from the list.
    */
    inline std::string buildResult(std::list<data> &texts) {
        std::string result;
        for (auto &item : texts) {
            result += item.text + item.split;
        }
        return result;
    }
};



template <typename ...T>
    requires batch::request_c<T...>
    batch::request_r<T...> batch::request(T... args) {
        // Conditions for different sets of parameters.
        constexpr bool endsWithIntSizeT = variadic::compare<T...>::template ends_with<std::list<data>&, int, size_t>();
        constexpr bool hasDestLang = variadic::isType<int, sizeof...(T)-2, T...>();
        constexpr bool startsWithMFrame = variadic::isType<mFrame*,0,T...>();

        // The texts list must always be either the second or third to last parameter.
        auto &&texts = variadic::get<sizeof...(T)-2-endsWithIntSizeT>(args...);
        
        // A pool of futures that will handle asynchronous requests for large amounts of data.
        // It is limited to a maximum amount of parallel requests to prevent Google from blocking the requests for abuse.
        future::pool_limited<true> F(MAX_POOL);

        /* Handle template conditional return types. */
        auto ret = [&F, &args...]<bool reduce = true>()  {
            if constexpr (reduce) F.reduce(); // Clear the future pool.

            if constexpr (startsWithMFrame) {
                return variadic::get<0>(args...)->IsAsleep(); // Signal termination of the translation if canceled by the user.
            } else {
                return;
            }
        };
        
        // If the text is empty, there is no translations to be done.
        if (texts.empty()) {
            return ret.template operator()<false>();
        }
        
         
        
        auto &&limit = variadic::get<sizeof...(T)-1>(args...); // Maximum request size.
        auto &&dest = static_ternary<hasDestLang>(variadic::get<sizeof...(T)-2>(args...), (int)0); // Destination language.
        std::string query = ""; // Used to build each request.
        auto it = texts.begin(), last = it, // Handle traversal across the list while building the requests.
            end = std::prev(texts.end()); // The last item is handled separately for the format of the request.

        size_t order = 0; // Value to send in the request which allows retrieving the separated translation in order.

        // Action whenever the destination language is used.
        auto getDest = [&dest]() {
            if constexpr (hasDestLang) {
                // If the destination language was included as a parameter, we translate into this language normally.
                return dest;
            } else {
                // If there was no destination language, we randomly chose a language for each individual item in the request.
                return (dest = google::getRandomLanguage());
            }
        };
        use_if<startsWithMFrame, int> counter; // Variable that is only used if the ui is involved.
        if constexpr (startsWithMFrame) counter = variadic::get<0>(args...)->GetProgressValue();

        // Handle updates for the progress gauge.
        auto queue = [&counter, &args...]() {
            if constexpr (startsWithMFrame) {
                variadic::get<0>(args...)->SendEvent(wxEVT_COMMAND_MTHREAD_SET_GAUGE_VALUE, counter);
            }
        };

        // Used to send make the requests to google's batchexecute endpoint.
        batchExecute b("https://translate.google.com");
        
        // Traverse the list.
        while (it != end) {
            // Add a request item.
            query += fmt::format("{}", makeRequest(it->text, order++, getDest(), it->language));
            it->language = dest; // Update the item's language.
            it++; // Iteration.
            counter++; // Update progress.

            // Make the request if we passed the max request size.
            if (query.size() > limit) {
                // Add the request asynchronously.
                // The last and it parameters are used to provide the range
                // of elements to modify after the translation is recieved.
                F.push(std::async(std::launch::async, &batchExecute::translate, &b, std::ref(texts), last, it, query));
                // Exit if the user canceled.
                if constexpr (startsWithMFrame)
                    if (variadic::get<0>(args...)->IsAsleep())
                        return ret();
                queue(); // Update progress ui.
                
                last = it; // The range of elements is now empty.
                order = 0; // Reset the order.
                query.clear(); // Reset the request data.
                continue;
            }
            query += ","; // Signify the addition of another item in the request.
        }
        // Add a request item.
        query += fmt::format("{}", makeRequest(it->text, order++, getDest(), it->language));
        it->language = dest; // Update the item's language.
        it++; // Iteration.
        counter++; // Update progress.
        queue(); // Update progress ui.

        // Preform final request.
        F.push(std::async(std::launch::async, &batchExecute::translate, &b, std::ref(texts), last, it, query));

        return ret();

        
}

template <bool obfuscate, typename ...T>
    requires batch::translateAction_c<T...>
    std::string batch::translateAction(T... args) {
        // Condition for if to interact with the ui.
        constexpr bool startsWithMFrame = variadic::isType<mFrame*,0,T...>();

        auto &&text = variadic::get<sizeof...(T)-3>(args...); // Input string.
        auto &&src = variadic::get<sizeof...(T)-1>(args...); // Language of the input string.
        std::list<data> texts; // Holds the input split into sentences and generally smaller chunks,
        // similar to how Google Translate splits text. An individual item is guaranteed to be less than 5000 characters in length.

        if constexpr (startsWithMFrame) {
            // Wait until the progress guage is set to zero.
            variadic::get<0>(args...)->SendBlockingEvent(wxEVT_COMMAND_MTHREAD_SET_GAUGE_VALUE, 0);
            // Split the text, while using the option the user set to decide
            // whether or not to include not-text characters in the request.
            splitter(&texts, text, src, variadic::get<0>(args...)->IncludeNonText());
        } else
            // Split the text into multiple requests.
            splitter(&texts, text, src, true);

        use_if<obfuscate, int> i;

        // Handles a request differently for ui and non-ui calls.
        auto requesty = [&args..., &texts, &i]<typename ...U>(U&&... sargs) {
            if constexpr (startsWithMFrame) {

                variadic::get<0>(args...)->SendEvent(wxEVT_COMMAND_MTHREAD_SET_GAUGE_VALUE, texts.size()*(int)i);
                variadic::get<0>(args...)->SendEvent(wxEVT_COMMAND_MTHREAD_SET_GAUGE_RANGE, texts.size()*(variadic::get<sizeof...(T)-2>(args...)+1));
                return request(variadic::get<0>(args...), sargs...); // Return boolean value that tells whether to cancel or not.
            } else {
                request(sargs...);
                return false;
            }
        };

        if constexpr (obfuscate) {
            auto &&iterations = variadic::get<sizeof...(T)-2>(args...); // Number of times to randomly translate the text into.
            for (i = 0; i < iterations; i++) {
                
                if (requesty(texts, REQUEST_SIZE_LIMIT))
                    return buildResult(texts); // If ui is involved: return if the user canceled.

                // Split each item after translation in case the amount of text or sentences increased.
                #define split_all_texts(condition) \
                    for ( \
                        auto it = texts.begin(); \
                        it != texts.end(); \
                        splitter(&texts, it, it->text, it->language, condition) \
                    );
                if constexpr (startsWithMFrame) {
                    // IncludeNonText condition based off the option set by the user in the ui.
                    split_all_texts(variadic::get<0>(args...)->IncludeNonText())
                    if (variadic::get<0>(args...)->IsAsleep()) { // Return if the user canceled.
                        return buildResult(texts);
                    }
                } else
                    split_all_texts(true)
                #undef split_all_texts
                
            }
            // Translate back into the source language.
            requesty(texts, src, REQUEST_SIZE_LIMIT);
        } else {
            auto &&dest = variadic::get<sizeof...(T)-2>(args...);
            // Translate into the target language.
            requesty(texts, dest, REQUEST_SIZE_LIMIT);
        }
        
        return buildResult(texts); // Return the result.
    }




#undef MAX_POOL
#undef REQUEST_SIZE_LIMIT