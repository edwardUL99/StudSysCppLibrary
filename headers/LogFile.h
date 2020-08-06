#ifndef LOGFILE_H
#define LOGFILE_H

#include <string>

namespace logging {
    class LogFile {
    private:
        std::string fileName;
        std::string content;
        
    public:
        /**
         * Constructs a LogFile object
         * @param fileName the name of the file
         * @param content the content to write if already present
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
         */
        void appendToFile(std::string line, bool newLine = true);
    };
}

#endif /* LOGFILE_H */

