#include "headers/Person.h"

using std::string;

Person::Person(int id, string name, int age, string email) {
    this->id = id;
    this->name = name;
    this->age = age;
    this->email = email;
}

int Person::getID() const {
    return id;
}

void Person::setID(int id) {
    this->id = id;
}

string Person::getName() const {
    return name;
}

void Person::setName(string name) {
    this->name = name;
}

int Person::getAge() const {
    return age;
}

void Person::setAge(int age) {
    this->age = age;
}

string Person::getEmail() const {
    return email;
}

void Person::setEmail(string email) {
    this->email = email;
}