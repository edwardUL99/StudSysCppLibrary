#include "headers/StudentAccount.h"

using std::string;

StudentAccount::StudentAccount(Student student, string pass) : Account(student.getName(), student.getEmail(), pass), student(student) {
}

Student StudentAccount::getStudent() const {
    return student;
}

string StudentAccount::getPassword() const {
    return pass;
}

void StudentAccount::setPassword(string pass) {
    this->pass = pass;
}

string StudentAccount::getObjectType() const {
    return "StudentAccount";
}

string StudentAccount::getDescription() const {
    string ret = "Student: " + student.getDescription() + ", Password: " + pass;
    return ret;
}

Tables StudentAccount::getTable() const {
    return STUDENT_ACCOUNTS;
}