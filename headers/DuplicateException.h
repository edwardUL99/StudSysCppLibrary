#include <exception>
using std::exception;
#include <string>

class DuplicateException : public exception {
    private:
        std::string entityDescription;

    public:
        DuplicateException(std::string description);
        virtual const char *what() const throw() override;
};