#include "headers/Lecturer.h"

using std::string;

const Lecturer Lecturer::NOT_FOUND = Lecturer("NOT FOUND", -1, "NOT_FOUND@staff.mail.ie", "NOT FOUND");

Lecturer::Lecturer(string name, int age, string email, string department) : Person(-1, name, age, email) {
    this->department = department;
}

string Lecturer::getDepartment() const {
    return department;
}

void Lecturer::setDepartment(string department) {
    this->department = department;
}

string Lecturer::getObjectType() const {
    return "Lecturer";
}

string Lecturer::getDescription() const {
    string ret = "Name: " + name + ", Age: " + std::to_string(age) + ", E-mail: " + email + ", Department: " + department;
    return ret;
}

Tables Lecturer::getTable() const {
    return LECTURERS;
}

bool Lecturer::operator==(const Lecturer &lecturer) {
    return this->email == lecturer.email;
}