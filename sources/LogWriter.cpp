#include "headers/LogWriter.h"

using std::ofstream;
using std::string;
using logging::LogFile;
using logging::LogWriter;

LogWriter::LogWriter(string filename, bool append) {
    logFile = LogFile(filename);
    init(filename, append);
}

LogWriter::LogWriter(string filename, string contents, bool append) {
    logFile = LogFile(filename, contents);
    if (contents == "")
        init(filename, append);
    else 
        init(filename, contents, append);
}

void LogWriter::init(string filename, bool append) {
    std::ios_base::openmode mode = append ? std::ios_base::app:std::ios_base::out;
    out = ofstream(filename, mode);
}

void LogWriter::init(string filename, string contents, bool append) {
    std::ios_base::openmode mode = append ? std::ios_base::app:std::ios_base::out;
    out = ofstream(filename, mode);
    out << contents;
}

void LogWriter::close() {
    out.flush();
    out.close();
}

LogFile LogWriter::getLogFile() const {
    return logFile;
}

void LogWriter::setContent(string content) {
    logFile.setContent(content);
    out << content;
    out.flush();
}

string LogWriter::appendToFile(string line, bool newLine) {
    string returnedLine = logFile.appendToFile(line, newLine);
    out << returnedLine;
    out.flush();
    return returnedLine;
}

void LogWriter::flush() {
    out.flush();
}