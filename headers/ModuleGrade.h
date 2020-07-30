#include "Module.h"
#include "Student.h"
#include "DatabaseItem.h"

class ModuleGrade : public DatabaseItem
{
private:
    Module module;
    Student student;
    float mark;

public:
    ModuleGrade(Module module, Student student, float mark);
    Module getModule() const;
    void setModule(Module module);
    Student getStudent() const;
    void setStudent(Student student);
    float getMark() const;
    void setMark(float mark);
    virtual std::string getObjectType() const override { return ""; };
    virtual std::string getDescription() const override { return ""; };
    virtual Tables getTable() const override;
};