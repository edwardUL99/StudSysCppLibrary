#include "headers/DuplicateException.h"
#include <string>

DuplicateException::DuplicateException(std::string description) : entityDescription(description + " already exists in the database") {
}

const char *DuplicateException::what() const throw() {
    return entityDescription.c_str();
}