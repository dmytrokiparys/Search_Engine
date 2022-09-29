#include "ConverterJSON.h"


std::string ConverterJSON::CheckFile(nlohmann::json &cur_file) const {
    std::ifstream current_file(cur_file);
    std::string current_file_content;
    if (!current_file.is_open()) {
        std::cerr << "\nNot found: " << cur_file << std::endl;
    } else {
        std::getline(current_file, current_file_content);

        int words_counter = 0;
        int word_length = 0;

        if (current_file_content[0] >= 97 && current_file_content[0] <= 122) {
            words_counter = 1;
        }

        for (int i = 0; i < current_file_content.size(); i++) {
            if ((current_file_content[i] < 97 || current_file_content[i] > 122) && current_file_content[i] != ' ') {
                std::cerr << "\nIncorrect symbol: " << current_file_content[i] << " in file" << cur_file << std::endl;
                break;
            }

            if ((current_file_content[i] >= 97 && current_file_content[i] <= 122) && current_file_content[i - 1] == ' ') {
                word_length = 0;
                words_counter += 1;
            }

            word_length += 1;

            if (words_counter > 1000) {
                std::cerr << "\nMore than 1000 words in " << cur_file << std::endl;
                break;
            }

            if (word_length > 100) {
                std::cerr << "\nWord in " << cur_file << " is too long" << std::endl;
                break;
            }
        }

        if (word_length <= 100 && words_counter <= 1000 && words_counter > 0) {
            current_file.close();
            return current_file_content;
        }
    }
    current_file_content = "";
    current_file.close();
    return current_file_content;
}


std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::ifstream file("config_files/config.json");

    if (!file.is_open()) {
        throw ConfigFileMissing();
    }

    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size == 0) {
        throw ConfigFileEmpty();
    }

    nlohmann::json dict;
    file >> dict;
    nlohmann::json config = dict["config"];

    if (config == nullptr) {
        throw ConfigHoleEmpty();
    }

    std::string name = config["name"];
    std::cout << "Program: " << name << std::endl;
    std::string version = config["version"];
    for (auto it: version) {
        if ((it < 48 || it > 59) && it != '.') {
            throw IncorrectVersion();
        }
    }

    std::string max_respons = config["max_responses"];
    for (auto it: max_respons) {
        if (it < 48 || it > 59) {
            throw IncorrectMaxRespons();
        }
    }
    max_responses = std::stoi(max_respons);

    nlohmann::json files = dict["files"];
    std::vector<std::string> all_files;

    for (auto it = files.begin(); it != files.end(); it++) {
            all_files.push_back(CheckFile(it.value()));
    }
    file.close();

    return all_files;
};

int ConverterJSON::GetResponsesLimit() const {
    return max_responses;
};


bool ConverterJSON::IsCorrectRequest(std::string &current_request, int &words_counter, std::vector<std::string> &requests) const {
    for (int i = 0; i < current_request.size(); i++) {
        if ((current_request[i] < 97 || current_request[i] > 122) && current_request[i] != ' ') {
            std::cerr << "\nIncorrect symbol in current request: " << current_request << std::endl;
            return false;
        }
        if (current_request[i - 1] == ' ') {
            words_counter++;
        }

        if (words_counter > 10) {
            std::cerr << "\nRequest must be less than 10 words!" << std::endl;
            return false;
        }
    }

    for (int i = 0; i < requests.size(); i++) {
        if (current_request == requests[i]) {
            return false;
        }
    }

    return true;
}

std::vector<std::string> ConverterJSON::GetRequests() const {
    std::vector<std::string> requests;
    std::ifstream file("config_files/requests.json");

    if (!file.is_open()) {
        throw RequestFileMissing();
    }

    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size == 0) {
        throw RequestFileEmpty();
    }

    nlohmann::json dict;
    file >> dict;
    nlohmann::json all_requests = dict["requests"];
    for (auto it = all_requests.begin(); it != all_requests.end(); it++) {
        int words_counter = 0;

        if (requests.size() <= 1000) {
            std::string current_request;
            current_request = it.value();

            if (IsCorrectRequest(current_request, words_counter, requests)) {
                requests.push_back(current_request);
            }
        } else {
            std::cerr << "\nIn this file too much requests!" << std::endl;
            break;
        }
    }

    return requests;
}


void ConverterJSON::PutAnswers(std::vector<std::vector<RelativeIndex>> answers) const {
    std::ofstream file("config_files/answers.json");

    if(file.is_open())
    {
        nlohmann::json all_answers_dict;
        nlohmann::json answer_dict;
        for (int i = 0; i < answers.size(); i++) {
            nlohmann::json current_request_dict;

            if (answers[i].empty()) {
                current_request_dict["result"] = {"false"};
                answer_dict["request" + std::to_string(i + 1)] = current_request_dict;
            } else {
                current_request_dict["result"] = {"true"};
                for (int j = 0; j < answers[i].size(); j++) {
                    current_request_dict["relevance"] += {"docid: ", answers[i][j].document_id, "rank: ",
                                                          answers[i][j].rank};
                }
                answer_dict["request" + std::to_string(i + 1)] = current_request_dict;
            }
        }
        all_answers_dict["answers"] = answer_dict;
        file << all_answers_dict;
    }
    else
    {
        throw AnswersFileError();
    }

    file.close();
}




