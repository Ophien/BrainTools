#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <stdio.h>
#include <mutex>

class Logger
{
    private:
        std::mutex _outputFileMutex;
        std::string _outputFilePath;
        FILE *_pOutputFile;
        bool _debug;

    public:
        Logger(const char* pInOutputFilePath);
        ~Logger();
        void logger(const char* pInMsg, const char* pInFunctionName = nullptr);  
        void enableDebugMode();
        void disableDebugMode();
};

#endif