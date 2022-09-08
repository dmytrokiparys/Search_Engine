#include "../include/SearchServer.h"




std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input, std::map<std::string, std::vector<Entry>> _freq_dictionary, int response_limit)       // метод обработки поисковых запросов
{
    std::vector<std::vector<RelativeIndex>> sorted_list;

        for(int i = 0; i < queries_input.size(); i++)            // цикл идет по результатам запросов
        {
            std::vector <std::string> request_unique_words;
            std::vector<RelativeIndex> relative_temp;
            std::string temp_word;
            int max_rel = 0;



                std::stringstream temp_string{queries_input[i]};    // формирует из них уникальные слова

                while (temp_string >> temp_word) {
                    bool is_unique = true;

                    if(_freq_dictionary.count(temp_word) == 0) {
                        is_unique = false;
                    }

                    for (int it = 0; it < request_unique_words.size(); it++) {
                        if (request_unique_words[it] == temp_word) {
                            is_unique = false;
                            break;
                        }
                    }
                    if (is_unique) {
                        std::cout << temp_word << std::endl;
                        request_unique_words.push_back(temp_word);
                    }
                }

                if (!request_unique_words.empty()) {
                    for (int j = 0; j < _freq_dictionary.find(
                            request_unique_words[0])->second.size(); j++)          // заполняем поля структуры RelativeIndex значениями первого слова из списка
                    {
                        RelativeIndex rel;
                        rel.document_id = _freq_dictionary.find(request_unique_words[0])->second[j].doc_id;

                        rel.rank = _freq_dictionary.find(request_unique_words[0])->second[j].count;
                        if (max_rel <
                            rel.rank)                                                                           // вычисляем максимальную релевантность
                        {
                            max_rel = rel.rank;
                        }

                        relative_temp.push_back(rel);                                                             // добавляем эту структуру в вектор
                    }
                }

                for (int k = 1; k < request_unique_words.size(); k++) {
                    for (int j = 0; j < _freq_dictionary.find(
                            request_unique_words[k])->second.size(); j++)                                 // идем по всем словам запроса, кроме первого и сравниваем эти слова с первым
                    {
                        if (_freq_dictionary.find(request_unique_words[k])->second[j].doc_id == relative_temp[j].document_id)          // если эти слова встречаются в одном файле, счетчик увеличивается
                        {
                            relative_temp[j].rank += _freq_dictionary.find(request_unique_words[k])->second[j].count;
                            if (max_rel < relative_temp[j].rank)                                                                           // вычисляем максимальную релевантность
                            {
                                max_rel = relative_temp[j].rank;
                            }
                        } else {
                            RelativeIndex rel;
                            rel.document_id = _freq_dictionary.find(request_unique_words[k])->second[j].doc_id;
                            rel.rank = _freq_dictionary.find(request_unique_words[k])->second[j].count;                // ?? Если в файле будет условно только одно слово из нескольких слов запроса, но встречаться оно будет много раз, это должно влиять на относительную релевантность?
                            relative_temp.push_back(rel);
                        }
                    }
                }

                for (int i = 1; i < relative_temp.size(); ++i)                                     // сортируем вектор с ответами по убыванию релевантности
                {
                    for (int r = 0; r < relative_temp.size() - i; r++) {
                        if (relative_temp[r].rank < relative_temp[r + 1].rank) {
                            float temp = relative_temp[r].rank;
                            relative_temp[r].rank = relative_temp[r + 1].rank;
                            relative_temp[r + 1].rank = temp;
                        }
                    }
                }

                if (relative_temp.size() > response_limit) {
                    relative_temp.resize(response_limit);                          // меняем размер вектора в зависимости от указанного нам значения response_limit
                }

                for (int y = 0; y < relative_temp.size(); y++) {
                    relative_temp[y].rank /= max_rel;                                                                                 // высчитываем относительную релевантность
                    std::cout << "Now: " << relative_temp[y].document_id << "    " << relative_temp[y].rank << std::endl;         // ВРЕМЕННО ДЛЯ ПРОВЕРКИ
                }

                sorted_list.push_back(relative_temp);

                std::cout << "End of request" << std::endl;                                       // ВРЕМЕННО ДЛЯ ПРОВЕРКИ
            }


        for(int t = 0;t<sorted_list.size();t++)
            std::cout<<"size: "<<sorted_list[t].size()<<"    ";                                   // ВРЕМЕННО ДЛЯ ПРОВЕРКИ

        return sorted_list;
    };

