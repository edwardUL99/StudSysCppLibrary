#include "headers/Logger.h"
#include "headers/LogWriter.h"
#include "headers/LogFile.h"

using logging::LogFile;
using logging::Logger;
using logging::LogWriter;
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

void Logger::initialiseMaps(const map<string, LogFile> &logFiles) {
    for (map<string, LogFile>::const_iterator it = logFiles.begin(); it != logFiles.end(); it++)
    {
        this->logFiles.insert(std::pair<string, LogWriter>(it->first, LogWriter(it->first, it->second.getContent())));
    }
}

void Logger::addLogFile(LogFile logFile)
{
    string fileName = logFile.getFileName();
    logFiles.insert(std::pair<string, LogWriter>(fileName, LogWriter(fileName, logFile.getContent())));
}

void Logger::removeLogFile(const string &fileName)
{
    map<string, LogWriter>::iterator filesIterator = logFiles.find(fileName);

    if (filesIterator != logFiles.end())
    {
        filesIterator->second.close();
        logFiles.erase(filesIterator);
    }
}

bool Logger::containsLogFile(const std::string &fileName) const
{
    return logFiles.find(fileName) != logFiles.end();
}

boost::optional<LogFile> Logger::findLogFile(const string &fileName) const
{
    map<string, LogWriter>::const_iterator filesIterator = logFiles.find(fileName);

    if (filesIterator != logFiles.end())
    {
        return filesIterator->second.getLogFile();
    }

    return boost::none;
}

bool Logger::setLogFileContents(const string &fileName, const string &contents)
{
    if (containsLogFile(fileName))
    {
        LogWriter &logWriter = logFiles.at(fileName);
        logWriter.setContent(contents);

        return true;
    }

    return false;
}

bool Logger::appendToLogFile(const string &fileName, const string &line, bool newLine)
{
    if (containsLogFile(fileName))
    {
        LogWriter &writer = logFiles.at(fileName);

        if (newLine) {
            writer << line;    
        } else {
            writer.appendToFile(line, false);
        }

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
        logFiles.at(fileName).flush();
    }
    catch (std::out_of_range &ex)
    {
    }
}

void Logger::flushAll()
{
    for (map<string, LogWriter>::iterator it = logFiles.begin(); it != logFiles.end(); it++)
    {
        it->second.flush();
    }
}