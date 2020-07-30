#include "headers/StudentRegistration.h"
#include <sstream>

using std::stringstream;
using std::string;

StudentRegistration::StudentRegistration(const Student &student, const Module &module) : student(student), module(module) {}

Student StudentRegistration::getStudent() const {
    return student;
}

void StudentRegistration::setStudent(const Student &student) {
    this->student = student;
}

Module StudentRegistration::getModule() const {
    return module;
}

void StudentRegistration::setModule(const Module &module) {
    this->module = module;
}

string StudentRegistration::getObjectType() const {
    return "StudentRegistration";
}

Tables StudentRegistration::getTable() const {
    return STUDENT_REGISTRATIONS;
}

string StudentRegistration::getDescription() const {
    stringstream s;
    s << "Student: " << student.getDescription() << ", Module: " << module.getDescription();
    
    return s.str();
}
