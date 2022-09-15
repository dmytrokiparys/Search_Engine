#include "InvertedIndex.h"

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) const {
    std::vector<Entry> word_count;

    for (int i = 0; i < docs.size(); i++) {
        Entry current_entry;
        current_entry.count = 0;
        std::stringstream temp_stream{docs[i]};
        std::string temp_word;
        int matching_count = 0;

        while (temp_stream >> temp_word) {
            if (temp_word == word) {
                current_entry.doc_id = i;
                current_entry.count += 1;
                matching_count += 1;
            }
        }

        if (matching_count > 0) {
            word_count.push_back(current_entry);
        }
    }
    return word_count;
};


void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = std::move(input_docs);
    std::vector<std::string> unique_words;
    std::vector<std::thread> files_threads;
    std::vector<Entry> temp_vec;

    for (int i = 0; i < docs.size(); i++) {
        std::stringstream current_string{docs[i]};
        std::string current_word;

        while (current_string >> current_word) {
            bool is_unique = true;
            for (auto &unique_word: unique_words) {
                if (unique_word == current_word) {
                    is_unique = false;
                    break;
                }
            }
            if (is_unique) {
                unique_words.push_back(current_word);
            }
        }
    }

    for (int i = 0; i < unique_words.size(); i++) {
        files_threads.emplace_back([&temp_vec, &unique_words, i, this]() {
            temp_vec = GetWordCount(unique_words[i]);
            freq_dictionary.insert(std::make_pair(unique_words[i], temp_vec));
        });
    }

    for (int i = 0; i < files_threads.size(); i++) {
        files_threads[i].join();
    }
}

std::map<std::string, std::vector<Entry>> InvertedIndex::FreqDictionaryReturn() const {
    return freq_dictionary;
};






