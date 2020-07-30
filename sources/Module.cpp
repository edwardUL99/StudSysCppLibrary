#include "headers/Module.h"

using std::string;

Module::Module(string code, string name, int credits, Lecturer lecturer) : lecturer(lecturer) {
    this->code = code;
    this->name = name;
    this->credits = credits;
    this->lecturer = lecturer;
}

string Module::getCode() const {
    return code;
}

void Module::setCode(string code) {
    this->code = code;
}

string Module::getName() const {
    return name;
}

void Module::setName(string name) {
    this->name = name;
}

int Module::getCredits() const {
    return credits;
}

void Module::setCredits(int credits) {
    this->credits = credits;
}

Lecturer Module::getLecturer() const {
    return lecturer;
}

void Module::setLecturer(Lecturer lecturer) {
    this->lecturer = lecturer;
}

string Module::getObjectType() const {
    return "Module";
}

string Module::getDescription() const {
    string ret = "Code: " + code + ", Name: " + name + ", Credits: " + std::to_string(credits) + ", Lecturer: " + lecturer.getName();
    return ret;
}

Tables Module::getTable() const {
    return MODULES;
}