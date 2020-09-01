#ifndef LOGGING_H
#define LOGGING_H

#include "LogFile.h"
#include "Logger.h"
#include "LogWriter.h"

namespace logging {
    //this is a simple header file to set up a logger that can be used globally and to include all required logging components
    extern Logger logger;
}

#endif