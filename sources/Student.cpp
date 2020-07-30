#include "headers/Student.h"

using std::string;

Student::Student(int id, string name, int age, Course course, float qca) : Person(id, name, age, std::to_string(id) + "@student.mail.ie"), course(course) {
    this->qca = qca;
}

float Student::getQCA() const {
    return qca;
}

void Student::setQCA(float qca) {
    this->qca = qca;
}

Course Student::getCourse() const {
    return course;
}

void Student::setCourse(Course course) {
    this->course = course;
}

string Student::getObjectType() const {
    return "Student";
}

string Student::getDescription() const {
    string ret = "ID: " + std::to_string(id) + ", Name: " + name + ", Age: " + std::to_string(age) + ", E-mail: " + email + ", QCA: " + std::to_string(qca) + ", Course: " + course.getID();
    return ret;
}

Tables Student::getTable() const {
    return STUDENTS;
}