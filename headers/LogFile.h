#ifndef LOGFILE_H
#define LOGFILE_H

#include <string>

namespace logging {
    /**
      * This class represents a log file that is being written to
      */
    class LogFile {
    private:
        std::string fileName;
        std::string content;

    public:
        /**
         * Constructs a LogFile object
         * @param fileName the name of the file
         * @param content the content to write if already present
         * @param append whether you should append to the logfile on the hard drive
         */
        LogFile(std::string fileName = "", std::string content = "");
        /**
         * Gets the name for this LogFile. The name is the same as the file saved to the hard drive
         * @return the name of this LogFile
         */
        std::string getFileName() const;
        /**
         * The file name to name this LogFile. Note that this is also the name of the file when it is written to the file
         * @param fileName the name of the file
         */
        void setFileName(std::string fileName);
        /**
         * Gets the content that is inside this LogFile
         * @return LogFile content
         */
        std::string getContent() const;
        /**
         * Sets the contents of this LogFile
         * @param content the content to add
         */
        void setContent(std::string content);
        /**
         * Appends the specified line to this LogFile
         * @param line the line to append
         * @param newLine true if you want to attach a new line to the end(default)
         * @return the line with any newLine characters on it if specified
         */
        std::string appendToFile(std::string line, bool newLine = true);
        /**
         * Checks the equality of this log file with the provided one by checking the equality of their file names only, not the contents
         * @param logFile the logfile to compare
         * @return true if equal
         */
        bool operator==(const LogFile &logFile) const;
        /**
         * Checks if this log file is lexicographically less than the provided log file by comparing their file names
         * @param logFile the file to compare
         * @return true if this log file is lexicographically less than the supplied log file
         */
        bool operator<(const LogFile &logFile) const;
    };
}

#endif /* LOGFILE_H */

