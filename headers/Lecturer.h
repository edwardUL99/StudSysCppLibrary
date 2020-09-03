#ifndef LECTURER_H
#define LECTURER_H

#include "Person.h"

class Lecturer : public Person
{
private:
    std::string department;

public:
    /**
     * A sentinel value to use in the case a lecturer doesn't exist
     * Doing lecturer == Lecturer::NOT_FOUND can check if it is the sentinel object
     */
    const static Lecturer NOT_FOUND;
    Lecturer(std::string name, int age, std::string email, std::string department);
    virtual ~Lecturer() = default;
    std::string getDepartment() const;
    void setDepartment(std::string department);
    virtual std::string getObjectType() const override;
    virtual std::string getDescription() const override;
    virtual Tables getTable() const override;
    /**
     * Checks lecturer equality by email only.
     * This has been implemented so you can easily do a check if (lecturer == Lecturer::NOT_FOUND)
     */
    bool operator==(const Lecturer &lecturer);
    friend class DatabaseManager;
};

#endif // LECTURER_H