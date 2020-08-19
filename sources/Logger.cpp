#include "headers/Logger.h"
#include "headers/LogFile.h"

using std::string;
using std::vector;
using std::map;
using std::ofstream;
using logging::Logger;
using logging::LogFile;

Logger::Logger(map<string, LogFile> logFiles) {
    initialiseMaps(logFiles);
}

Logger::Logger() {}

Logger::Logger(vector<string> logFileNames, vector<string> logContents) {
    convertVectorsToMap(logFileNames, logContents);
}

void Logger::convertVectorsToMap(const vector<string> &logFileNames, const vector<string> &logContents) {
    bool addEmptyContents = logContents.size() == 0;
    map<string, LogFile> logFiles;

    for (int i = 0; i < logFileNames.size(); i++) {
        string fileName = logFileNames[i];
        string contents = addEmptyContents ? "":logContents[i];
        LogFile file(fileName, contents);
        logFiles.insert(std::pair<string, LogFile>(fileName, LogFile(fileName, contents)));
    }

    initialiseMaps(logFiles);
}

void Logger::initialiseMaps(const map<string, LogFile> &logFiles) {
    this->logFiles = logFiles;
    for (map<string, LogFile>::const_iterator it = logFiles.begin(); it != logFiles.end(); it++) {
        logWriters.insert(std::pair<LogFile, ofstream>(it->second, ofstream(it->first)));
        setLogFileContents(it->first, it->second.getContent());
    }
}

void Logger::addLogFile(LogFile logFile) {
    string fileName = logFile.getFileName();
    logFiles.insert(std::pair<string, LogFile>(fileName, logFile));
    logWriters.insert(std::pair<LogFile, ofstream>(logFile, ofstream(fileName)));
}

void Logger::refreshLogFile(const string &fileName, const string &newContents) {
    map<string, LogFile>::iterator filesIterator = logFiles.find(fileName);

    if (filesIterator != logFiles.end()) {
        LogFile file = filesIterator->second;

        logFiles.erase(filesIterator);

        file.setContent(newContents);
        logFiles[fileName] = file;
    }
}

void Logger::removeLogFile(const string &fileName) {
    map<string, LogFile>::iterator filesIterator = logFiles.find(fileName);

    if (filesIterator != logFiles.end()) {
        map<LogFile, ofstream>::iterator writersIterator = logWriters.find(filesIterator->second);
        logFiles.erase(filesIterator);
        
        if (writersIterator != logWriters.end()) {
            writersIterator->second.close();
            logWriters.erase(writersIterator);
        }
    }
}

boost::optional<LogFile> Logger::findLogFile(const string &fileName) const {
    map<string, LogFile>::const_iterator filesIterator = logFiles.find(fileName);

    if (filesIterator != logFiles.end()) {
        return filesIterator->second;
    }

    return boost::none;
}

bool Logger::setLogFileContents(const std::string &fileName, const std::string &contents) {
    boost::optional<LogFile> fileOptional = findLogFile(fileName);

    if (fileOptional) {
        LogFile file = fileOptional.get();
        file.setContent(contents);
        
        refreshLogFile(fileName, contents);
        logWriters.at(file) << contents;

        return true;
    }

    return false;
}

bool Logger::appendToLogFile(const std::string &fileName, const std::string &line, bool newLine) {
    boost::optional<LogFile> fileOptional = findLogFile(fileName);

    if (fileOptional) {
        LogFile file = fileOptional.get();
        string returnedLine = file.appendToFile(line, newLine);
        
        refreshLogFile(fileName, file.getContent());
        logWriters.at(file) << returnedLine;
        
        return true;
    }

    return false;
}

bool Logger::appendToLogFile(const std::string &fileName, LogTypes type, const std::string &message, bool newLine) {
    static map<LogTypes, string> logTypeMappings;
    if (logTypeMappings.size() == 0) {
        logTypeMappings.insert(std::pair<LogTypes, string>(LogTypes::ERROR, "[ERROR]: "));
        logTypeMappings.insert(std::pair<LogTypes, string>(LogTypes::WARNING, "[WARNING]: "));
        logTypeMappings.insert(std::pair<LogTypes, string>(LogTypes::INFO, "[INFO]: "));
    }

    return appendToLogFile(fileName, logTypeMappings.at(type) + message, newLine);
}

void Logger::flushFile(const std::string &fileName) {
    try {
        logWriters.at(logFiles.at(fileName)).flush();
    } catch (std::out_of_range &ex) {}
}

void Logger::flushAll() {
    for (map<LogFile, ofstream>::iterator it = logWriters.begin(); it != logWriters.end(); it++) {
        it->second.flush();
    }
}