#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>
#include <sys/time.h>

#include <baselineOperator.hpp>
#include <sliceOperator.hpp>
#include <csv.hpp>

double get_wall_time() {
    struct timeval t;
    if (gettimeofday(&t, NULL)){
        //  Handle error
        return 0;
    }
    return (double)t.tv_sec + (double)t.tv_usec * .000001;
}

std::string get_now() {
    time_t rawtime;
    struct tm *gmt;
    time(&rawtime);
    gmt = gmtime(&rawtime);

    std::string today("GMT-");
    today += std::to_string(gmt->tm_year + 1900);
    if (gmt->tm_mon + 1 < 10)
        today += "0";
    today += std::to_string(gmt->tm_mon + 1);
    if (gmt->tm_mday < 10)
        today += "0";
    today += std::to_string(gmt->tm_mday);
    if (gmt->tm_hour < 10)
        today += "-0";
    else
        today += "-";
    today += std::to_string(gmt->tm_hour);
    if (gmt->tm_min < 10)
        today += "0";
    today += std::to_string(gmt->tm_min);
    if (gmt->tm_sec < 10)
        today += "0";
    today += std::to_string(gmt->tm_sec);

    return today;
}

int main() {
    {
        int wmNr   = 100000; // number of watermarks to emit
        int ePerWm =      50; // events between each pair of consecutive watermarks
        int wLen   =   72000; // window length
        int sLen   =     300; // slice length

        std::string today = get_now();

        Csv bl("out", today + "-baseline");

        bl << "name" << "value" << std::endl;
        bl << "watermarks count" << wmNr << std::endl;
        bl << "window length" << wLen << std::endl;
        bl << "slice length" << sLen << std::endl;

        Csv pt("out", today + "-prototype");

        pt << "name" << "value" << std::endl;
        pt << "watermarks count" << wmNr << std::endl;
        pt << "window length" << wLen << std::endl;
        pt << "slice length" << sLen << std::endl;

        double start;

        std::default_random_engine generator(117);
        std::default_random_engine generatorBaseline(117);
        std::default_random_engine generatorPrototype(117);

        std::uniform_real_distribution<double> randomValue(0.0, 200.0);
        std::uniform_int_distribution<int> randomOutOfOrder(-10, 20);
        std::uniform_int_distribution<int> randomNrOfEventsTilWatermark(0, ePerWm);

        std::cout << "Experimental Environment Initialization" << std::endl;

        int iWm = randomNrOfEventsTilWatermark(generatorBaseline);
        int numberOfTuples = 0;
        double baselineTime;
        double prototypeTime;

        for (int wm = 0; wm < wmNr; wm++) {
            for (int e = 0; e < iWm; e++) {
                numberOfTuples++;
            }
            
            iWm = randomNrOfEventsTilWatermark(generatorBaseline);
        }

        std::cout << "Initializing Baseline Experiment" << std::endl;
        // baseline experiment
        iWm = randomNrOfEventsTilWatermark(generatorBaseline);
        int ts = 10;

        BaselineOperator *baseline = new BaselineOperator(0, wLen);

        std::cout << "Started Baseline Experiment" << std::endl;
        start = get_wall_time();
        for (int wm = 0; wm < wmNr; wm++) {
            for (int e = 0; e < iWm; e++) {
                baseline->processElement(
                    new Event(
                        randomValue(generatorBaseline),
                        ts + randomOutOfOrder(generatorBaseline)
                ));
                ts += 10;
            }

            std::vector<Element *> out = baseline->processWatermark(
                new Watermark(ts + randomOutOfOrder(generatorBaseline))
            );

            // discard output stream for now
            for (Element *o : out)
                delete o;

            out.clear();
            out.resize(0);
            
            iWm = randomNrOfEventsTilWatermark(generatorBaseline);
        }

        baselineTime = get_wall_time() - start;

        std::cout << "Cleaning memory" << std::endl;
        delete baseline;


        std::cout << "Initializing Prototype Experiment" << std::endl;
        // prototype experiment
        iWm = randomNrOfEventsTilWatermark(generatorPrototype);
        ts = 10;

        SliceOperator *prototype = new SliceOperator(0, wLen, sLen);
        
        std::cout << "Started Prototype Experiment" << std::endl;
        start = get_wall_time();
        for (int wm = 0; wm < wmNr; wm++) {
            for (int e = 0; e < iWm; e++) {
                prototype->processElement(
                    new Event(
                        randomValue(generatorPrototype),
                        ts + randomOutOfOrder(generatorPrototype)
                ));
                ts += 10;
            }

            std::vector<Element *> out = prototype->processWatermark(
                new Watermark(ts + randomOutOfOrder(generatorPrototype))
            );

            // discard output stream for now
            for (Element *o : out)
                delete o;

            out.clear();
            out.resize(0);
            iWm = randomNrOfEventsTilWatermark(generatorPrototype);
        }

        prototypeTime = get_wall_time() - start;

        std::cout << "Cleaning memory" << std::endl;
        delete prototype;

        double bP = prototypeTime / baselineTime;
        double pP = baselineTime / prototypeTime;

        std::cout.precision(3);
        std::cout << std::fixed << "Summary" << std::endl;
        std::cout << "  Number of events processed: " << numberOfTuples << std::endl;
        std::cout << "  Baseline time in sec: " << baselineTime << std::endl;
        std::cout << "  Prototype time in sec: " << prototypeTime << std::endl;
        if (bP > pP)
            std::cout << "  Baseline is " << bP << " faster than prototype" << std::endl;
        else
            std::cout << "  Prototype is " << pP << " faster than baseline" << std::endl;

        bl << "events count" << numberOfTuples << std::endl;
        bl << "run time in sec" << baselineTime << std::endl;
        bl << "run time baseline / prototype" << bP << std::endl;

        pt << "events count" << numberOfTuples << std::endl;
        pt << "run time in sec" << prototypeTime << std::endl;
        pt << "run time prototype / baseline" << pP << std::endl;
    }
    
    exit(EXIT_SUCCESS);
}