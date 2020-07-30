#include "headers/Exam.h"

using std::string;
using std::vector;

int Exam::last_id = 1;

Exam::Exam(Module module, string name, int year, int semester, int numberOfQuestions, float totalWeight, vector<ExamQuestion> questions, bool auto_increment) : module(module) {
    this->id = last_id;
    if (auto_increment) last_id++;
    this->name = name;
    this->year = year;
    setSemester(semester);
    this->numberOfQuestions = numberOfQuestions;
    this->totalWeight = totalWeight;
    this->questions = questions;
    setWeightPerQuestion();
}

Exam::Exam(int id, Module module, string name, int year, int semester, int numberOfQuestions, float totalWeight, vector<ExamQuestion> questions) : Exam(module, name, year, semester, numberOfQuestions, totalWeight, questions, false) {
    this->id = id;
}

int Exam::getID() const {
    return id;
}

void Exam::setID(int id) {
    this->id = id;
}

int Exam::getLastID() {
    return last_id;
}

void Exam::setLastID(int last_id) {
    Exam::last_id = last_id;
}

Module Exam::getModule() const {
    return module;
}

void Exam::setModule(Module module) {
    this->module = module;
}

string Exam::getName() const {
    return name;
}

void Exam::setName(string name) {
    this->name = name;
}

int Exam::getYear() const {
    return year;
}

void Exam::setYear(int year) {
    this->year = year;
}

int Exam::getSemester() const {
    return semester;
}

void Exam::setSemester(int semester) {
    if (semester < 1) semester = 1;
    else if (semester > 2) semester = 2;
    
    this->semester = semester;
}

int Exam::getNumberOfQuestions() const {
    return numberOfQuestions;
}

void Exam::setNumberOfQuestions(int numberOfQuestions) {
    this->numberOfQuestions = numberOfQuestions;
}

float Exam::getWeightPerQuestion() const {
    return weightPerQuestion;
}

void Exam::setWeightPerQuestion() {
    this->weightPerQuestion = totalWeight / numberOfQuestions;
}

float Exam::getTotalWeight() const {
    return totalWeight;
}

void Exam::setTotalWeight(float totalWeight) {
    this->totalWeight = totalWeight;
    setWeightPerQuestion();
}

bool Exam::addExamQuestion(const ExamQuestion &question) {
    if (questions.size() < numberOfQuestions) {
        questions.push_back(question);

        return true;
    } else {
        return false;
    }
}

vector<ExamQuestion> Exam::getQuestions() const {
    return questions;
}

void Exam::setQuestions(vector<ExamQuestion> &questions) {
    this->questions = questions;
}

string Exam::getObjectType() const {
    return "Exam";
}

string Exam::getDescription() const {
    string ret = "Module: " + module.getCode() + ", Name: " + name + ", Year: " + std::to_string(year) + ", Semester: " + std::to_string(semester);
    return ret;
}

Tables Exam::getTable() const {
    return EXAMS;
}