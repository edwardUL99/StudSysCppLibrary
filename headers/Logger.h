#ifndef LOGGER_H
#define LOGGER_H

#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <boost/optional.hpp>

namespace logging
{
    class LogFile;
    class LogWriter;

    /**
     * An enum class to represent the log type to write
     */
    enum class LogTypes {
        WARNING,
        ERROR,
        INFO
    };

    /**
     * This class is responsible for managing logging. It handles writing of logs and also adding to them
     */
    class Logger {
        private:
            std::map<std::string, LogWriter> logFiles; //the file name is mapped to its equivalent log file
            void convertVectorsToMap(const std::vector<std::string> &logFileNames, const std::vector<std::string> &logContents); //takes the t2 vectors, constructs LogFile objects out of them and then calls initialise maps
            void initialiseMaps(const std::map<std::string, LogFile> &logFiles); //initialises the maps for this logger

        public:
            /**
             * Takes a map of logFiles that already exist and constructs a Logger object out of them
             * @param logFiles an existing map of log file names to their LogFile equivalents
             */
            Logger(std::map<std::string, LogFile> logFiles);
            /**
             * Default constructs this logger
             */
            Logger();
            /**
             * Takes a vector of log file names, constructs an empty logfile each and then constructs the map and object from them
             * @param logFileNames the names of the log file names
             * @param logFileContent the vector of contents to add to the equivalent log file produced. i.e. logFileNames[i]'s content should be at logContents[i]
             *      It is undefined behaviour if logContents size is not 0 but not equal to logFileNames' size
             */
            Logger(std::vector<std::string> logFileNames, std::vector<std::string> logContents = std::vector<std::string>());
            /**
             * Checks if this logger contains the LogFile identified by the name
             * @param fileName the name of the log file
             * @return true if the logger contains it
             */ 
            bool containsLogFile(const std::string &fileName) const;
            /**
             * Adds the specified log file to this LOgger object
             * @param logFile the log file to add
             */
            void addLogFile(LogFile logFile);
            /**
             * Attempts to remove the LogFile specified by the file name from this Logger
             * @param fileName the name of the log file
             */
            void removeLogFile(const std::string &fileName);
            /**
             * Attempts to find the log file specified by the file name and returns it, if not found returns an empty optional object
             * @param fileName the name of the LogFile
             * @return an optional object containing the LogFile if it exists, or empty otherwise
             */
            boost::optional<LogFile> findLogFile(const std::string &fileName) const;
            /**
             * Sets the contents of the log file specified by the file name with the contents provided and writes it to the file
             * @param fileName the name of the log file
             * @param contents the content to set the LogFile to
             * @return true if the operation was successful
             */
            bool setLogFileContents(const std::string &fileName, const std::string &contents);
            /**
             * Appends the line to the Log File specified by the file name and writes it to the file
             * @param fileName the name of the LogFile
             * @param line the line to append
             * @param newLine true if you wish to add a new line, default is true
             * @return true if the operation was successful
             */
            bool appendToLogFile(const std::string &fileName, const std::string &line, bool newLine = true);
            /**
             * A convenience method to append the log type for you in the format [LOG_TYPE]: message
             * @param fileName the name of the file to write to
             * @param type the enumerated value depicting the type of this Log Message
             * @param message the message to enter after the log type
             * @param newLine true if you wish to add a new line
             * @return true if the operation was successful
             */
            bool appendToLogFile(const std::string &fileName, LogTypes type, const std::string &message, bool newLine = true);
            /**
             * Flushes the file specified by the filename if it exists. It does nothing otherwise
             * @param fileName the name of the log file
             */
            void flushFile(const std::string &fileName);
            /**
             * Flushes all the log files in this logger
             */
            void flushAll();
    };
}

#endif