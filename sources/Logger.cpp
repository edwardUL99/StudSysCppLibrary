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
        logFiles.insert(std::pair<string, LogFile>(fileName, contents));
    }

    initialiseMaps(logFiles);
}

void Logger::initialiseMaps(const map<string, LogFile> &logFiles) {
    this->logFiles = logFiles;
    for (map<string, LogFile>::const_iterator it = logFiles.begin(); it != logFiles.end(); it++) {
        logWriters.insert(std::pair<LogFile, ofstream>(it->second, ofstream(it->first)));
    }
}

void Logger::addLogFile(const LogFile &logFile) {
    string fileName = logFile.getFileName();
    logFiles.insert(std::pair<string, LogFile>(fileName, logFile));
    logWriters.insert(std::pair<LogFile, ofstream>(logFile, ofstream(fileName)));
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
        
        removeLogFile(fileName);
        logWriters.insert(std::pair<LogFile, ofstream>(file, ofstream(fileName)));

        logWriters.at(file) << contents;
    }

    return false;
}

bool Logger::appendToLogFile(const std::string &fileName, const std::string &line, bool newLine) {
    boost::optional<LogFile> fileOptional = findLogFile(fileName);

    if (fileOptional) {
        LogFile file = fileOptional.get();
        string returnedLine = file.appendToFile(line, newLine);
        
        removeLogFile(fileName);
        logWriters.insert(std::pair<LogFile, ofstream>(file, ofstream(fileName)));

        logWriters.at(file) << returnedLine;
    }

    return false;
}