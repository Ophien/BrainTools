#ifndef BRAIN_TOOLS_H
#define BRAIN_TOOLS_H

#include <utility> // used for pair and other structures
#include <vector>
#include <stdio.h>
#include <math.h>

#define DEBUG_MODE false

namespace braintools
{

class BrainTools
{
    public:
        BrainTools();
        ~BrainTools();
        void detectRmsAnomaly(std::vector<double> &rInRmsBuffer, std::vector<std::pair<int, int>> &rOutDetectedIndex, double &rInThreshold, int &rInRmsRefractionCount);
        double calculateRms(const float *rInBuffer, int &rInBufferSize);
        void logger(const char* rInMsg);
        void checkStatus();
};

}

#endif