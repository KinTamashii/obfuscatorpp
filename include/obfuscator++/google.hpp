#pragma once
#include <iostream>
#include <random>

#include <curlpp/Easy.hpp>


namespace google {
    constexpr const char * language_codes[] {"auto", "af", "sq", "am", "ar", "hy", "as", "ay", "az", "bm", "eu", "be", "bn", "bho", "bs", "bg", "ca", "ceb", "ny", "zh-CN", "zh-TW", "co", "hr", "cs", "da", "dv", "doi", "nl", "en", "eo", "et", "ee", "tl", "fi", "fr", "fy", "gl", "ka", "de", "el", "gn", "gu", "ht", "ha", "haw", "iw", "hi", "hmn", "hu", "is", "ig", "ilo", "id", "ga", "it", "ja", "jw", "kn", "kk", "km", "rw", "gom", "ko", "kri", "ku", "ckb", "ky", "lo", "la", "lv", "ln", "lt", "lg", "lb", "mk", "mai", "mg", "ms", "ml", "mt", "mi", "mr", "mni-Mtei", "lus", "mn", "my", "ne", "no", "or", "om", "ps", "fa", "pl", "pt", "pa", "qu", "ro", "ru", "sm", "sa", "gd", "nso", "sr", "st", "sn", "sd", "si", "sk", "sl", "so", "es", "su", "sw", "sv", "tg", "ta", "tt", "te", "th", "ti", "ts", "tr", "tk", "ak", "uk", "ur", "ug", "uz", "vi", "cy", "xh", "yi", "yo", "zu"};
    static const char * language_names[] {
            "Auto",
            "Afrikaans",
            "Albanian",
            "Amharic",
            "Arabic",
            "Armenian",
            "Assamese",
            "Aymara",
            "Azerbaijani",
            "Bambara",
            "Basque",
            "Belarusian",
            "Bengali",
            "Bhojpuri",
            "Bosnian",
            "Bulgarian",
            "Catalan",
            "Cebuano",
            "Chichewa",
            "Chinese (Simplified)",
            "Chinese (Traditional)",
            "Corsican",
            "Croatian",
            "Czech",
            "Danish",
            "Dhivehi",
            "Dogri",
            "Dutch",
            "English",
            "Esperanto",
            "Estonian",
            "Ewe",
            "Filipino",
            "Finnish",
            "French",
            "Frisian",
            "Galician",
            "Georgian",
            "German",
            "Greek",
            "Guarani",
            "Gujarati",
            "Haitian (Creole)",
            "Hausa",
            "Hawaiian",
            "Hebrew",
            "Hindi",
            "Hmong",
            "Hungarian",
            "Icelandic",
            "Igbo",
            "Ilocano",
            "Indonesian",
            "Irish",
            "Italian",
            "Japanese",
            "Javanese",
            "Kannada",
            "Kazakh",
            "Khmer",
            "Kinyarwanda",
            "Konkani",
            "Korean",
            "Krio",
            "Kurdish (Kurmanji)",
            "Kurdish (Sorani)",
            "Kyrgyz",
            "Lao",
            "Latin",
            "Latvian",
            "Lingala",
            "Lithuanian",
            "Luganda",
            "Luxembourgish",
            "Macedonian",
            "Maithili",
            "Malagasy",
            "Malay",
            "Malayalam",
            "Maltese",
            "Maori",
            "Marathi",
            "Meiteilon (Manipuri)",
            "Mizo",
            "Mongolian",
            "Myanmar (Burmese)",
            "Nepali",
            "Norwegian",
            "Odia (Oriya)",
            "Oromo",
            "Pashto",
            "Persian",
            "Polish",
            "Portuguese",
            "Punjabi",
            "Quechua",
            "Romanian",
            "Russian",
            "Samoan",
            "Sanskrit",
            "Scots (Gaelic)",
            "Sepedi",
            "Serbian",
            "Sesotho",
            "Shona",
            "Sindhi",
            "Sinhala",
            "Slovak",
            "Slovenian",
            "Somali",
            "Spanish",
            "Sundanese",
            "Swahili",
            "Swedish",
            "Tajik",
            "Tamil",
            "Tatar",
            "Telugu",
            "Thai",
            "Tigrinya",
            "Tsonga",
            "Turkish",
            "Turkmen",
            "Twi",
            "Ukrainian",
            "Urdu",
            "Uyghur",
            "Uzbek",
            "Vietnamese",
            "Welsh",
            "Xhosa",
            "Yiddish",
            "Yoruba",
            "Zulu"
        };
    namespace languages {
        enum {
            Auto,
            Afrikaans,
            Albanian,
            Amharic,
            Arabic,
            Armenian,
            Assamese,
            Aymara,
            Azerbaijani,
            Bambara,
            Basque,
            Belarusian,
            Bengali,
            Bhojpuri,
            Bosnian,
            Bulgarian,
            Catalan,
            Cebuano,
            Chichewa,
            Chinese_Simplified,
            Chinese_Traditional,
            Corsican,
            Croatian,
            Czech,
            Danish,
            Dhivehi,
            Dogri,
            Dutch,
            English,
            Esperanto,
            Estonian,
            Ewe,
            Filipino,
            Finnish,
            French,
            Frisian,
            Galician,
            Georgian,
            German,
            Greek,
            Guarani,
            Gujarati,
            Haitian_Creole,
            Hausa,
            Hawaiian,
            Hebrew,
            Hindi,
            Hmong,
            Hungarian,
            Icelandic,
            Igbo,
            Ilocano,
            Indonesian,
            Irish,
            Italian,
            Japanese,
            Javanese,
            Kannada,
            Kazakh,
            Khmer,
            Kinyarwanda,
            Konkani,
            Korean,
            Krio,
            Kurdish_Kurmanji,
            Kurdish_Sorani,
            Kyrgyz,
            Lao,
            Latin,
            Latvian,
            Lingala,
            Lithuanian,
            Luganda,
            Luxembourgish,
            Macedonian,
            Maithili,
            Malagasy,
            Malay,
            Malayalam,
            Maltese,
            Maori,
            Marathi,
            Meiteilon_Manipuri,
            Mizo,
            Mongolian,
            Myanmar_Burmese,
            Nepali,
            Norwegian,
            Odia_Oriya,
            Oromo,
            Pashto,
            Persian,
            Polish,
            Portuguese,
            Punjabi,
            Quechua,
            Romanian,
            Russian,
            Samoan,
            Sanskrit,
            Scots_Gaelic,
            Sepedi,
            Serbian,
            Sesotho,
            Shona,
            Sindhi,
            Sinhala,
            Slovak,
            Slovenian,
            Somali,
            Spanish,
            Sundanese,
            Swahili,
            Swedish,
            Tajik,
            Tamil,
            Tatar,
            Telugu,
            Thai,
            Tigrinya,
            Tsonga,
            Turkish,
            Turkmen,
            Twi,
            Ukrainian,
            Urdu,
            Uyghur,
            Uzbek,
            Vietnamese,
            Welsh,
            Xhosa,
            Yiddish,
            Yoruba,
            Zulu
        };
    };
    namespace status {
        enum {
            OK = 200,
            MOVED = 302,
            BAD_REQUEST = 400,
            UNAUTHORIZED = 401,
            FORBIDDEN = 403,
            NOT_FOUND = 404,
            METHOD_NOT_ALLOWED = 405,
            CONFLICT = 409,
            TOO_MANY_REQUESTS = 429,
            CLIENT_CLOSED_REQUEST = 499,
            INTERNAL_SERVER_ERROR = 500,
            NOT_IMPLEMENTED = 501,
            SERVICE_UNAVAILABLE = 503,
            GATEWAY_TIMEOUT = 504
        };
    };
    constexpr const char* getLanguageCode(int index) {
        return language_codes[index];
    }

    int getRandomLanguage();
    int getRandomLanguage(int match);

};