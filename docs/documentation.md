# Brain Tools documentation

# detectRmsAnomaly

### Declaration 

    void detectRmsAnomaly(std::vector<double> &rInRmsBuffer,
                          std::vector<std::pair<int, int>> &rOutDetectedIndex,
                          double &rInThreshold,
                          int &rInRmsRefractionCount);

### Parameters

### Return Values

### Exceptions
                    
### Usage

    ```
    #include <libbraintools/BrainTools.h> // includes the brain tools library

    int main(int argc, int char* argv[])
    {
        // brain tools object
        braintools::BrainTools* pBrainToolsObject;

        // create object
        pBrainToolsObject = new braintools:BrainTools("log.out", "profiling.out");

        // create rms buffer to be checked
        std::vector<double> raw_samples;

        // number of samples to be generated
        int number_of_samples = 1000;

        // populate raw samples to simulate a observed signal
        pRawSamples = new double[number_of_samples];
        for(int sample = 0; sample < number_of_samples; sample++)
            raw_samples.push_back(sample);

        // generate RMS buffer from the raw_sample buffer
        std::vector<double> rms_buffer;
        pBrainToolsObject->generateRmsBuffer(raw_samples, rms_buffer);

        // detection parameters
        double detection_threshold = 0.2;
        int refractory_rms_count = 1;

        // output sample - value pairs of all detected anomalies
        std::vector<std::pair<int, int>> detected_rmssample_value_pairs;
        
        // call for the RMS based anomaly detection algorithm
        // the detected anomalies will be stored inside the detected_rmssample_value_pairs, 
        // where each pair is the rms start index and rms end index
        pBrainToolsObject->detectRmsAnomaly(rms_buffer, detected_rmssample_value_pairs, detection_threshold, refractory_rms_count);

        // print anomalies
        for(unsigned int rms_sample = 0; rms_sample < detected_rmssample_value_pair.size(); rms_sample++)
            printf"start: %d, end: %d", (detected_rms_sample_value_pair[rms_sample].first, detected_rms_sample_value_pair[rms_sample].seconds);

        return 0;
    }
    ```