#ifndef LOG_WRITER_H
#define LOG_WRITER_H

#include <fstream>
#include "LogFile.h"

namespace logging {
    class LogWriter {
        private:
            LogFile logFile;
            std::ofstream out;
            void init(std::string filename, bool append);
            void init(std::string filename, std::string contents, bool append);
        
        public:
            /**
             * @brief Constructs a default LogWriter
             * NOT RECOMMENDED since this class does not allow you to change the file name, so you will not be able to change the file contained within it
             * This should only be used for default-initialisation, e.g. in maps
             * This does not create any physical file on the hard disk
             */
            LogWriter() = default;
            /**
             * @brief Constructs a LogWriter to write to a log file with the specified file name, with empty contents
             * @param filename The name of the logfile
             * @param append Append to the file identified by this file name if it exists, default is true. Overrides an existing file if true
             */
            LogWriter(std::string filename, bool append = true);
            /**
             * @brief Constructs a LogWriter to write to a log file with the specified file name and contents
             * If the log file already exists and append is true, it will not replace the content of that file, just append these contents onto that file
             * This means there may be disparities between the encapsulated LogFile and the actual physical log file on disk
             * It is not this class' responsibility to read in the existing content and add it to the encapsulated LogFile
             * @param filename The name of the log file
             * @param contents The content to add to the log file
             * @param append Append to the file identifided by this file name if it exists, default is true. Override an existing file if true
             */
            LogWriter(std::string filename, std::string contents, bool append = true);
            /**
             * @brief Closes this LogWriter by closing the underlying streams
             */
            void close();
            /**
             * @brief Retrieves the LogFile this LogWriter is encapsulating
             * This retrieves a COPY of the LogFile, not the actual LogFile instance
             */
            LogFile getLogFile() const;
            /**
             * @brief Sets the contents of the LogFile with the specified content
             * This flushes the stream to the physical file after each call in case of program crash, so the log file is always written to
             * @param content The content to set the LogFile to contain, if this log writer was opened with append as true and the file already exists, it will just add this content to the file after the file's existing contents
             * This means there may be disparities between the encapsulated LogFile and the actual physical log file on disk
             * It is not this class' responsibility to read in the existing content and add it to the encapsulated LogFile
             */
            void setContent(std::string content);
            /**
             * @brief Appends the specified line to the LogFile and with a new line if specified (default)
             * This flushes the stream to the physical file after each call in case of program crash, so the log file is always written to
             * @param line the line to add to the file
             * @param newLine flag to add new line character to the end of the line, default is true
             * @return the line appended to the file (including the newLine if specified)
             */
            std::string appendToFile(std::string line, bool newLine = true);
            /**
             * @brief Flushes the enclosed stream
             * Not required but added to ensure backwards compatibility after refactoring
             */
            void flush();
            /**
             * @brief Provides a convenient way to add a line to the LogFile
             * This is equivalent to calling writer.appendToFile(line, true)
             * @param writer The writer to add the line to
             * @param line the line to add to the writer
             * @return the LogWriter passed in to allow chaining
             */
            friend LogWriter &operator<<(LogWriter &writer, const std::string &line) {
                writer.appendToFile(line);
                return writer;
            }
    };
}

#endif