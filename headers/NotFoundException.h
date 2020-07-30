#include <exception>
#include <string>

using std::exception;

class NotFoundException : public exception {
    private:
        std::string description;
    public:
        NotFoundException(std::string description);
        virtual const char* what() const throw() override;
};