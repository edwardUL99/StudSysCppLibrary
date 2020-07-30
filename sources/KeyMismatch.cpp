#include "headers/KeyMismatch.h"

using std::string;

KeyMismatch::KeyMismatch(string newKey, string oldKey) {
    this->newKey = newKey;
    this->oldKey = oldKey;
    this->message = "Key " + newKey + " does not match Key " + oldKey;
}

const char* KeyMismatch::what() const throw() {
    return message.c_str();
}