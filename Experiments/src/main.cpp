#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>
#include <sys/time.h>

#include <bucketSlidingWindowOperator.hpp>
#include <slicedSlidingWindowOperator.hpp>
#include <rollingSlidingWindowOperator.hpp>
#include <csv.hpp>

struct Setup {
    std::string name;
    std::vector<long> wmNr;
    std::vector<long> ePerWm;
    std::vector<long> wLen;
    std::vector<long> sLen;
    std::string to_string() {
        std::stringstream out;

        out << "Setup{" << std::endl << "name=" << name << std::endl;

        out << "wmNr=[";
        for (int i = 0; i < wmNr.size() - 1; i++) {
            out << std::to_string(wmNr[i]) << ", ";
        }
        if (!wmNr.empty())
            out << std::to_string(wmNr.back());
        out << "]," << std::endl;

        out << "ePerWm=[";
        for (int i = 0; i < ePerWm.size() - 1; i++) {
            out << std::to_string(ePerWm[i]) << ", ";
        }
        if (!ePerWm.empty())
            out << std::to_string(ePerWm.back());
        out << "]," << std::endl;

        out << "wLen=[";
        for (int i = 0; i < wLen.size() - 1; i++) {
            out << std::to_string(wLen[i]) << ", ";
        }
        if (!wLen.empty())
            out << std::to_string(wLen.back());
        out << "]," << std::endl;

        out << "sLen=[";
        for (int i = 0; i < sLen.size() - 1; i++) {
            out << std::to_string(sLen[i]) << ", ";
        }
        if (!sLen.empty())
            out << std::to_string(sLen.back());
        out << "]}" << std::endl;

        return out.str();;
    };
};

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

void experiment(Csv &csv, long wmNr, long ePerWm, long wLen, long sLen) {
    double start;

    std::default_random_engine generator(117);
    std::default_random_engine generatorBucket(117);
    std::default_random_engine generatorSlicing(117);
    std::default_random_engine generatorRolling(117);

    std::uniform_real_distribution<double> randomValue(0.0, 200.0);
    std::uniform_int_distribution<long> randomOutOfOrder(-10, 20);
    std::uniform_int_distribution<long> randomNrOfEventsTilWatermark(0, ePerWm);

    std::cout << "Experimental Environment Initialization" << std::endl;

    long iWm = randomNrOfEventsTilWatermark(generator);
    long numberOfEvents = 0;
    long ts;
    long cWm = 0;
    long lWm = 0;
    double bucketTime;
    double slicingTime;
    double rollingTime;

    for (long wm = 0; wm < wmNr; wm++) {
        for (long e = 0; e < iWm; e++)
            numberOfEvents++;

        iWm = randomNrOfEventsTilWatermark(generator);
    }

    csv << std::to_string(numberOfEvents)
    << std::to_string(wmNr)
    << std::to_string(ePerWm)
    << std::to_string(wLen)
    << std::to_string(sLen);

    std::cout << "Initializing Bucket Sliding Window Experiment" << std::endl;
    // bucketOperator experiment
    iWm = randomNrOfEventsTilWatermark(generatorBucket);
    ts = 10;
    cWm = 0;
    lWm = 0;

    BucketSlidingWindowOperator *bucketOperator = new BucketSlidingWindowOperator(0, wLen, sLen);

    std::cout << "Started Bucket Sliding Window Experiment" << std::endl;
    start = get_wall_time();
    for (long wm = 0; wm < wmNr; wm++) {
        for (long e = 0; e < iWm; e++) {
            bucketOperator->processElement(
                new Event(
                    randomValue(generatorBucket),
                    ts + randomOutOfOrder(generatorBucket)
            ));
            ts += 10;
        }

        cWm = ts + randomOutOfOrder(generatorBucket);
        if (cWm <= lWm)
            cWm = lWm + 1;
        std::vector<Element *> out
                = bucketOperator->processWatermark(new Watermark(cWm));
        lWm = cWm;

        // discard output stream for now
        for (Element *o : out)
            delete o;

        out.clear();
        out.resize(0);

        iWm = randomNrOfEventsTilWatermark(generatorBucket);
    }

    bucketTime = get_wall_time() - start;
    csv << std::to_string(bucketTime);

    std::cout << "Cleaning memory" << std::endl;
    delete bucketOperator;


    std::cout << "Initializing Sliced Sliding Window Experiment" << std::endl;
    // slicedOperator experiment
    iWm = randomNrOfEventsTilWatermark(generatorSlicing);
    ts = 10;
    cWm = 0;
    lWm = 0;

    SlicedSlidingWindowOperator *slicedOperator = new SlicedSlidingWindowOperator(0, wLen, sLen);

    std::cout << "Started Sliced Sliding Window Experiment" << std::endl;
    start = get_wall_time();
    for (long wm = 0; wm < wmNr; wm++) {
        for (long e = 0; e < iWm; e++) {
            slicedOperator->processElement(
                new Event(
                    randomValue(generatorSlicing),
                    ts + randomOutOfOrder(generatorSlicing)
            ));
            ts += 10;
        }

        cWm = ts + randomOutOfOrder(generatorSlicing);
        if (cWm <= lWm)
            cWm = lWm + 1;
        std::vector<Element *> out
                = slicedOperator->processWatermark(new Watermark(cWm));
        lWm = cWm;

        // discard output stream for now
        for (Element *o : out)
            delete o;

        out.clear();
        out.resize(0);

        iWm = randomNrOfEventsTilWatermark(generatorSlicing);
    }

    slicingTime = get_wall_time() - start;
    csv << std::to_string(slicingTime);

    std::cout << "Cleaning memory" << std::endl;
    delete slicedOperator;


    std::cout << "Initializing Rolling Sliding Window Experiment" << std::endl;
    // prototype experiment
    iWm = randomNrOfEventsTilWatermark(generatorRolling);
    ts = 10;
    cWm = 0;
    lWm = 0;

    RollingSlidingWindowOperator *prototype = new RollingSlidingWindowOperator(0, wLen, sLen);

    std::cout << "Started Rolling Sliding Window Experiment" << std::endl;
    start = get_wall_time();
    for (long wm = 0; wm < wmNr; wm++) {
        for (long e = 0; e < iWm; e++) {
            prototype->processElement(
                new Event(
                    randomValue(generatorRolling),
                    ts + randomOutOfOrder(generatorRolling)
            ));
            ts += 10;
        }

        cWm = ts + randomOutOfOrder(generatorRolling);
        if (cWm <= lWm)
            cWm = lWm + 1;
        std::vector<Element *> out
                = prototype->processWatermark(new Watermark(cWm));
        lWm = cWm;

        // discard output stream for now
        for (Element *o : out)
            delete o;

        out.clear();
        out.resize(0);
        iWm = randomNrOfEventsTilWatermark(generatorRolling);
    }

    rollingTime = get_wall_time() - start;
    csv << std::to_string(rollingTime);

    std::cout << "Cleaning memory" << std::endl;
    delete prototype;

    double sP = rollingTime / slicingTime;
    double rP = slicingTime / rollingTime;

    std::cout.precision(3);
    std::cout << std::fixed << "Summary" << std::endl;
    std::cout << "  Number of events processed: " << numberOfEvents << std::endl;
    std::cout << "  Bucket Sliding Window time in sec:  " << bucketTime << std::endl;
    std::cout << "  Sliced Sliding Window time in sec:  " << slicingTime << std::endl;
    std::cout << "  Rolling Sliding Window time in sec: " << rollingTime << std::endl;
    if (sP > rP)
        std::cout << "  Sliced Sliding Window is " << sP << " faster than Rolling Sliding Window" << std::endl;
    else
        std::cout << "  Rolling Sliding Window is " << rP << " faster than Sliced Sliding Window" << std::endl;

    csv << std::endl;
}

