#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <string>
#include <mutex>
#include <thread>
#include <map>
#include "nlohmann/json.hpp"
#include "Exceptions.h"

struct RelativeIndex {
    size_t document_id;
    float rank;

    bool operator==(const RelativeIndex &other) const {
        return (document_id == other.document_id && rank == other.rank);
    }
};

class ConverterJSON {
public:
    ConverterJSON() = default;

    std::string CheckFile(nlohmann::json &cur_file) const;

    bool IsCorrectRequest(std::string &current_request, int &words_counter, std::vector<std::string> &requests) const;

    std::vector<std::string> GetTextDocuments();

    int GetResponsesLimit() const;

    std::vector<std::string> GetRequests() const;

    void PutAnswers(std::vector<std::vector<RelativeIndex>> answers) const;

private:
    int max_responses;
};