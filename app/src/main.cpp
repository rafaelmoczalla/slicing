#include <iostream>
#include <cstdlib>
#include <random>

#include <sys/times.h>

#include <baselineOperator.hpp>
#include <sliceOperator.hpp>

int main() {
    {
        int wmNr = 1000; // number of watermarks to emit
        int wLen = 21600000; // window length
        int sLen = 300000; // slice length

        tms start;
        tms end;

        std::default_random_engine generatorBaseline(117);
        std::default_random_engine generatorPrototype(117);

        std::uniform_int_distribution<int> randomOutOfOrder(-40000, 160000);
        std::uniform_int_distribution<int> randomNrOfEventsTilWatermark(0, 100);

        std::cout << "Initializing Baseline Experiment" << std::endl;
        // baseline experiment
        int iWm = randomNrOfEventsTilWatermark(generatorBaseline);
        int ts = 40000;

        std::vector<Element *> stream;
        for (int wm = 0; wm < wmNr; wm++) {
            for (int e = 0; e < iWm; e++) {
                stream.push_back(new Event(0, ts + randomOutOfOrder(generatorBaseline)));
                ts += 10000;
            }

            stream.push_back(new Watermark(ts));
            iWm += randomNrOfEventsTilWatermark(generatorBaseline);
        }

        BaselineOperator *baseline = new BaselineOperator(0, wLen);

        std::cout << "Started Baseline Experiment" << std::endl;
        times(&start);
        for (Element *e : stream) {
            if (e->getType() == 0) {
                baseline->processWatermark(static_cast<Watermark *>(e));
            } else if (e->getType() == 1) {
                baseline->processElement(static_cast<Event *>(e));
            }
        }
        times(&end);

        std::cout << "time in CPU ticks: " << end.tms_utime - start.tms_utime << std::endl;

        stream.clear();
        stream.resize(0);

        std::cout << "Initializing Prototype Experiment" << std::endl;
        // prototype experiment
        iWm = randomNrOfEventsTilWatermark(generatorPrototype);
        ts = 40000;
        
        for (int wm = 0; wm < wmNr; wm++) {
            for (int e = 0; e < iWm; e++) {
                stream.push_back(new Event(0, ts + randomOutOfOrder(generatorPrototype)));
                ts += 10000;
            }

            stream.push_back(new Watermark(ts));
            iWm += randomNrOfEventsTilWatermark(generatorPrototype);
        }

        SliceOperator *prototype = new SliceOperator(0, wLen, sLen);

        std::cout << "Started Prototype Experiment" << std::endl;
        times(&start);
        for (Element *e : stream) {
            if (e->getType() == 0) {
                prototype->processWatermark(static_cast<Watermark *>(e));
            } else if (e->getType() == 1) {
                prototype->processElement(static_cast<Event *>(e));
            }
        }
        times(&end);

        std::cout << "time in CPU ticks: " << end.tms_utime - start.tms_utime << std::endl;
    }
    
    return 0;
}