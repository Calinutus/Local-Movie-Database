#pragma once

#include <stdexcept>
#include <string>

class RepositoryException : public std::exception {
public:
   explicit RepositoryException(const std::string &msg): message(msg){}
   const char* what() const noexcept override {
      return message.c_str();
   }
private:
   std::string message;
};
