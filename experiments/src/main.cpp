#include <iostream>
#include <cstdlib>
#include <random>

#include <sys/times.h>

#include <baselineOperator.hpp>
#include <sliceOperator.hpp>

int main() {
    {
        int wmNr   = 20000000; // number of watermarks to emit
        int ePerWm =       10; // events between each pair of consecutive watermarks
        int wLen   =     4200; // window length
        int sLen   =      300; // slice length

        tms start;
        tms end;

        std::default_random_engine generator(117);
        std::default_random_engine generatorBaseline(117);
        std::default_random_engine generatorPrototype(117);

        std::uniform_real_distribution<double> randomValue(0.0, 200.0);
        std::uniform_int_distribution<int> randomOutOfOrder(-10, 20);
        std::uniform_int_distribution<int> randomNrOfEventsTilWatermark(0, ePerWm);

        std::cout << "Experimental Environment Initialization" << std::endl;

        int iWm = randomNrOfEventsTilWatermark(generatorBaseline);
        int numberOfTuples = 0;
        int baselineTicks;
        int prototypeTicks;

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
        times(&start);
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
        times(&end);

        baselineTicks = end.tms_utime - start.tms_utime;

        std::cout << "Cleaning memory" << std::endl;
        delete baseline;


        std::cout << "Initializing Prototype Experiment" << std::endl;
        // prototype experiment
        iWm = randomNrOfEventsTilWatermark(generatorPrototype);
        ts = 10;

        SliceOperator *prototype = new SliceOperator(0, wLen, sLen);
        
        std::cout << "Started Prototype Experiment" << std::endl;
        times(&start);
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
        times(&end);

        prototypeTicks = end.tms_utime - start.tms_utime;

        std::cout << "Cleaning memory" << std::endl;
        delete prototype;

        double bP = ((double) prototypeTicks) / baselineTicks;
        double pP = ((double) baselineTicks) / prototypeTicks;

        std::cout << "Summary" << std::endl;
        std::cout << "  Number of events processed: " << numberOfTuples << std::endl;
        std::cout << "  Baseline CPU ticks: " << baselineTicks << std::endl;
        std::cout << "  Prototype CPU ticks: " << prototypeTicks << std::endl;
        if (bP > pP)
            std::cout << "  Baseline is " << bP << " faster than prototype" << std::endl;
        else
            std::cout << "  Prototype is " << pP << " faster than baseline" << std::endl;
    }
    
    return 0;
}