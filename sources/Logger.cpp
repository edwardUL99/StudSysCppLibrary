#include "headers/Logger.h"
#include "headers/LogFile.h"

using logging::LogFile;
using logging::Logger;
using std::map;
using std::ofstream;
using std::string;
using std::vector;

Logger::Logger(map<string, LogFile> logFiles)
{
    initialiseMaps(logFiles);
}

Logger::Logger() {}

Logger::Logger(vector<string> logFileNames, vector<string> logContents)
{
    convertVectorsToMap(logFileNames, logContents);
}

void Logger::convertVectorsToMap(const vector<string> &logFileNames, const vector<string> &logContents)
{
    bool addEmptyContents = logContents.size() == 0;
    map<string, LogFile> logFiles;

    for (int i = 0; i < logFileNames.size(); i++)
    {
        string fileName = logFileNames[i];
        string contents = addEmptyContents ? "" : logContents[i];
        LogFile file(fileName, contents);
        logFiles.insert(std::pair<string, LogFile>(fileName, LogFile(fileName, contents)));
    }

    initialiseMaps(logFiles);
}

void Logger::initialiseMaps(const map<string, LogFile> &logFiles)
{
    this->logFiles = logFiles;
    for (map<string, LogFile>::const_iterator it = logFiles.begin(); it != logFiles.end(); it++)
    {
        logWriters.insert(std::pair<LogFile, ofstream>(it->second, ofstream(it->first, std::ios::app)));
        setLogFileContents(it->first, it->second.getContent());
    }
}

void Logger::addLogFile(LogFile logFile)
{
    string fileName = logFile.getFileName();
    logFiles.insert(std::pair<string, LogFile>(fileName, logFile));
    logWriters.insert(std::pair<LogFile, ofstream>(logFile, ofstream(fileName, std::ios::app)));
}

void Logger::removeLogFile(const string &fileName)
{
    map<string, LogFile>::iterator filesIterator = logFiles.find(fileName);

    if (filesIterator != logFiles.end())
    {
        map<LogFile, ofstream>::iterator writersIterator = logWriters.find(filesIterator->second);
        logFiles.erase(filesIterator);

        if (writersIterator != logWriters.end())
        {
            writersIterator->second.close();
            logWriters.erase(writersIterator);
        }
    }
}

bool Logger::containsLogFile(const std::string &fileName) const
{
    return logFiles.find(fileName) != logFiles.end();
}

boost::optional<LogFile> Logger::findLogFile(const string &fileName) const
{
    map<string, LogFile>::const_iterator filesIterator = logFiles.find(fileName);

    if (filesIterator != logFiles.end())
    {
        return filesIterator->second;
    }

    return boost::none;
}

bool Logger::setLogFileContents(const std::string &fileName, const std::string &contents)
{
    if (containsLogFile(fileName))
    {
        LogFile &logFile = logFiles.at(fileName);
        logFile.setContent(contents);

        ofstream &writer = logWriters.at(logFile);
        writer << contents;
        writer.flush();

        return true;
    }

    return false;
}

bool Logger::appendToLogFile(const std::string &fileName, const std::string &line, bool newLine)
{
    if (containsLogFile(fileName))
    {
        LogFile &logFile = logFiles.at(fileName);
        string returnedLine = logFile.appendToFile(line, newLine);

        ofstream &writer = logWriters.at(logFile);
        writer << returnedLine;
        writer.flush();
        

        return true;
    }

    return false;
}

bool Logger::appendToLogFile(const std::string &fileName, LogTypes type, const std::string &message, bool newLine)
{
    static map<LogTypes, string> logTypeMappings;
    if (logTypeMappings.size() == 0)
    {
        logTypeMappings.insert(std::pair<LogTypes, string>(LogTypes::ERROR, "[ERROR]: "));
        logTypeMappings.insert(std::pair<LogTypes, string>(LogTypes::WARNING, "[WARNING]: "));
        logTypeMappings.insert(std::pair<LogTypes, string>(LogTypes::INFO, "[INFO]: "));
    }

    return appendToLogFile(fileName, logTypeMappings.at(type) + message, newLine);
}

void Logger::flushFile(const std::string &fileName)
{
    try
    {
        logWriters.at(logFiles.at(fileName)).flush();
    }
    catch (std::out_of_range &ex)
    {
    }
}

void Logger::flushAll()
{
    for (map<LogFile, ofstream>::iterator it = logWriters.begin(); it != logWriters.end(); it++)
    {
        it->second.flush();
    }
}