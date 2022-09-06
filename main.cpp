#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <string>
#include <exception>
#include <mutex>
#include <thread>
#include <map>
#include "gtest/gtest.h"
#include "nlohmann/json.hpp"






class ConfigFileEmpty: public std::exception        // исключение для отсутствия поля config
{
    const char* what() const noexcept override
    {
        return  "Config file is empty!";
    }
};

class ConfigFileMissing: public std::exception       // исключение для отсутствия JSON-файла config
{
    const char* what() const noexcept override
    {
        return  "Config file is missing!";
    }
};


class ConverterJSON {                       // класс для работы с JSON-файлами
    int max_responses;
public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments()          // Метод получения содержимого файлов. Возвращает список с содержимым файлов перечисленных в config.json
    {
        std::ifstream file("../config.json");

        if (!file.is_open())                    // Проверка на наличие JSON-файла config
        {
            throw ConfigFileMissing();
        }


        nlohmann::json dict;
        file>>dict;
        nlohmann::json config = dict["config"];

        if (config == nullptr)            //проверка на наличие поля config
        {
            throw ConfigFileEmpty();
        }

        std::string name = config["name"];
        std::cout<<"Program: "<<name<<std::endl;
        std::string version = config["version"];             // пока непонятно, откуда мы должны получить версию приложения, чтоб сравнить ее с этим полем

        std::string max_respons = config["max_responses"];
        max_responses = std::stoi(max_respons);
        nlohmann::json files = dict["files"];

        std::vector<std::string> all_files;

        for(auto it = files.begin(); it != files.end(); it++)
            {
                std::ifstream current_file(it.value());
                if (!current_file.is_open())                    // проверка на то, существует ли очередной файл из списка
                {
                    std::cerr<<"Not found: "<<it.value()<<std::endl;
                }
                else
                {
                    std::string current_file_content;

                    std::getline(current_file, current_file_content);
                    //current_file>>current_file_content;

                    int words_counter = 0;
                    int word_length = 0;

                    if((current_file_content[0]>=97 && current_file_content[0]<=122))     // проверка, есть ли хоть один символ в файле
                    {
                        words_counter = 1;
                    }

                    for (int i = 0; i<current_file_content.size(); i++)
                    {
                        if((current_file_content[i]<97 || current_file_content[i]>122) && current_file_content[i] != ' ')    // убеждаемся, что слова состоят лишь из строчных латинских букв и разделены пробелами
                        {
                            std::cerr<<"Incorrect symbol: "<<current_file_content[i]<<" in file"<< it.value()<<std::endl;
                            break;
                        }

                        if((current_file_content[i]>=97 && current_file_content[i]<=122) && current_file_content[i-1] == ' ') // проверяем количество слов и длину каждого слова
                        {
                                word_length = 0;
                                words_counter += 1;
                        }

                        word_length +=1;

                        if (words_counter > 1000)      // проверка на количество слов в тексте файла
                        {
                            std::cerr<<"More than 1000 words in "<<it.value()<<std::endl;
                            break;
                        }


                        if(word_length>100)           // проверка на длину слов
                        {
                            std::cerr<<"Word in "<<it.value()<<" is too long"<<std::endl;
                            break;
                        }
                    }

                    if(word_length <= 100 && words_counter <= 1000  && words_counter>0)
                    {
                        all_files.push_back(current_file_content);            // закидываем в вектор содержимое очередного файла
                    }
                    current_file.close();
                }
            }
        file.close();

        for (auto & all_file : all_files)            // ВРЕМЕННО просматриваем файлы из вектора
            {
                std::cout<<all_file<<std::endl;
            }
        return all_files;                                         // возвращаем все содержимое файлов
    };

    int GetResponsesLimit() const            // Метод считывает поле max_responses для определения предельного количества ответов на один запрос
    {
        return max_responses;
    };




    std::vector<std::string> GetRequests()          // Метод получения запросов из файла requests.json, возвращает список запросов из файла requests.json
    {
        std::vector<std::string> requests;
        std::ifstream file ("../requests.json");
        nlohmann::json dict;
        file>>dict;
        nlohmann::json all_requests = dict["requests"];
        for(auto it = all_requests.begin(); it!=all_requests.end(); it++)
        {
            int words_counter = 0;
            bool is_correct_request = true;
            if(requests.size()<=1000)
            {
                std::string current_request;
                current_request = it.value();
                for(int i = 0; i< current_request.size(); i++)
                {
                    if((current_request[i] < 97 || current_request[i] >122) && current_request[i] != ' ')         // проверка того, что запрос состоит из строчных латинских букв и пробелов
                    {
                        std::cerr<<"Incorrect symbol in current request: "<<current_request[i]<<std::endl;
                        is_correct_request = false;
                        break;
                    }
                    if(current_request[i-1] == ' ')                                                                 // считаем количество слов
                    {
                        words_counter++;
                    }

                    if (words_counter>10)                                                                         // если в запросе больше 10 слов, выходит ошибка
                    {
                        std::cerr<<"Request must be less than 10 words!"<<std::endl;
                        is_correct_request = false;
                        break;
                    }
                }

                for(int i = 0; i<requests.size(); i++)                              // проверка на уникальность запроса
                {
                    if (current_request == requests[i])
                    {
                        is_correct_request = false;
                        break;
                    }
                }

                if(is_correct_request)
                {
                    requests.push_back(current_request);
                }
            }
            else
            {
                std::cerr<<"In this file too much requests!"<<std::endl;                      // или все эти проверки лучше как исключения выбрасывать?
                break;
            }
        }

        for(auto & request : requests)                                                          // ВРЕМЕННО
        {
            std::cout<<request<<std::endl;
        }
        return requests;
    };
/**
* Положить в файл answers.json результаты поисковых запросов
*/
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
    {

    };
};




