#include "headers/ConfigFileProcessor.h"
#include <cctype>
#include <iostream>

using std::string;
using std::map;
using std::ifstream;

ConfigFileProcessor::ConfigFileProcessor(std::string file, char separator) {
    configFile.open(file);
    this->separator = separator;
    read();
}

bool ConfigFileProcessor::available() {
    return configFile.is_open();
}

string ConfigFileProcessor::getValue(string property) {
    return properties[property];
}

string ConfigFileProcessor::trimWhiteSpace(const string &s) {
    string trimmed;

    for (int i = 0; i < s.size(); i++) {
        char ch = s[i];
        if (!isspace(ch)) {
            trimmed += ch;
        }
    }

    return trimmed;
}

void ConfigFileProcessor::processLine(string line) {
    if (line != "" && line[0] != '#') {
        //the line isn't a comment or a blank line

        std::size_t eqPos = line.find(separator);
        string property = trimWhiteSpace(line.substr(0, eqPos));

        string value = trimWhiteSpace(line.substr(eqPos + 1));

        properties[property] = value;
    }
}

void ConfigFileProcessor::read() {
    if (available()) {
        string line = "";

        while (getline(configFile, line)) {
            processLine(line);
        }
    }
}