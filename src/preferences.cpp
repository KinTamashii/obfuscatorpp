#include "preferences.hpp"

preferences pref;


void preferences::read(const std::string &path) {
    this->path = path;
    this->path += "/Obfuscator++";
    if (!wxDirExists(this->path)) {
        wxMkdir(this->path);
        this->path += "/prefs.txt";
    } else {
        this->path += "/prefs.txt";
        if (wxFileExists(this->path)) {
            decode(file::read(this->path));
        }
    }
}

std::string encodeBase10Literal(int n) {
    std::string str = "";
    do {
        str += (n % 10) + '0';
        n /= 10;
    } while (n);
    std::reverse(str.begin(), str.end());
    return str;
}

std::string preferences::encode() {
    std::string output;
    int i = google::languages::Afrikaans;
    for (auto active : active_languages) {
        std::string name = google::language_names[i];
        output += name + ":";
        for (int i = 0; i < 30-name.size(); i++) {
            output += ' ';
        }
        output += std::string((active) ? "true" : "false");
        if (i == defaultLanguage) output += " default";
        output += '\n';
        i++;
    }
    output += "Default Number of Iterations:  " + encodeBase10Literal(defaultNumIterations) + '\n';
    output += std::string("Include Non-Text:              ") + (includeNonText ? "true" : "false");
    return output;
}


inline bool isWord(const char *cur) {
    return ('A' <= *cur && *cur <= 'Z') || ('a' <= *cur && *cur <= 'z');
}

inline bool isSpace(const char *cur) {
    return *cur == ' ' || *cur == '\t';
}

void findSpace(const char *&cur, const char *end) {
    while (cur != end && *cur != '\n' && *cur != ' ' && *cur != '\t') cur++;
}

std::string getWord(const char *&cur, const char *end) {
    std::string word;
    while (cur != end && isWord(cur)) {
        word += std::tolower(*cur);
        cur++;
    }
    findSpace(cur, end);
    return word;
}

void skipSpace(const char *&cur, const char *end) {
    while (cur != end && isSpace(cur)) cur++;
}

inline bool isNumber(const char *cur) {
    return '0' <= *cur && *cur <= '9';
}

int getNumber(const char *&cur, const char *end) {
    int number = 0;
    while (cur != end && isNumber(cur)) {
        number *= 10;
        number += (*cur) - '0';
        cur++;
    }
    findSpace(cur, end);
    return number;
}

bool preferences::noActiveLanguages() {
    for (auto lang : active_languages) {
        if (lang) return false;
    }
    return true;
}

void preferences::decode(const std::string &input) {
    
    const char *cur = input.c_str(), *end = cur + input.size();
    while (cur != end) {
        std::string str;
        while (cur != end && *cur != ':') {
            if (isWord(cur)) {
                str += std::tolower(*cur);
            }
            cur++;
        }
        cur++;
        

        

        int key = (keys.count(str)) ? keys[str] : 135;
        skipSpace(cur, end);
        if (cur == end) break;
        if (key < 133) {
            while (cur != end && *cur != '\n') {
                if (isWord(cur)) {

                    std::string word = getWord(cur, end);
                    if (word == "default") {
                        defaultLanguage = key + 1;
                    } else {
                        active_languages[key] = (word == "true");
                    }
                } else if (isNumber(cur)) {
                    active_languages[key] = getNumber(cur, end);
                }
                skipSpace(cur, end);
            }
        } else if (key == 133) {
            if (isWord(cur)) {
                defaultNumIterations = getWord(cur, end) == "true";
            } else if (isNumber(cur)) {
                defaultNumIterations = getNumber(cur, end);
            }
            while (cur != end && *cur != '\n') cur++;
        } else if (key == 134) {
            if (isWord(cur)) {
                includeNonText = getWord(cur, end) == "true";
            } else if (isNumber(cur)) {
                includeNonText = getNumber(cur, end);
            }
            while (cur != end && *cur != '\n') cur++;
        } else {
            std::cout << "!" << '\n';
            while (cur != end && *cur != '\n') cur++;
        }

    }
    
    
}
