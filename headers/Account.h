#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include "DatabaseItem.h"

class Account : public DatabaseItem {
    protected:
        std::string name;
        std::string email;
        std::string pass;

    public:
        Account(std::string name, std::string email, std::string pass);
        virtual ~Account() = default;
        std::string getName() const; 
        void setName(std::string name);
        std::string getEmail() const;
        std::string getPassword() const;
        void setPassword(std::string pass);
};

#endif
