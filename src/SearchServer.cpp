#include "SearchServer.h"

int max_rel;

std::vector<std::string> SearchServer::UniqueWords(std::string queries_input, std::map<std::string, std::vector<Entry>> &_freq_dictionary) const {
    std::vector<std::string> request_unique_words;
    std::stringstream temp_string{queries_input};
    std::string temp_word;

    while (temp_string >> temp_word) {
        bool is_unique = true;

        if (_freq_dictionary.count(temp_word) == 0) {
            is_unique = false;
        }

        for (int it = 0; it < request_unique_words.size(); it++) {
            if (request_unique_words[it] == temp_word) {
                is_unique = false;
                break;
            }
        }
        if (is_unique) {
            request_unique_words.push_back(temp_word);
        }
    }
    return request_unique_words;
}

std::vector<RelativeIndex>
SearchServer::Relative(std::string queries_input, std::map<std::string, std::vector<Entry>> &_freq_dictionary) const {
    std::vector<RelativeIndex> relative_temp;
    max_rel = 0;
    std::vector<std::string> unique_words = UniqueWords(queries_input, _freq_dictionary);

    if (!unique_words.empty()) {
        for (int i = 0; i < unique_words.size(); i++) {
            for (int j = 0; j < _freq_dictionary.find(unique_words[i])->second.size(); j++) {
                RelativeIndex rel;
                rel.document_id = _freq_dictionary.find(unique_words[i])->second[j].doc_id;
                rel.rank = _freq_dictionary.find(unique_words[i])->second[j].count;
                if (max_rel < rel.rank) {
                    max_rel = rel.rank;
                }
                bool is_contain_id = true;
                for (int k = 0; k < relative_temp.size(); k++) {
                    if (relative_temp[k].document_id == _freq_dictionary.find(unique_words[i])->second[j].doc_id) {
                        is_contain_id = false;
                        relative_temp[k].rank += _freq_dictionary.find(unique_words[i])->second[j].count;
                        if (max_rel < relative_temp[k].rank) {
                            max_rel = relative_temp[k].rank;
                        }
                    }
                }
                if (is_contain_id) {
                    relative_temp.push_back(rel);
                }
            }
        }
    }

    return relative_temp;
}

std::vector<std::vector<RelativeIndex>> SearchServer::Search(const std::vector<std::string> &queries_input, std::map<std::string, std::vector<Entry>> _freq_dictionary, int response_limit) const {
    std::vector<std::vector<RelativeIndex>> sorted_list;

    for (int i = 0; i < queries_input.size(); i++) {
        std::vector<RelativeIndex> relative_ind = Relative(queries_input[i], _freq_dictionary);
        for (int j = 1; j < relative_ind.size(); ++j) {
            for (int r = 0; r < relative_ind.size() - j; r++) {
                if (relative_ind[r].rank < relative_ind[r + 1].rank) {
                    auto temp = relative_ind[r];
                    relative_ind[r] = relative_ind[r + 1];
                    relative_ind[r + 1] = temp;
                }
            }
        }

        if (relative_ind.size() > response_limit) {
            relative_ind.resize(response_limit);
        }

        for (int y = 0; y < relative_ind.size(); y++) {
            relative_ind[y].rank /= max_rel;
        }

        sorted_list.push_back(relative_ind);
    }
    return sorted_list;
};

