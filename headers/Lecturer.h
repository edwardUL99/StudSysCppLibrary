#ifndef LECTURER_H
#define LECTURER_H

#include "Person.h"

class Lecturer : public Person
{
private:
    std::string department;

public:
    Lecturer(std::string name, int age, std::string email, std::string department);
    virtual ~Lecturer() = default;
    std::string getDepartment() const;
    //returns a comma seperated list of values to be used when columns (id,name, age, email, department) are used
    void setDepartment(std::string department);
    virtual std::string getObjectType() const;
    virtual std::string getDescription() const;
    virtual Tables getTable() const;
    friend class DatabaseManager;
};

#endif // LECTURER_H