#include "headers/DatabaseException.h"

DatabaseException::DatabaseException(std::string message) {
    this->message = message;
}

const char * DatabaseException::what() const throw() {
    return message.c_str();
}