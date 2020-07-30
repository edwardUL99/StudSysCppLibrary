#ifndef EXAM_SUBMISSION_H
#define EXAM_SUBMISSION_H

#include "Student.h"
#include "Exam.h"
#include <map>

class ExamGrade;

//Represents the answers a student gave to an exam
class ExamSubmission {
    private:
        Student student;
        Exam exam;
        //Map in the form of the question and the answer for that question submitted by the person taking the exam
        std::map<ExamQuestion, ExamAnswer> submitted;
        float getMark() const; //calculates the total mark 

        //nothing is implemented yet 

    public:
        /**
         * Initialises this ExamSubmission by filling it with empty answers
         */
        ExamSubmission(const Student &student, const Exam &exam);
        Student getStudent() const;
        Exam getExam() const;
        /**
         * Totals all correct answers and returns the calculated mark
         * If there are any unanswered questions, they are marked 0
         */
        ExamGrade getExamGrade() const;
        /**
         * Use this function to set the answer for the specified question to the answer provided
         * If question exists (the question should have been one from the Exam provided, as they are added to this with "Not answered" answers at construction), the answer is set successfully,
         * else a std::out_of_range is thrown as the question was not found
         */
        void setAnswer(const ExamQuestion &question, const ExamAnswer &answer);
};

#endif