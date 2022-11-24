#pragma once
#include <util/util.hpp>
#include <util/unicode.hpp>

#include <abbr.hpp>
#include <google.hpp>
#include <array>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <util/parse.hpp>

#define WS_SPACE " 　"
#define WS_NONSPACE "\r\t\n"
#define WS_ALL WS_SPACE WS_NONSPACE

#include <batch/data.hpp>


/* Splits a string into sentences and smaller statements based off of the language. */
class splitter {
    private:
        uint8_t *begin;
        uint8_t *cbegin;
        uint8_t *cur;
        uint8_t *end;
        uint32_t code;
        uint32_t lastCode;
        int language;
        int codePointCount = 0;
        size_t size = 0;

        std::list<data> *L;
        std::list<data>::iterator it;

        
        inline void append(uint8_t *end) {
            L->insert(it, {std::string(cbegin, end), "", language});
            size += (end-cbegin); // Keep track of the size of the text.
            codePointCount = 0;
        }
        template <bool includeNonLetters, bool skipWS = true>
        inline void append() {
            cur = std::min(cur, end);
            
            append(cur);

            auto temp = std::prev(it); // We will add the split text to the previous iterator.
            if constexpr (skipWS) {
                uint32_t cur_codepoint;
                if constexpr (includeNonLetters) {
                    while (*cur && (unicode::u8::isAnyCodePoint<WS_ALL>((cur_codepoint = unicode::u8::codepoint(&cur))) )) {
                        unicode::u8::append_codepoint(temp->split, cur_codepoint);
                        cbegin = cur;
                    }
                } else {
                    while (*cur && (unicode::u8::isAnyCodePoint<WS_ALL>((cur_codepoint = unicode::u8::codepoint(&cur))) || !isLetter(cur_codepoint) )) {
                        unicode::u8::append_codepoint(temp->split, cur_codepoint);
                        cbegin = cur;
                    }
                }
                
            } else {
                cbegin = cur;
            }
            
        }

        template <bool includeNonLetters>
        inline uint32_t readCodepoint() {
            auto temp = cur;
            code = unicode::u8::codepoint(&temp);
            int codeSize = 1 + (code > 0xFFFF);
            if ((codePointCount + codeSize) >= 5000) { // Split if the text exceeds 5000 codepoints.
                append<includeNonLetters, false>();
            }
            cur = temp;
            codePointCount += codeSize;
            return code;
        }

    public:
        template <bool includeNonLetters = true>
        void constructor(std::list<data> *L, std::list<data>::iterator &it, const std::string &text, int src) {
            src += (src==google::languages::Auto) * google::languages::English; // Default to english if language is unknown.
            this->L = L;
            this->it = it;
            bool orgWasEmpty = L->empty();
            auto prev = std::prev(it);
            
            
            cur = cbegin = begin = (uint8_t*)text.c_str();
            end = text.size() + begin;
            readCodepoint<includeNonLetters>();
            language = src;
            
            if constexpr (includeNonLetters) {
                while (code)
                    isLetter(code) ? parseLetters<true>() : parseNonLetters();
            } else {
                parseLetters<false>();
            }
            
            if (end > cbegin) { // Add the remaining text.

                if (orgWasEmpty) { // If the list was originally empty, append normally.
                    append(end);
                    it++;
                } else if (prev != std::prev(it)) { // Otherwise, if something has been added to the list,
                                                    // erase the item which was the source of the split text.
                    append(end);
                    prev = std::prev(it);
                    prev->split = it->split;
                    it = L->erase(it);
                } else { // If nothing was added to the list, the item was not split, so just iterate past.
                    it++;
                }
                return; 
            }

            if (prev != std::prev(it) && it != L->end()) { // If something has been added to the list,
                                                    // erase the item which was the source of the split text.
                prev = std::prev(it);
                prev->split = it->split;
                it = L->erase(it);
            }
            
        }
        // Allow runtime choice between constructors to include non-letters in the text.
        splitter(std::list<data> *L, std::list<data>::iterator &it, const std::string &text, int src, bool includeNonLetters) {
            (includeNonLetters) ? constructor<true>(L, it, text, src) : constructor<false>(L, it, text, src);
        }

        // Allow runtime choice between constructors to include non-letters in the text.
        splitter(std::list<data> *L, const std::string &text, int src, bool includeNonLetters) {
            auto it = L->begin();
            (includeNonLetters) ? constructor<true>(L, it, text, src) : constructor<false>(L, it, text, src);
        }

        
        // Check whether to split the text here or not.
        template <bool includeNonLetters>
        bool checkForBreak() {
            if (
                (
                    language != google::languages::Chinese_Simplified &&
                    language != google::languages::Chinese_Traditional &&
                    language != google::languages::Japanese
                ) && // Any language that is not Japanese/Chinese has a period as a fullstop.
                code != lastCode &&
                code == unicode::u8::codepoint(".") &&
                !isAbbreviation(cur-1, begin, language) && // Check if the period is the end of a sentence or just an abbreviation.
                (
                    unicode::u8::isAnyCodePoint<WS_ALL>(unicode::u8::codepoint(cur)) ||
                    isNumericSubscript(&cur) // Appears on some sites like wikipedia.
                )
            ) {
                append<includeNonLetters, true>();
                return true;
            }
            
            if (
                (
                    (
                        (
                            language == google::languages::Chinese_Simplified ||
                            language == google::languages::Chinese_Traditional || 
                            language == google::languages::Japanese
                        ) &&
                        code == unicode::u8::codepoint("。")
                    ) ||
                    unicode::u8::isAnyCodePoint<"!?;|">(code)
                ) &&
                unicode::u8::isAnyCodePoint<WS_ALL>(unicode::u8::codepoint(cur))
            ) {
                append<includeNonLetters, true>();
                return true;
            }
            if (unicode::u8::isAnyCodePoint<WS_NONSPACE>(code)) {
                cur--;
                append<includeNonLetters, true>();
                return true;
            }
            
            return false;
        }

        template <bool includeNonLetters>
        void parseLetters();

        void parseNonLetters();


        /* Returns the size of the text present in all of the list excluding the text in splits. */
        inline size_t getTextSize() {
            return size;
        }
};