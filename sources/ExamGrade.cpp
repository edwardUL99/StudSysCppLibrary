#include "headers/ExamGrade.h"

using std::string;

ExamGrade::ExamGrade(Student student, Exam exam, float grade) : student(student), exam(exam) {
    this->grade = grade;
}

Student ExamGrade::getStudent() const {
    return student;
}

void ExamGrade::setStudent(Student student) {
    this->student = student;
}

Exam ExamGrade::getExam() const {
    return exam;
}

void ExamGrade::setExam(Exam exam) {
    this->exam = exam;
}

float ExamGrade::getGrade() const {
    return grade;
}

void ExamGrade::setGrade(float grade) {
    this->grade = grade;
}

string ExamGrade::getObjectType() const {
    return "ExamGrade";
}

string ExamGrade::getDescription() const {
    string ret = "Student ID: " + std::to_string(student.getID()) + ", Exam: " + exam.getName();
    return ret;
}

Tables ExamGrade::getTable() const {
    return EXAM_GRADES;
}