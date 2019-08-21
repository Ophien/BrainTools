#include "BrainTools.h"

using namespace braintools;

BrainTools::BrainTools()
{
}

BrainTools::~BrainTools()
{    
}

void BrainTools::detectRmsAnomaly(std::vector<double> &rInRmsBuffer, std::vector<std::pair<int, int>> &rOutDetectedIndex, double &rInThreshold, int &rInRmsRefractionCount)
{
    #if DEBUG_MODE==true
        logger("Start setting up parameters");
    #endif

    bool detection_enabled = true;
    bool detected = false;
    int rms_samples_since_detection = 0;
    int head_detected_sample = 0;

    #if DEBUG_MODE==true
        logger("End setting up parameters");
        logger("Start detecting ripples");
    #endif

    for (unsigned int rms_sample = 0; rms_sample < rInRmsBuffer.size(); rms_sample++)
    {
        double sample = rInRmsBuffer[rms_sample];

        if (detection_enabled && sample > rInThreshold)
        {
            #if DEBUG_MODE==true
                logger("\tStart ripple event");
            #endif

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

            #if DEBUG_MODE==true
                logger("\tEnd ripple event");
                logger("\tStart inserting event into output vector");
            #endif

            // insert detected interval into the output vector
            rOutDetectedIndex.push_back(std::pair<int,int>(head_detected_sample, rms_sample));

            #if DEBUG_MODE==true
                logger("\tEnd inserting event into output vector");
            #endif
        }
    }

    #if DEBUG_MODE==true
        logger("End detecting ripples");
    #endif
}

double BrainTools::calculateRms(const float *rInBuffer, int &rInBufferSize)
{
    double sum = 0.0;

    #if DEBUG_MODE==true
        logger("Start calculating RMS");
    #endif

    for (int cnt = 0; cnt < rInBufferSize; cnt++)
    {
        sum += pow(rInBuffer[cnt], 2.0);
    }

    #if DEBUG_MODE==true
        logger("End calculating RMS");
        logger("Start calculating square root");
    #endif

    double rms = sqrt(sum / (double)rInBufferSize);

    #if DEBUG_MODE==true
        logger("End calculating square root");
    #endif

    return rms;
}

void BrainTools::checkStatus()
{
    printf("Status OK\n");
}

void BrainTools::logger(const char* rInMsg)
{
    printf("%s\n", rInMsg);
}