#ifndef STUDENT_REGISTRATION_H
#define STUDENT_REGISTRATION_H

#include "DatabaseItem.h"
#include "Student.h"
#include "Module.h"

/**
 * A class to represent a student's registration to a certain module
 * Note that the database manager doesn't provide an update method for this as it is a trivial class containing only two data members:
 *  The student and module
 * Therefore it is easiest to remove the old one and insert a new one
 */
class StudentRegistration : public DatabaseItem {
    private:
        Student student;
        Module module;

    public:
        StudentRegistration(const Student &student, const Module &module);
        virtual ~StudentRegistration() = default;
        Student getStudent() const;
        void setStudent(const Student &student);
        Module getModule() const;
        void setModule(const Module &module);
        virtual std::string getObjectType() const override;
        virtual Tables getTable() const override;
        virtual std::string getDescription() const override;
};

#endif