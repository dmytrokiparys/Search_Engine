#pragma once
#include <exception>

class ConfigHoleEmpty : public std::exception {
    const char *what() const noexcept override {
        return "Config hole is empty!";
    }
};

class ConfigFileEmpty : public std::exception {
    const char *what() const noexcept override {
        return "Config file is empty!";
    }
};

class ConfigFileMissing : public std::exception {
    const char *what() const noexcept override {
        return "Config file is missing!";
    }
};

class IncorrectVersion : public std::exception {
    const char *what() const noexcept override {
        return "Incorrect version of program!";
    }
};

class IncorrectMaxRespons : public std::exception {
    const char *what() const noexcept override {
        return "Incorrect quantity of max responses!";
    }
};

class RequestFileMissing : public std::exception {
    const char *what() const noexcept override {
        return "Request file is missing!";
    }
};

class RequestFileEmpty : public std::exception {
    const char *what() const noexcept override {
        return "Request file is empty!";
    }
};

class AnswersFileError : public std::exception {
    const char *what() const noexcept override {
        return "Error while writing answers!";
    }
};