int main() {
    {
        std::string today = get_now();
        std::string folder = "out/rollingExperiments_" + today;

        std::filesystem::create_directories(folder);

        std::vector<Setup> experiments;

        Setup setup1;
        setup1.name = "VaryingCountOfWatermarks";
        setup1.wmNr.push_back(100000);
        setup1.wmNr.push_back(200000);
        setup1.wmNr.push_back(300000);
        setup1.wmNr.push_back(400000);
        setup1.ePerWm.push_back(50);
        setup1.wLen.push_back(7200);
        setup1.sLen.push_back(30);
        experiments.push_back(setup1);

        Setup setup2;
        setup2.name = "VaryingCountOfEventsPerWatermark";
        setup2.wmNr.push_back(100000);
        setup2.ePerWm.push_back(30);
        setup2.ePerWm.push_back(60);
        setup2.ePerWm.push_back(90);
        setup2.ePerWm.push_back(120);
        setup2.wLen.push_back(7200);
        setup2.sLen.push_back(30);
        experiments.push_back(setup2);

        Setup setup3;
        setup3.name = "VaryingCountOfParallelWindows";
        setup3.wmNr.push_back(100000);
        setup3.ePerWm.push_back(50);
        setup3.wLen.push_back(12000);
        setup3.wLen.push_back(24000);
        setup3.wLen.push_back(36000);
        setup3.wLen.push_back(48000);
        setup3.sLen.push_back(30);
        experiments.push_back(setup3);

        for (Setup setup : experiments) {
            std::cout << std::endl << setup.name << std::endl;
            for (char item : setup.name)
                std::cout << "*";
            std::cout << std::endl;

            Csv csv(folder, setup.name);

            csv << "Event Count"
            << "Watermark Count"
            << "Event Count per Watermark"
            << "Window Length"
            << "Step Length"
            << "Bucket Window Runtime in Sec"
            << "Sliced Window Runtime in Sec"
            << "Rolling Window Runtime in Sec"
            << std::endl;

            for (long wmNr : setup.wmNr)
                for (long ePerWm : setup.ePerWm)
                    for (long wLen : setup.wLen)
                        for (long sLen : setup.sLen) {
                            experiment(csv, wmNr, ePerWm, wLen, sLen);
                            if (setup.name.compare(experiments.back().name) != 0
                                && setup.sLen.back() != sLen)
                                std::cout << "###########################################" << std::endl
                                << "wmNr=" << std::to_string(wmNr) << std::endl
                                << "ePerWm=" << std::to_string(ePerWm) << std::endl
                                << "wLen=" << std::to_string(wLen) << std::endl
                                << "sLen=" << std::to_string(sLen) << std::endl;
                        }
        }
    }

    exit(EXIT_SUCCESS);
}