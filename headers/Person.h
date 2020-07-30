#ifndef PERSON_H
#define PERSON_H

#include "DatabaseItem.h"

class Person : public DatabaseItem
{
protected:
    //have an id field that has auto increment, if different with student and lecturer keep id and auto incr. in each subclass
    int id;
    std::string name;
    int age;
    std::string email;
    Person(int id, std::string name, int age, std::string email);

public:
    virtual ~Person() = default;
    std::string getName() const;
    int getID() const;
    void setID(int id);
    void setName(std::string name); //maybe have string methods return strings with '' around them since they are only being used in the database as database namespace
    int getAge() const;
    void setAge(int age);
    std::string getEmail() const;
    void setEmail(std::string email);
};

#endif // PERSON_H