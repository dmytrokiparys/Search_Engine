#include "../include/ConverterJSON.h"




std::vector<std::string> ConverterJSON::GetTextDocuments()          // Метод получения содержимого файлов. Возвращает список с содержимым файлов перечисленных в config.json
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
        std::string version = config["version"];             //?? пока непонятно, откуда мы должны получить версию приложения, чтоб сравнить ее с этим полем

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

                    if((current_file_content[i]>=97 && current_file_content[i]<=122) && current_file_content[i-1] == ' ') // если слова состоят из букв и разделены пробелами, утснавливаем счетчики
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

    int ConverterJSON::GetResponsesLimit()             // Метод возвращает поле max_responses для определения предельного количества ответов на один запрос
    {
        return max_responses;
    };


    std::vector<std::string> ConverterJSON::GetRequests()          // Метод получения запросов из файла requests.json, возвращает список запросов из файла requests.json
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
                std::cerr<<"In this file too much requests!"<<std::endl;                      //?? или все эти проверки лучше как исключения выбрасывать?
                break;
            }
        }

        for(auto & request : requests)                                                          // ВРЕМЕННО смотрим список запросов
        {
            std::cout<<request<<std::endl;
        }
        return requests;
    }


    void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers)             // Метод записывает в файл answers результаты поисковой работы
    {
        std::ofstream file("../answers.json");

        nlohmann::json all_answers_dict;

        nlohmann::json answer_dict;
        for (int i = 0; i<answers.size(); i++)
        {
            nlohmann::json current_request_dict;
            if(answers[i].empty())
            {
                current_request_dict["result"] = {"false"};
                answer_dict["request" + std::to_string(i+1)] = current_request_dict;
            }
            else
            {
                current_request_dict["result"] = {"true"};
                for (int j = 0; j < answers[i].size(); j++) {
                    current_request_dict["relevance"] += {"docid: ", answers[i][j].document_id, "rank: ", answers[i][j].rank};
                }
                answer_dict["request" + std::to_string(i+1)] = current_request_dict;
            }
        }
        all_answers_dict["answers"] = answer_dict;
        file<<all_answers_dict;
        file.close();
    };


