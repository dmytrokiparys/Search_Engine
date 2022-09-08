#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H
#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <string>
#include <mutex>
#include <thread>
#include <map>
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"
#include "Exceptions.h"


struct RelativeIndex{
    size_t document_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (document_id == other.document_id && rank == other.rank);
    }
};

class ConverterJSON {
public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments();

    int GetResponsesLimit();

    std::vector<std::string> GetRequests();

    void putAnswers(std::vector<std::vector<RelativeIndex>> answers);

private:
    int max_responses;
};


#endif //SEARCH_ENGINE_CONVERTERJSON_H