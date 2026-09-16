#pragma once

#include <stdexcept>

class MovieValidatorException: public std::exception {
public:
    explicit MovieValidatorException(const std::string &msg) : message(msg){}
    const char* what() const noexcept override {
        return message.c_str();
    }
private:
    std::string message;
};