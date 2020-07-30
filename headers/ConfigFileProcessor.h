#ifndef CONFIG_FILE_PROCESSOR_H
#define CONFIG_FILE_PROCESSOR_H

#include <map>
#include <string>
#include <fstream>

/**
 * This class takes in a Config File that is structured in the means of attribute on left side of '=' is the property and right side is the value
 * This makes it more flexible as it can read in any number of config params
 * Comments start with # in the config file
 */
class ConfigFileProcessor {
    private:
        std::map<std::string, std::string> properties;
        std::ifstream configFile;
        std::string trimWhiteSpace(const std::string &s);
        char separator; //the separator between property and value, default is = but you can set it to be :
        void processLine(std::string line);
        void read();

    public:
        ConfigFileProcessor(std::string file, char separator = '=');
        bool available(); //if it's not available, the file couldn't be opened
        std::string getValue(std::string property);
};

#endif