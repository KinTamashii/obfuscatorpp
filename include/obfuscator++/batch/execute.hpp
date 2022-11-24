#pragma once
#include <string>
#include <data.hpp>
#include "parse.hpp"

class batchExecute { // Handles requests to Google's batchexecute endpoint.
    private:
        size_t id; // Updated for each request.
        std::string uiName;
        std::string backend;
        std::string csrfToken;
        std::string path;  // Base url where the particular batchexecute function is used.
        bool firstRequest = true; // State for if current request is the first.
    public:
        batchExecute(const std::string &url);
        void getParameters();
        
        inline size_t getID() {
            if (firstRequest) {
                firstRequest = false;
                return id;
            }
            return (id += 100000); // Add 100000 if this is not the first request.
        }
        std::string execute(const std::string &payload, const std::string &functions);
        inline void translate(std::list<data> &texts, std::list<data>::iterator start, std::list<data>::iterator end, const std::string &query) {
            const std::string & str = execute(query, "MkEWBc"); // Make the request to the translation function.
            if (str.empty()) { // The request was likely invalid.
                return;
            }
            parse(texts, start, end, str); // Parse the result, updating all items within the
            // range of start and end iterators to their respective translations.
            
        }
};