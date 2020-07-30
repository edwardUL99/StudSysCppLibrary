#include "headers/Course.h"

using std::string;

Course::Course(string id, string type, string name, int duration, Lecturer course_leader) : course_leader(course_leader) {
    this->id = id;
    this->type = type;
    this->name = name;
    this->duration = duration;
}

string Course::getID() const {
    return id;
}

void Course::setID(string id) {
    this->id = id;
}

string Course::getType() const {
    return type;
}

void Course::setType(string type) {
    this->type = type;
}

string Course::getName() const {
    return name;
}

void Course::setName(string name) {
    this->name = name;
}

int Course::getDuration() const {
    return duration;
}

void Course::setDuration(int duration) {
    this->duration = duration;
}

Lecturer Course::getCourseLeader() const {
    return course_leader;
}

void Course::setCourseLeader(Lecturer course_leader) {
    this->course_leader = course_leader;
}

string Course::getObjectType() const {
    return "Course";
}

string Course::getDescription() const {
    string ret = "ID: " + id + ", Type: " + type + ", Name: " + name + ", Duration: " + std::to_string(duration) + ", Course Leader Name: " + course_leader.getName();
    return ret;
}

Tables Course::getTable() const {
    return COURSES;
}