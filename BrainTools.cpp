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
                                  int &rInRmsRefractionCount,
                                  void (*rInDetectedIntervalStartCallBack)(void *params),
                                  void (*rInDetectedIntervalEndCallBack)(void *params),
                                  void *rInDetectedIntervalStartCallBackParams,
                                  void *rInDetectedIntervalEndCallBackParams)
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

            // call the detected interval start call back to trigger any special events
            // when a detection occurs
            if (rInDetectedIntervalStartCallBack != nullptr)
            {
                if (rInDetectedIntervalStartCallBackParams != nullptr)
                    rInDetectedIntervalStartCallBack(rInDetectedIntervalStartCallBackParams);
                else
                    rInDetectedIntervalStartCallBack(nullptr);
            }

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
            // call the detected interval end call back to trigger any special events
            // when a detection occurs
            if (rInDetectedIntervalEndCallBack != nullptr)
            {
                if (rInDetectedIntervalEndCallBackParams != nullptr)
                    rInDetectedIntervalEndCallBack(rInDetectedIntervalEndCallBackParams);
                else
                    rInDetectedIntervalEndCallBack(nullptr);
            }

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
    BT_PROFILING_START("checkStatus")
    printf("Status OK\n");
    BT_PROFILING_END("checkStatus")
}