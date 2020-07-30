#include "headers/LecturerAccount.h"

using std::string;

LecturerAccount::LecturerAccount(Lecturer lecturer, string pass) : Account(lecturer.getName(), lecturer.getEmail(), pass), lecturer(lecturer) {
}

Lecturer LecturerAccount::getLecturer() const {
    return lecturer;
}

string LecturerAccount::getPassword() const {
    return pass;
}

void LecturerAccount::setPassword(string pass) {
    this->pass = pass;
}

string LecturerAccount::getObjectType() const {
    return "LecturerAccount";
}

string LecturerAccount::getDescription() const {
    string ret = "Lecturer: " + lecturer.getDescription() + ", Password: " + pass;
    return ret;
}

Tables LecturerAccount::getTable() const {
    return LECTURER_ACCOUNTS;
}