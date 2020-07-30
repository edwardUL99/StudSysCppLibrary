#include "headers/NotFoundException.h"
#include <iostream>

using std::string;

NotFoundException::NotFoundException(string description) {
    this->description = description + " does not exist in the database";
}

const char* NotFoundException::what() const throw() {
    return description.c_str();
}