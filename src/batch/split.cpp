#include <batch/split.hpp>



template <>
void splitter::parseLetters<true>() {
    
    lastCode = 0;
    do {
        lastCode = code;
    } while (
        code &&
        (readCodepoint<true>()) &&
        !(  
            checkForBreak<true>() ||
            (!unicode::u8::isAnyCodePoint<WS_SPACE>(code) && !isLetter(code))
        )
    );
    
}

template <>
void splitter::parseLetters<false>() {
    
    lastCode = 0;
    do {
        lastCode = code;
        readCodepoint<false>();
        checkForBreak<false>();
    } while (code);
    
}

void splitter::parseNonLetters() {
    
    lastCode = code;
    int counter = 1;
    while (
        code &&
        !isLetter(readCodepoint<true>()) &&
        !checkForBreak<true>()
    ) {
        if (counter == 50) {
            append<true, false>();
            counter = 0;
            continue;
        }
        counter++;
        lastCode = code;
    };
}