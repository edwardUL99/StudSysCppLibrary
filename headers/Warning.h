#ifndef WARNING_H
#define WARNING_H

#include <string>
#include <iostream>

class Warning
{
private:
    std::string error;
    std::string datetime;
    std::string query;

public:
    Warning(std::string error, std::string query);
    std::string getError() const;
    std::string getDateTime() const;
    std::string getQuery() const;
    std::string toString() const;
    friend std::ostream &operator<<(std::ostream &os, const Warning &w)
    {
        os << w.toString();
        return os;
    }
};

#endif // WARNING_H