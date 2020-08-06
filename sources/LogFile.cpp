#include "headers/LogFile.h"

using std::string;
using logging::LogFile;

LogFile::LogFile(string fileName, string content) {
    this->fileName = fileName;
    this->content = content;
}

string LogFile::getFileName() const {
    return fileName;
}

void LogFile::setFileName(string fileName) {
    this->fileName = fileName;
}

string LogFile::getContent() const {
    return content;
}

void LogFile::setContent(string content) {
    this->content = content;
}

void LogFile::appendToFile(string line, bool newLine) {
    if (newLine)
        line += "\n";
    this->content += line;
}


