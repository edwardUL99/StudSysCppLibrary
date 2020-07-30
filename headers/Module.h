#ifndef MODULE_H
#define MODULE_H

#include "DatabaseItem.h"
#include "Lecturer.h"

class Module : public DatabaseItem
{
private:
    std::string code;
    std::string name;
    int credits;
    Lecturer lecturer;

public:
    Module(std::string code, std::string name, int credits, Lecturer lecturer);
    virtual ~Module() = default;
    std::string getCode() const;
    void setCode(std::string code);
    std::string getName() const;
    void setName(std::string name);
    int getCredits() const;
    void setCredits(int credits);
    Lecturer getLecturer() const;
    void setLecturer(Lecturer lecturer);
    virtual std::string getObjectType() const override;
    virtual std::string getDescription() const override;
    virtual Tables getTable() const override;
    friend class DatabaseManager;
};

#endif // MODULE_H