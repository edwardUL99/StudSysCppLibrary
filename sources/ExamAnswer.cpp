#include "headers/ExamAnswer.h"

using std::string;

ExamAnswer::ExamAnswer(int examID, int question, string answer, bool key) : examID(examID), question(question), answer(answer), key(key) {}

ExamAnswer::ExamAnswer(int examID, int question, int answer, bool key) : ExamAnswer(examID, question, std::to_string(answer), key) {}

ExamAnswer::ExamAnswer(int examID, int question, float answer, bool key) : ExamAnswer(examID, question, std::to_string(answer), key) {}

int ExamAnswer::getExamID() const {
    return examID;
}

void ExamAnswer::setExamID(int examID) {
    this->examID = examID;
}

int ExamAnswer::getQuestion() const {
    return question;
}

void ExamAnswer::setQuestion(int question) {
    this->question = question;
}

string ExamAnswer::getAnswer() const {
    return answer;
}

void ExamAnswer::setAnswer(string answer) {
    this->answer = answer;
}

void ExamAnswer::setAnswer(int answer) {
    this->setAnswer(std::to_string(answer));
}

void ExamAnswer::setAnswer(float answer) {
    this->setAnswer(std::to_string(answer));
}

bool ExamAnswer::isKey() const {
    return key;
}

void ExamAnswer::setKey(bool key) {
    this->key = key;
}

std::string ExamAnswer::getObjectType() const {
    return "ExamAnswer";
}

Tables ExamAnswer::getTable() const {
    return EXAM_ANSWERS;
}

std::string ExamAnswer::getDescription() const {
    if (key) {
        return "Key: " + answer;
    } else {
        return "Answer: " + answer;
    }
}

bool ExamAnswer::operator==(const ExamAnswer &answer) const {
    return this->answer == answer.answer;
}

std::ostream &operator<<(std::ostream &os, const ExamAnswer &answer) {
    os << answer.answer;
    return os;
}
