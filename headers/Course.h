#ifndef COURSE_H
#define COURSE_H

#include "DatabaseItem.h"
#include "Lecturer.h"

class string;

class Course : public DatabaseItem
{
private:
    std::string id;
    std::string type;
    std::string name;
    int duration;
    Lecturer course_leader;

public:
    Course(std::string id, std::string type, std::string name, int duration, Lecturer course_leader);
    virtual ~Course() = default;
    std::string getID() const;
    void setID(std::string id);
    std::string getType() const;
    void setType(std::string type);
    std::string getName() const;
    void setName(std::string name);
    int getDuration() const;
    void setDuration(int duration);
    Lecturer getCourseLeader() const;
    void setCourseLeader(Lecturer course_leader);
    virtual std::string getObjectType() const override;
    virtual std::string getDescription() const override;
    virtual Tables getTable() const override;
    friend class DatabaseManager;
};

#endif // COURSE_H