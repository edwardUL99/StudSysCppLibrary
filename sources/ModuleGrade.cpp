#include "headers/ModuleGrade.h"

ModuleGrade::ModuleGrade(Module module, Student student, float mark) : module(module), student(student) {
    this->mark = mark;
}

Module ModuleGrade::getModule() const {
    return module;
}

void ModuleGrade::setModule(Module module) {
    this->module = module;
}

Student ModuleGrade::getStudent() const {
    return student;
}

void ModuleGrade::setStudent(Student student) {
    this->student = student;
}

float ModuleGrade::getMark() const {
    return mark;
}

void ModuleGrade::setMark(float mark) {
    this->mark = mark;
}

Tables ModuleGrade::getTable() const {
    return MODULE_GRADES;
}