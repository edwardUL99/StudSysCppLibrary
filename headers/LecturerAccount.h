#ifndef LECTURER_ACCOUNT_H
#define LECTURER_ACCOUNT_H

#include "DatabaseItem.h"
#include "Account.h"
#include "Lecturer.h"

#include <string>

class LecturerAccount : public DatabaseItem, public Account {
    private:
        Lecturer lecturer;

    public:
        LecturerAccount(Lecturer lecturer, std::string pass);
        virtual ~LecturerAccount() = default;
        Lecturer getLecturer() const;
        std::string getPassword() const;
        void setPassword(std::string pass);
        virtual std::string getObjectType() const override;
        virtual std::string getDescription() const override;
        virtual Tables getTable() const override;
};

#endif