#include "../include/InvertedIndex.h"





std::mutex searching_access;      // добавляем mutex. Будем его использовать при индексации файлов в отдельных потоках


std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word)           // Метод определяет количество вхождений слова word в загруженной базе документов
{
        std::vector<Entry> word_count;                                            // вектор, который мы будем возвращать

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

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)                       // Обновить или заполнить базу документов, по которой будем совершать поиск. Базу берем из предыдущего класса
{
        docs = std::move(input_docs);
        std::vector<std::thread> files_threads(docs.size());                               // создаем отдельный поток для каждого файла

        for(int i = 0; i<docs.size(); i++)
        {
            files_threads[i] = std::thread([&]()                                     // здесь нужно будет каждое слово выделить и отправить в метод GetWordCount
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
            files_threads[i].join();
        }
        /*for(int j = 0;j<docs.size();j++)               //?? изначально пытался присоединить потоки через вектор, но выдает ошибку. Не подскажете, с чем это может быть связано?
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

std::map<std::string, std::vector<Entry>> InvertedIndex::freq_dictionary_return()        // метод, который возвращает частотный словарь
{
    return freq_dictionary;
};





