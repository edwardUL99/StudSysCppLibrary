#include "headers/ExamSubmission.h"
#include "headers/ExamGrade.h"

using std::string;
using std::map;

ExamSubmission::ExamSubmission(const Student &student, const Exam &exam) : student(student), exam(exam) {
    for (const ExamQuestion &question : exam.getQuestions()) {
        this->submitted.emplace(question, ExamAnswer(exam.getID(), question.getNumber())); //calls default constructor for ExamAnswer (the answer is "Not answered") and asigns it to this question
    }
}

float ExamSubmission::getMark() const {
    float mark = 0;
    float weightPerQuestion = exam.getWeightPerQuestion();

    for (map<ExamQuestion, ExamAnswer>::const_iterator answer = submitted.begin(); answer != submitted.end(); answer++) {
        if (answer->first.checkAnswer(answer->second)) {
            //the answer is correct, so add on the marks to the final mark
            mark += weightPerQuestion;
        }
    }

    return mark;
}

Student ExamSubmission::getStudent() const {
    return student;
}

Exam ExamSubmission::getExam() const {
    return exam;
}

ExamGrade ExamSubmission::getExamGrade() const {
    float mark = getMark();

    return ExamGrade(student, exam, mark);
}

void ExamSubmission::setAnswer(const ExamQuestion &question, const ExamAnswer &answer) {
    try {   
        this->submitted.at(question) = answer;
    } catch (std::out_of_range &o) {
        throw o;
    }
}