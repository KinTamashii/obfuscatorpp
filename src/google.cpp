#include "google.hpp"
#include <preferences.hpp>
namespace google {
    std::random_device rd;
    std::mt19937 gen(rd());
};



int google::getRandomLanguage() {
    std::uniform_int_distribution random = std::uniform_int_distribution(languages::Afrikaans, languages::Zulu);
    int lang;
    while (!pref.getActiveLanguage(lang = random(gen)));
    return lang;
}

int google::getRandomLanguage(int match) {
    
    std::uniform_int_distribution random = std::uniform_int_distribution(languages::Afrikaans, languages::Zulu);
    int lang;
    while (!pref.getActiveLanguage(lang = random(gen)) || lang == match);
    std::cout << match << ',' << lang << ';' << '\n';
    return lang;
}

#include <curlpp/Infos.hpp>
