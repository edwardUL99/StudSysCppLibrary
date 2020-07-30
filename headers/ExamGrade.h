#ifndef EXAM_GRADE_H
#define EXAM_GRADE_H

#include "DatabaseItem.h"
#include "Student.h"
#include "Exam.h"

class ExamGrade : public DatabaseItem
{
private:
    Student student;
    Exam exam;
    float grade;

public:
    ExamGrade(Student student, Exam exam, float grade);
    virtual ~ExamGrade() = default;
    Student getStudent() const;
    void setStudent(Student student);
    Exam getExam() const;
    void setExam(Exam exam);
    float getGrade() const;
    void setGrade(float grade);
    virtual std::string getObjectType() const override;
    virtual std::string getDescription() const override;
    virtual Tables getTable() const override;
    friend class DatabaseManager;
};

#endif // EXAM_GRADE_H