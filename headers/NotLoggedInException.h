#ifndef NOT_LOGGED_IN_EXCEPTION_H
#define NOT_LOGGED_IN_EXCEPTION_H

#include <string>

#include <exception>
using std::exception;

class NotLoggedInException : public exception {
    private:
        std::string message;

    public:
        NotLoggedInException(std::string message);
        virtual const char *what() const throw() override;
};

#endif