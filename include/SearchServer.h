#pragma once
#include "InvertedIndex.h"


class SearchServer {
public:
    SearchServer() = default;

    std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string> &queries_input, std::map<std::string, std::vector<Entry>> _freq_dictionary, int response_limit) const;

    std::vector<RelativeIndex> Relative(std::string queries_input, std::map<std::string, std::vector<Entry>> &_freq_dictionary) const;

    std::vector<std::string> UniqueWords(std::string queries_input, std::map<std::string, std::vector<Entry>> &_freq_dictionary) const;
};