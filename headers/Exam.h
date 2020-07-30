#ifndef EXAM_H
#define EXAM_H

#include "DatabaseItem.h"
#include "Module.h"
#include "ExamQuestion.h"
#include <vector>

class string;

//may need different sort of exam for the system, with questions
//using this exam but think about how you can store the questions after and restore them

/**
 * Provides a function to provide an exam to the student
 * Currently, it can only be used for multiple-choice style exams
 * Furthermore, in this version of Exam, there does not exist negative marking,
 * so the lecturer will have to manually deduct negative marking for each wrong answer
 * 
 * Note, the Exam class does not provide functionality for "answering" it,
 * that functionality is dedicated to the UI class of ExamPage,
 * which uses this exam class internally
 */
class Exam : public DatabaseItem
{
private:
    int id;
    static int last_id;
    Module module;
    std::string name;
    int year;
    int semester;
    int numberOfQuestions;
    float weightPerQuestion;
    float totalWeight;
    std::vector<ExamQuestion> questions;
    void setWeightPerQuestion();

public:
    /**
     * Use this constructor when you do not know the ID of the exam, i.e. it has not been inserted into the database yet and has not been assigned an autoincremented id
     * By default, this constructor increments Exam's internal auto_increment for the id on C++ side, not database side
     * If you do not want to increment this, pass false as the last parameter
     */
    Exam(Module module, std::string name, int year, int semester, int numberOfQuestions, float totalWeight, std::vector<ExamQuestion> questions = std::vector<ExamQuestion>(), bool auto_increment = true);
    
    /**
     * Use this constructor when the ID of the exam is known. To be used when you are retrieving an exam from the database
     * Does not increment Exam's internal C++ side auto increment
     */
    Exam(int id, Module module, std::string name, int year, int semester, int numberOfQuestions, float totalWeight, std::vector<ExamQuestion> questions = std::vector<ExamQuestion>());
    virtual ~Exam() = default;
    int getID() const;
    static int getLastID();
    static void setLastID(int last_id);
    void setID(int id);
    Module getModule() const;
    void setModule(Module module);
    std::string getName() const;
    void setName(std::string name);
    int getYear() const;
    void setYear(int year);
    int getSemester() const;
    void setSemester(int semester);
    int getNumberOfQuestions() const;
    void setNumberOfQuestions(int numberOfQuestions);
    float getWeightPerQuestion() const;
    float getTotalWeight() const;
    void setTotalWeight(float totalWeight);
    bool addExamQuestion(const ExamQuestion &question);
    std::vector<ExamQuestion> getQuestions() const;
    void setQuestions(std::vector<ExamQuestion> &questions);
    virtual std::string getObjectType() const override;
    virtual std::string getDescription() const override;
    virtual Tables getTable() const override;
    friend class DatabaseManager;
};

#endif // EXAM_H