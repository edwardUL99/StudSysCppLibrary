#include <exception>
#include <string>

using std::exception;

class KeyMismatch : public exception {
    private:
        std::string newKey;
        std::string oldKey;
        std::string message;
        
    public:
        KeyMismatch(std::string newKey, std::string oldKey);
        virtual const char* what() const throw() override;
};