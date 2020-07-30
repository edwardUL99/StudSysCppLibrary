#ifndef NOT_ANSWER_KEY_H
#define NOT_ANSWER_KEY_H

#include <exception>
#include <string>

using std::exception;

class NotAnswerKeyException : public exception {
    private:
        std::string message;

    public:
        NotAnswerKeyException(std::string message);
        virtual const char * what() const throw() override;
};

#endif