struct Entry {
    size_t doc_id, count;

    bool operator ==(const Entry& other) const { // Данный оператор необходим для проведения тестовых сценариев
        return (doc_id == other.doc_id && count == other.count);
    }
};


std::mutex searching_access;      // добавляем mutex. Будем его использовать при индексации файлов в отдельных потоках


class InvertedIndex {
public:
    InvertedIndex() = default;

    std::vector<Entry> GetWordCount(const std::string &word)           // Метод определяет количество вхождений слова word в загруженной базе документов
    {
        std::vector<Entry> word_count;                       // вектор, который мы будем возвращать

        for(int i = 0; i<docs.size(); i++)
        {
            Entry current_entry;
            current_entry.count = 0;
            std::stringstream temp_stream{docs[i]};
            std::string temp_word;
            int matching_count = 0;
            while(temp_stream>>temp_word)                  // если найдено совпадение, указываем номер документа и считаем, сколько раз слово там встретилось
            {
                if(temp_word == word)
                {
                    current_entry.doc_id = i;
                    current_entry.count += 1;
                    matching_count +=1;
                }
            }
            if(matching_count>0)                              // если в файле было найдено хоть одно совпадение, добавляем очередную структуру в вектор
            {
                searching_access.lock();
                word_count.push_back(current_entry);
                searching_access.unlock();
            }
        }
        return word_count;
    };

    void UpdateDocumentBase(std::vector<std::string> input_docs)                       // Обновить или заполнить базу документов, по которой будем совершать поиск. Базу берем из предыдущего класса
    {
        docs = std::move(input_docs);
        std::vector<std::thread> filess(docs.size());                               // создаем отдельный поток для каждого файла

        for(int i = 0; i<docs.size(); i++)
        {
            filess[i] = std::thread([&]()                                     // здесь нужно будет каждое слово выделить и отправить в метод GetWordCount
            {
                std::stringstream current_string{docs[i]};                         // создаем поток для нахождения отдельных слов
                std::string current_word;
                while(current_string>>current_word)                                   // проверяем мапу на наличие очередного слова из списка
                {
                    if(!freq_dictionary.count(current_word))                       // если такое слово раньше не встречалось, вызываем функцию GetWordCount и кладем ее вместе со словом в мапу
                    {
                        freq_dictionary.insert(std::make_pair(current_word, GetWordCount(current_word)));
                    }
                }
            });
            filess[i].join();
        }
         /*for(int j = 0;j<docs.size();j++)               // изначально пытался присоединить потоки через вектор, но выдает ошибку. Не понимаю, с чем это может быть связано
         {
             filess[j].join();
         }*/

        for (const auto & it : freq_dictionary)               // ВРЕМЕННО ДЛЯ ПРОВЕРКИ
        {
            std::cout << it.first<<" ";
            for(int i=0;i<it.second.size();i++)
            {
                std::cout<<"{"<<it.second[i].doc_id<<" "<<it.second[i].count<<"} ";
            }
            std::cout<<std::endl;
        }
    };

    std::map<std::string, std::vector<Entry>> freq_dictionary_return()        // метод, который возвращает частотный словарь
    {
        return freq_dictionary;
    }

private:
    std::vector<std::string> docs; // список содержимого документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // частотный словарь
};






struct RelativeIndex{
    size_t doc_id;
    float rank;

    bool operator ==(const RelativeIndex& other) const
    {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class SearchServer {
public:

    SearchServer(InvertedIndex& idx) : _index(idx){};           // в конструктор класса передаётся ссылка на класс InvertedIndex, чтобы SearchServer мог узнать частоту слов встречаемых в запросе
/**
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input) {                  // метод обработки поисковых запросов

        std::vector<std::vector<RelativeIndex>> sorted_list;
        std::vector<std::map<std::string, std::vector<Entry>>> sorted_dictionary;
        std::vector<RelativeIndex> relative_temp;

        for(int i = 0; i < queries_input.size(); i++)            // цикл идет по результатам запросов
        {
            std::string temp_word;
            std::stringstream temp_string{queries_input[i]};    // формирует из них уникальные слова
            std::vector<size_t> relatives;
            std::vector <std::string> request_unique_words;

            while(temp_string>>temp_word)
            {
                bool is_unique = true;
                for(int it = 0; it < request_unique_words.size(); it++)
                {
                    if(request_unique_words[it] == temp_word)
                    {
                        is_unique = false;
                        break;
                    }
                }
                if(is_unique)
                {
                    request_unique_words.push_back(temp_word);
                }
            }

            /*for(int j = 0;j<_index.freq_dictionary_return().find(request_unique_words[0])->second.size();j++)
            {
                RelativeIndex rel;
                rel.doc_id = _index.freq_dictionary_return().find(request_unique_words[0])->second[j].doc_id;
                rel.rank = _index.freq_dictionary_return().find(request_unique_words[0])->second[j].count;
                relative_temp.push_back(rel);
            }*/

            /*for(int k = 0; k<request_unique_words.size(); k++)
            {
                for(int j = 0;j<_index.freq_dictionary_return().find(request_unique_words[k])->second.size();j++)
                {
                    size_t relative = 0;
                    relative += _index.freq_dictionary_return().find(temp_word)->second[j].count;
                }

            }*/


        }

        return sorted_list;
    };
private:
    InvertedIndex _index;
};


int main() {



ConverterJSON converter;
InvertedIndex index;
SearchServer search_server(index);
try
{
    index.UpdateDocumentBase(converter.GetTextDocuments());
    search_server.search(converter.GetRequests());
}
catch (const std::exception& x)
{
    std::cerr<<"Caught exception: "<<x.what()<<std::endl;
}

}
