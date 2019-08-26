#include "Profiler.h"

Profiler::Profiler(const char *pInOutputFilePath)
{
    _outputFilePath = pInOutputFilePath;
}

Profiler::~Profiler()
{
}

void Profiler::start(const char *pInFunctionName)
{
    // thread safe map, one per thread id
    std::thread::id thread_id = std::this_thread::get_id();
    if (_profilingStartMarksBuffer.find(thread_id) == _profilingStartMarksBuffer.end())
    {
        // insert map
        std::map<const char*, double>* statistic_map = new std::map<const char*, double>();
        std::pair<std::thread::id, std::map<const char*, double>*> inser_pair;
        inser_pair.first = thread_id;
        inser_pair.second = statistic_map;

        _profilingStartMarksBuffer.insert(inser_pair);
    }

    if(_profilingStartMarksBuffer[thread_id]->find(pInFunctionName) == _profilingStartMarksBuffer[thread_id]->end())
       _profilingStartMarksBuffer[thread_id]->insert(std::pair<const char*, double>(pInFunctionName, 0.0));

    // set value
    auto current_time = std::chrono::high_resolution_clock::now();
    auto duration_in_seconds = std::chrono::duration<double>(current_time.time_since_epoch());
    double seconds = duration_in_seconds.count();

    (*_profilingStartMarksBuffer[thread_id])[pInFunctionName] = seconds;
}

void Profiler::end(const char *pInFunctionName)
{
    // check if start was set
    std::thread::id thread_id = std::this_thread::get_id();
    if (_profilingStartMarksBuffer.find(thread_id) == _profilingStartMarksBuffer.end())
        return;
    else
        if(_profilingStartMarksBuffer[thread_id]->find(pInFunctionName) == _profilingStartMarksBuffer[thread_id]->end())
            return;
            
    // get current time in seconds
    auto current_time = std::chrono::high_resolution_clock::now();
    auto duration_in_seconds = std::chrono::duration<double>(current_time.time_since_epoch());
    double seconds = duration_in_seconds.count();

    // calculate deltatime
    double delta_time = seconds - (*_profilingStartMarksBuffer[thread_id])[pInFunctionName];

    // set delta time in the main buffer
    if (_deltaTimesMap.find(pInFunctionName) == _deltaTimesMap.end())
        _deltaTimesMap.insert(std::pair<const char *, std::vector<double> *>(pInFunctionName, new std::vector<double>()));

    // thread safe insertion into the main statistic profiling restults
    _deltaTimeMapMutex.lock();
    _deltaTimesMap[pInFunctionName]->push_back(delta_time);
    _deltaTimeMapMutex.unlock();
}

void Profiler::calculateStatistics(std::map<const char *, StatisticsObject *> &rOutStatisticsMap)
{
    // TODO::calculate statistics here
}

void Profiler::finish()
{
    FILE *pOutputFile = fopen(_outputFilePath.c_str(), "w");

    // calculate all statistics
    std::map<const char *, StatisticsObject *> statistics_objects_per_function;
    calculateStatistics(statistics_objects_per_function);

    // TODO implement statistics writing method

    fclose(pOutputFile);
}