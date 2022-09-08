#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H
#pragma once
#include "InvertedIndex.h"


class SearchServer {
public:

    SearchServer() = default;

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input, std::map<std::string, std::vector<Entry>> _freq_dictionary, int response_limit);
};

#endif //SEARCH_ENGINE_SEARCHSERVER_H