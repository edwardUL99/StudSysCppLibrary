#include "headers/Account.h"

using std::string;

Account::Account(string name, string email, string pass) {
    this->name = name;
    this->email = email;
    this->pass = pass;
}

string Account::getName() const {
    return name;
}

void Account::setName(string name) {
    this->name = name;
}

string Account::getEmail() const {
    return email;
}

string Account::getPassword() const {
    return pass;
}

void Account::setPassword(string pass) {
    this->pass = pass;
}