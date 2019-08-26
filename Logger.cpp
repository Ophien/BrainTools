#include "Logger.h"

Logger::Logger(const char *pInOutputFilePath)
{
    _outputFilePath = pInOutputFilePath;
}

Logger::~Logger()
{
    if (_pOutputFile)
        fclose(_pOutputFile);
}

void Logger::enableDebugMode()
{
    _debug = true;
}

void Logger::disableDebugMode()
{
    _debug = false;
}

void Logger::logger(const char *pInMsg, const char *pInFunctionName)
{
    // thread safe logger
    _outputFileMutex.lock();

    // open output file with write permission
    _pOutputFile = fopen(_outputFilePath.c_str(), "aw");

    // print out the desired log
    if (pInFunctionName != nullptr)
    {
        if(_debug)
            printf("%s - %s\n", pInFunctionName, pInMsg);
        
        fprintf(_pOutputFile, "%s - %s\n", pInFunctionName, pInMsg);
    }
    else
    {
        if(_debug)
            printf("Unknown - %s\n", pInMsg);
        
        fprintf(_pOutputFile, "Unknown - %s\n", pInMsg);
    }

    // close output logger file
    fclose(_pOutputFile);

    // release this thread save call
    _outputFileMutex.unlock();
}