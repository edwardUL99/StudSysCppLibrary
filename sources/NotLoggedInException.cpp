#include "headers/NotLoggedInException.h"

using std::string;

NotLoggedInException::NotLoggedInException(string message) {
    this->message = message;
}

const char* NotLoggedInException::what() const throw() {
    return message.c_str();
}