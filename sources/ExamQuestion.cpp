#include "headers/ExamQuestion.h"
#include "headers/NotAnswerKeyException.h"

ExamQuestion::ExamQuestion(int examID, int number, std::string question, ExamAnswer correctAnswer, std::vector<ExamAnswer> possibleAnswers, int numberOfAnswers) : examID(examID), number(number), question(question), key(correctAnswer), possibleAnswers(possibleAnswers), numberOfAnswers(numberOfAnswers) {
    setKey(correctAnswer);
}

int ExamQuestion::getExamID() const {
    return examID;
}

void ExamQuestion::setExamID(int examID) {
    this->examID = examID;
}


int ExamQuestion::getNumber() const {
    return number;
}

void ExamQuestion::setNumber(int number) {
    this->number = number;
}

std::string ExamQuestion::getQuestion() const
{
    return question;
}

void ExamQuestion::setQuestion(std::string question)
{
    this->question = question;
}

bool ExamQuestion::addExamAnswer(const ExamAnswer &answer)
{
    if (possibleAnswers.size() < numberOfAnswers)
    {
        possibleAnswers.push_back(answer);

        return true;
    }
    else
    {
        return false;
    }
}

std::vector<ExamAnswer> ExamQuestion::getPossibleAnswers() const
{
    return possibleAnswers;
}

void ExamQuestion::setPossibleAnswers(std::vector<ExamAnswer> &possibleAnswers) {
    if (possibleAnswers.size() == numberOfAnswers) {
        this->possibleAnswers = possibleAnswers;
    } 
}

bool ExamQuestion::checkAnswer(const ExamAnswer &answer) const
{
    return key == answer;
}

ExamAnswer ExamQuestion::getKey() const
{
    return key;
}

void ExamQuestion::setKey(ExamAnswer &key)
{
    if (key.isKey()) {
        this->key = key;
    } else {
        throw NotAnswerKeyException(key.getAnswer());
    }
}

int ExamQuestion::getNumberOfAnswers() const
{
    return numberOfAnswers;
}

void ExamQuestion::setNumberOfAnswers(int numberOfAnswers)
{
    this->numberOfAnswers = numberOfAnswers;
}

std::string ExamQuestion::getObjectType() const {
    return "ExamQuestion";
}

Tables ExamQuestion::getTable() const {
    return EXAM_QUESTIONS;
}

std::string ExamQuestion::getDescription() const {
    std::string ret = "Number: " + std::to_string(number) + " Question: " + question + ", Key: " + key.getAnswer() + ", Number of Answers: " + std::to_string(numberOfAnswers);
    return ret; 
}

bool operator<(const ExamQuestion &q1, const ExamQuestion &q2) {
    return q1.number < q2.number;
}