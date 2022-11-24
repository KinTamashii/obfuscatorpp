#pragma once
#include <string>
#include <list>
#include <batch/data.hpp>
#include <batch/base122.hpp>


class listReader;

/* Returns the translation text for an item in the response data. */
std::string parseResultString(listReader N);

/* Update the items in the range from start to end iterators with the translations present in the response data. */
void parse(std::list<data> &results, std::list<data>::iterator start, std::list<data>::iterator end, const std::string &str);



