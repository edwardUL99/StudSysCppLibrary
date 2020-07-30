#include "headers/NotAnswerKeyException.h"

NotAnswerKeyException::NotAnswerKeyException(std::string message) {
    this->message = message + " is not an answer key";
}

const char *NotAnswerKeyException::what() const throw() {
    return message.c_str();
}