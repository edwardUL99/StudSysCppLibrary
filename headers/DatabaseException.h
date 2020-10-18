#ifndef DATABASE_EXCEPTION_H
#define DATABASE_EXCEPTION_H

#include <exception>
#include <string>

class DatabaseException : public std::exception {
    private:
        std::string message;

    public:
        DatabaseException(std::string message);
        virtual const char * what() const throw() override;
};

#endif