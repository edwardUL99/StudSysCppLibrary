#ifndef EXAM_QUESTION_H
#define EXAM_QUESTION_H

#include <vector>
#include <string>
#include "ExamAnswer.h"
#include "DatabaseItem.h"

/**
 * A class representing an Exam Question and it's answers
 */
class ExamQuestion : public DatabaseItem {
    private:
        int number;
        int examID;
        std::string question;
        ExamAnswer key; //when doing UI and setting up an exam make sure you call ExamANswer("answer", true) to make it a key
        std::vector<ExamAnswer> possibleAnswers;
        int numberOfAnswers;

    public:
        ExamQuestion(int examID, int number, std::string question, ExamAnswer correctAnswer, std::vector<ExamAnswer> possibleAnswers = std::vector<ExamAnswer>(), int numberOfAnswers = 4);
        
        virtual ~ExamQuestion() = default;

        int getExamID() const;

        void setExamID(int examID);

        int getNumber() const;

        void setNumber(int number);

        std::string getQuestion() const;

        void setQuestion(std::string question);

        bool addExamAnswer(const ExamAnswer &answer);

        std::vector<ExamAnswer> getPossibleAnswers() const;
        /**
         * Sets the possible answers for this question with the vector of answers provided
         * Note that this method assumes that answers.size() matches numberOfAnswers
         * There is no logic to check/handle the case where it does not match and therefore this is undefined behaviour and I cannot tell where/when this could cause problems, you've been warned!
         */
        void setPossibleAnswers(std::vector<ExamAnswer> &answers);

        /**
         * Checks if the answer given matches the correctAnswer
         */
        bool checkAnswer(const ExamAnswer &answer) const;

        ExamAnswer getKey() const;

        void setKey(ExamAnswer &key);

        int getNumberOfAnswers() const;

        void setNumberOfAnswers(int numberOfAnswers);

        virtual std::string getObjectType() const override;

        virtual Tables getTable() const override;

        virtual std::string getDescription() const override;
        friend bool operator<(const ExamQuestion &q1, const ExamQuestion &q2);
};

#endif