#include "parse.hpp"
#include <random>
#include <listReader.hpp>
#include <util/arrayToInt.hpp>
#include <sstream>
std::random_device rd;
std::minstd_rand gen(rd()); 

inline std::string getRandomChoice(const std::vector<uint8_t*> &choices) {
    std::uniform_int_distribution random = std::uniform_int_distribution<>(0, choices.size()-1);
    return listReader::getString(choices[random(gen)],0,2);
}


std::string parseResultString(listReader N) {
    
    
    N.travel(3,5,0,0); // Synonyms: Present if the input text was a single word.
    
    if (N.inRange()) {
    
        std::vector<uint8_t*> words;
        for (size_t j = 0; N.inRange();) {
            N.travel(3,5,0,j,1,0,0);
            for (size_t k = 0; N.inRange(); N.travel(3,5,0,j,1,++k,0)) {
                words.push_back(N.pos()); // Add each synonym.
            }
            
            N.travel(3,5,0,++j);
        }
        N.reset(); // Return to the beginning.
        
        N.travel(1,0,0,5,0,4,0,0);
        for (size_t j = 0; N.inRange(); N.travel(1,0,0,5,0,4,++j,0)) {
            words.push_back(N.pos()); // Add each translation.
        }

        return getRandomChoice(words); // Pick a random result.
    }
    
    
    N.reset(); // Return to the beginning.
    
    N.travel(0,4); // Contains the input data, which is only present here if not in the translation.
    
    if (N.inRange()) {
        
        
        std::string result;
        std::vector<uint8_t*> splits; // Contains the separating characters from the input data.
        
        N.travel(0,4,0,0);
        
        // Start at a later index if the first item begins with a newline.
        if (arrayIntCompare<R"("\\n)", uint32_t>(N.pos()+1))  {
            result += N.getString();
            N.travel(0,4,2,0);
            for (size_t j = 2; N.inRange(); N.travel(0,4,(j += 2),0)) {
                splits.push_back(N.pos());
            }
            
        } else {
            N.travel(0,4,1,0);
            for (size_t j = 1; N.inRange(); N.travel(0,4,(j += 2),0)) {
                splits.push_back(N.pos());
            }
        }
        
        
        
        N.travel(1,0,0,5,0); // Go to the translation.
        for (size_t j = 0; N.inRange();) {
            
            std::vector<uint8_t*> statements; // Contains all translations for a statement.
            N.travel(1,0,0,5,j,0);
            
            if (arrayIntCompare<R"("\\n)", uint32_t>(N.pos()+1)) {
                result += N.getString(); // Add seperating chars.
                N.travel(1,0,0,5,++j);
                continue;
            }
            N.travel(1,0,0,5,j,4,0,0);
            
            for (size_t k = 0; N.inRange();  N.travel(1,0,0,5,j,4,++k,0)) {
                if (!arrayIntCompare<R"(\"\")", uint32_t>(N.pos()))
                    statements.push_back(N.pos()); // Add each potential statement.
            }
            
            
            if (statements.size()) result += getRandomChoice(statements); // Randomly pick one of the statements to add.
            if (j < splits.size()) { // Add the seperating chars.
                uint8_t *pos = splits[j];
                if (!(pos[2] == '\\' && pos[3] == '\\' && pos[4] == 'n')) {
                    result += listReader::getString(pos, 0, 2);
                }
            }
            N.travel(1,0,0,5,++j);
        }
        
        return result;
    }
    
    N.travel(1,0,0,5,0,0); // Go to the translation.
    return N.getString(); // Return the translation.
}

#include <google.hpp>
void parse(std::list<data> &results, std::list<data>::iterator start, std::list<data>::iterator end, const std::string &str) {
    std::vector<data*> results_ref; // Store pointers to the list elements to allow random access,
                                    // since the response data is unordered.
    results_ref.reserve(results.size());
    for (auto it = start; it != end; it++) {
        results_ref.push_back(&*it);
    }

    listReader M (&str.front()+6, &str[str.size()]); // Acts a string view that allows iterating over it like a list.
    M.travel(0,1);

    // Iterate through every item in the "list".
    for (size_t i = 0; M.getString() == "MkEWBc"; M.travel(++i, 1)) {
        M.travel(i, 2);
        if (!M.isString())
            continue;
        listReader N = M.getStringObject(); // The translation for the item is stored in a list within a stringified json object.
        M.travel(i, 6); // Go to the index string.
        std::string parsedResult = parseResultString(N);
        size_t pos = base122::decodeString(M.pos(), M.end_ptr()); // Get the index of the item and decode it from base 122.
        
        if ((parsedResult.size()))
            results_ref[pos]->text = parsedResult; // Update the item text with the translation if it is not empty.
    }
}

