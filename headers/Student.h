#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"
#include "Course.h"

class Student : public Person
{
private:
    float qca;
    Course course;

public:
    Student(int id, std::string name, int age, Course course, float qca = 0.0);
    virtual ~Student() = default;
    float getQCA() const;
    void setQCA(float qca);
    Course getCourse() const;
    void setCourse(Course course);
    virtual std::string getObjectType() const override;
    virtual std::string getDescription() const override;
    virtual Tables getTable() const override;
    friend class DatabaseManager;
};

#endif // STUDENT_H