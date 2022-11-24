#pragma once
#include <iostream>
#include <vector>


/* Iterate over a stringified json list. */
class listReader {
    private:
        uint8_t *begin, *cur, *end;
        std::vector<size_t> position;
        size_t escape;
        inline void skipWS() {
            uint8_t c;
            while (
                valid() &&
                (((c = *cur) == '\t') ||
                (c == '\n') || 
                (c == '\r') ||
                (c == ' '))
            ) cur++;
        }
        inline void exit(size_t dif) {
            while (valid() && dif) {
                switch (*cur) {
                    case '[':
                        position.push_back(0); dif++;
                        break;
                    case ']':
                        position.pop_back(); dif--;
                        break;
                    case ',':
                        position.back()++;
                        break;
                    case '"':
                        cur++;
                        while (valid() && *cur != '"') {
                            if (*cur == '\\') {
                                if ((escape > 1) && cur[escape/2] == '"') {
                                    cur += escape/2;
                                    break;
                                }
                                cur += escape;
                                if (*cur == '\\') {
                                    cur++;
                                }
                            }
                            cur++;
                        }
                        break;
                }
                cur++;
            }
        }

        inline bool reach(std::vector<size_t>::const_iterator &l_it) {
            size_t dif = 0;
            size_t index = *l_it;
            if (!index) return false;
            while (valid() && (dif || (position.size() && position.back() != index))) {
                switch (*cur) {
                    case '[':
                        position.push_back(0); dif++;
                        break;
                    case ']':
                        if (!dif) {
                            return true;
                        }
                        position.pop_back(); dif--;
                        break;
                    case ',':
                        position.back()++;
                        break;
                    case '"':
                        cur++;
                        while (valid() && *cur != '"') {
                            if (*cur == '\\') {
                                if ((escape > 1) && cur[escape/2] == '"') {
                                    cur += escape/2;
                                    break;
                                }
                                cur += escape;
                                if (*cur == '\\') {
                                    cur++;
                                }
                            }
                            cur++;
                        }
                        break;
                }
                cur++;
            }
            l_it++;
            return false;
            
        }
    public:
        listReader(const std::string &str, uint8_t depth = 0) {
            setString(str);
            setStringDepth(depth);
        }
        
        inline void setString(const std::string &str) {
            cur = begin = (uint8_t*)str.c_str();
            end = cur + str.size();
        }
        listReader(const char *begin, const char *end, uint8_t depth = 0) {
            setString(begin, end);
            setStringDepth(depth);
        }
        inline void setString(const char *begin, const char *end) {
            cur = this->begin = (uint8_t*)begin;
            this->end = (uint8_t*)end;
        }
        inline bool is(uint8_t val) {
            return *cur == val;
        }
        listReader(uint8_t *begin, uint8_t *end, uint8_t depth = 0) {
            setString(begin, end);
            setStringDepth(depth);
        }
        inline void setString(uint8_t *begin, uint8_t *end) {
            cur = this->begin = begin;
            this->end = end;
        }

        inline void setStringDepth(uint8_t depth) {
            escape = (size_t) 1 << depth;
            //std::cout << escape << '\n';
        }
        inline bool valid() {return cur != end;}
        inline bool invalid() {return cur == end;}

        inline bool isString() {
            return cur[escape-1] == '"';
        }

        inline bool inRange() {
            return *cur != ']' && *cur != ',';
        }
        inline bool outOfRange() {
            return *cur == ']' || *cur == ',';
        }

        inline void reset() {
            cur = begin;
            position.clear();
        }

        listReader getStringObject();
        
        void travel(const std::vector<size_t> &location);
        static std::string getString(uint8_t *pos, uint8_t *end, size_t escape);
        

        inline std::string getString() {
            return getString(cur, end, escape);
        }

        template <typename T>
        static T getNumericString(uint8_t *pos, uint8_t *end, size_t escape) {
            
            pos += escape;
            T val = 0;

            while (pos != end && *pos != '"') {
                if (*pos == '\\') {
                    if ((escape > 1) && pos[escape/2] == '"') {
                        pos += escape/2;
                        return val;
                    }
                    pos += escape;
                    if (pos == end) return val;
                }
                val *= 10;
                val += (*pos++) - 0x30;
            }
            return val;
        }
        template <typename T>
        inline T getNumericString() {
            return getNumericString<T>(cur, end, escape);
        }

        inline bool isNull() {
            return *(uint32_t*)cur == *(uint32_t*)"null";
        }

        template <typename ...T>
        constexpr void travel(T ...args) {
            travel({(size_t)args...});
        }

        inline uint8_t *pos() {
            return cur;
        }
        inline uint8_t *end_ptr() {
            return end;
        }

        void print();
        friend std::ostream& operator<<(std::ostream& os, const listReader& obj);
        
};