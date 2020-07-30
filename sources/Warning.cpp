#include "headers/Warning.h"
#include <ctime>

using std::string;

Warning::Warning(string error, string query) : error(error), query(query) {
    time_t ttime = time(0);
    this->datetime = ctime(&ttime);
}

string Warning::getError() const {
    return error;
}

string Warning::getDateTime() const {
    return datetime;
}

string Warning::getQuery() const {
    return query;
}

string Warning::toString() const {
    return "WARNING: " + datetime + " " + error + "\n on Query: " + query;
}
