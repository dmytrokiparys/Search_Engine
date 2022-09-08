#ifndef SEARCH_ENGINE_EXCEPTIONS_H
#define SEARCH_ENGINE_EXCEPTIONS_H
#pragma once

#include <exception>

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


#endif //SEARCH_ENGINE_EXCEPTIONS_H