#include "BrainTools.h"

using namespace braintools;

BrainTools::BrainTools(const char* pInLoggerOutputFile, const char* pInProfilingOutputFile)
{
    // initialize logger
    _logger = new Logger(pInLoggerOutputFile);

    // initialize profiler
    _profiler = new Profiler(pInProfilingOutputFile);
}

BrainTools::~BrainTools()
{
}

void BrainTools::detectRmsAnomaly(std::vector<double> &rInRmsBuffer,
                                  std::vector<std::pair<int, int>> &rOutDetectedIndex,
                                  double &rInThreshold,
                                  int &rInRmsRefractionCount)
{
    BT_PROFILING_START("detectRmsAnomaly");
    BT_LOGGER("detectRmsAnomaly", "Start setting up parameters");

    bool detection_enabled = true;
    bool detected = false;
    int rms_samples_since_detection = 0;
    int head_detected_sample = 0;

    BT_LOGGER("detectRmsAnomaly", "End setting up parameters");
    BT_LOGGER("detectRmsAnomaly", "Start detecting ripples");

    for (unsigned int rms_sample = 0; rms_sample < rInRmsBuffer.size(); rms_sample++)
    {
        double sample = rInRmsBuffer[rms_sample];

        if (detection_enabled && sample > rInThreshold)
        {
            BT_LOGGER("detectRmsAnomaly", "Start ripple event");

            head_detected_sample = rms_sample;
            detected = true;
            detection_enabled = false;
        }

        // count rms samples since last detection
        if (detected)
        {
            rms_samples_since_detection += 1;
        }

        // enable detection again
        if (rms_samples_since_detection > rInRmsRefractionCount)
        {
            // reset
            detected = false;
            detection_enabled = true;
            rms_samples_since_detection = 0;

            BT_LOGGER("detectRmsAnomaly", "End ripple event");
            BT_LOGGER("detectRmsAnomaly", "Start inserting event into output vector");

            // insert detected interval into the output vector
            rOutDetectedIndex.push_back(std::pair<int, int>(head_detected_sample, rms_sample));

            BT_LOGGER("detectRmsAnomaly", "End inserting event into output vector");
        }
    }

    BT_LOGGER("detectRmsAnomaly", "End detecting ripples");
    BT_PROFILING_END("detectRmsAnomaly")
}

double BrainTools::calculateRms(const float *rInBuffer, int &rInBufferSize)
{
    BT_PROFILING_START("calculateRms");

    double sum = 0.0;

    BT_LOGGER("calculateRms", "Start calculating RMS");

    for (int cnt = 0; cnt < rInBufferSize; cnt++)
    {
        sum += pow(rInBuffer[cnt], 2.0);
    }

    BT_LOGGER("calculateRms", "End calculating RMS");
    BT_LOGGER("calculateRms", "Start calculating square root");

    double rms = sqrt(sum / (double)rInBufferSize);

    BT_LOGGER("calculateRms", "End calculating square root");
    BT_PROFILING_END("calculateRms")

    return rms;
}

void BrainTools::checkStatus()
{
    BT_LOGGER("checkStatus", "Start checking status");
    BT_PROFILING_START("checkStatus");
    printf("Status OK\n");
    BT_PROFILING_END("checkStatus");
    BT_LOGGER("checkStatus", "End checking status");
}

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