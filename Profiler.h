#ifndef PROFILER_H
#define PROFILER_H

#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <mutex>

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

#endif