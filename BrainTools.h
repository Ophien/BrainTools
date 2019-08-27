#ifndef BRAIN_TOOLS_H
#define BRAIN_TOOLS_H

#include <utility>  // used for pair and other structures
#include <vector>   // used as a data structure to some f the input buffers
#include <stdio.h>  // used for printf, FILE, and related functions
#include <math.h>   // used for math functions such as cos, sin.
#include <chrono>   // used to measure execution times inside all library's functions
#include <thread>   // used for thread operations
#include <mutex>    // user for mutex object for thread safe logger and profoling
#include <string>   // used for basic string operations
#include <map>      // used as storage in many components

// directive used to enable and disable the profiling mode of all declarations in this file.
// if it is enabled, then all functions will log their respective execution times.
#define BT_PROFILING_MODE false

// debug logger macro
#define BT_LOGGER(function_name, msg) _logger->logger(function_name, msg)

// profilling start macro
#define BT_PROFILING_START(function_name) if(BT_PROFILING_MODE == true) _profiler->start(function_name);

// profilling end macro
#define BT_PROFILING_END(function_name) if(BT_PROFILING_MODE == true) _profiler->end(function_name);

namespace braintools
{

class StatisticsObject
{
    public:
        std::vector<double> _observations;

        double _variance;
        double _deviation;
        double _max;
        double _min;
        double _average;
        double _median;
};

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

class Profiler
{
    private:
        std::string _outputFilePath;
        FILE *_pOutputFile;

        std::mutex _deltaTimeMapMutex;
        std::map<const char *, std::vector<double> *> _deltaTimesMap;
        std::map<std::thread::id, std::map<const char*, double>*> _profilingStartMarksBuffer;

        void calculateStatistics(std::map<const char*, StatisticsObject*>& rOutStatisticsMap);

    public:
        Profiler(const char* pInOutputFilePath);
        ~Profiler();
        void start(const char* pInFunctionName);
        void end(const char* pInFunctionName);
        void finish();
};

/*
 * This class is the core object of the Brain Tools library. It contains all methods and helper functions
 * to allow to process signals considering all sorts of input buffers.
 */
class BrainTools
{
  public:
    BrainTools(const char *pInLoggerOutputFile, const char *pInProfilingOutputFile);
    ~BrainTools();

    /*
    * detectRmsAnomaly - This function detects anomalies in a RMS array computed from a raw signal window.
    *                    It detectes anomalies based on a threshold. Furthermore, it allows to receive callback
    *                    functions to be called when the start or end of an anomaly appears.
    * Params:
    *   rInRmsBuffer -> vector<double> containing the window to be verified.
    *   rOutDetectedIndex -> vector<pair<int,int>> containing the (rms index, and rms value) of the detected anomaly.
    *   rInThreshold -> int containing the threshold to be used to detect anomalies. If the RMS value exceds the threshold
    *                   an anomaly is marked.
    *   rInRmsRefractionCount -> int containing the number of RMS samples to be used a refractory time. After a detection it is
    *                            the number os samples the function will discard before starting a new detection.
    */
    void detectRmsAnomaly(std::vector<double> &rInRmsBuffer,
                          std::vector<std::pair<int, int>> &rOutDetectedIndex,
                          double &rInThreshold,
                          int &rInRmsRefractionCount);
    void generateRmsBuffer(std::vector<double> &rInRmsBuffer, std::vector<double> &rOutRmsBuffer);
    double calculateRms(const float *pInBuffer, int &rInBufferSize);

    /*
     * checkStats - This function allows to check if the library was loaded successfuly by other programs.
     *              If everything is OK and the child program loads this object correctly, then it should 
     *              print the message "OK".
     */
    void checkStatus();

  private:
    // name of this object's logger output file.
    std::string _loggerOutputFile;

    // name of this object's output profiling file.
    std::string _profilingOutputFile;

    // logger object.
    Logger *_logger;

    // profiler object
    Profiler *_profiler;
};

} // namespace braintools

#endif