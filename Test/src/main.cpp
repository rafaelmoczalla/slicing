#include <cassert>
#include <cstdlib>
#include <cstring>
#include <random>
#include <cmath>

#include <bucketSlidingWindowOperator.hpp>
#include <slicedSlidingWindowState.hpp>
#include <slicedSlidingWindowOperator.hpp>
#include <slice.hpp>
#include <rollingSlidingWindowState.hpp>
#include <rollingSlidingWindowOperator.hpp>
#include <iostream>

void bucketStateTest() {
    BucketSlidingWindowState *state = new BucketSlidingWindowState("key");
    std::string soll = "BucketSlidingWindowState{buckets=[], lastWatermark=0, key=key}";
    assert(soll.compare(state->to_string()) == 0);

    state->addBucket(0, 0, 1000);
    state->addValue(0, 12, 14);
    soll = "BucketSlidingWindowState{buckets=[\n"
    "Slice{events=[\nEvent{value=12.000000, ts=14, type=1}\n], sum=12.000000, count=1, start=0, end=1000}"
    "\n], lastWatermark=0, key=key}";
    assert(soll.compare(state->to_string()) == 0);

    delete state;
}

void bucketOperatorTest() {
    BucketSlidingWindowOperator *bucketOperator = new BucketSlidingWindowOperator(0, 1000, 500);
    std::string soll = "BucketSlidingWindowOperator{window=[BucketSlidingWindowState{buckets=[], lastWatermark=0, key=key}], start=0, size=1000, step=500}";
    assert(soll.compare(bucketOperator->to_string()) == 0);

    bucketOperator->processElement(new Event(12, 14));
    soll = "BucketSlidingWindowOperator{window=[BucketSlidingWindowState{buckets=[\n"
    "Slice{events=[\nEvent{value=12.000000, ts=14, type=1}\n], sum=12.000000, count=1, start=-500, end=500}, \n"
    "Slice{events=[\nEvent{value=12.000000, ts=14, type=1}\n], sum=12.000000, count=1, start=0, end=1000}"
    "\n], lastWatermark=0, key=key}], start=0, size=1000, step=500}";
    assert(soll.compare(bucketOperator->to_string()) == 0);

    std::vector<Element *> out = bucketOperator->processWatermark(new Watermark(30));
    soll = "BucketSlidingWindowOperator{window=[BucketSlidingWindowState{buckets=[\n"
    "Slice{events=[\nEvent{value=12.000000, ts=14, type=1}\n], sum=12.000000, count=1, start=-500, end=500}, \n"
    "Slice{events=[\nEvent{value=12.000000, ts=14, type=1}\n], sum=12.000000, count=1, start=0, end=1000}"
    "\n], lastWatermark=30, key=key}], start=0, size=1000, step=500}";
    assert(soll.compare(bucketOperator->to_string()) == 0);

    delete bucketOperator;
}

void sliceTest() {
    Slice *slice = new Slice(0, 10000);
    std::string soll = "Slice{events=[], sum=0.000000, count=0, start=0, end=10000}";
    assert(soll.compare(slice->to_string()) == 0);

    slice->add(12, 14);
    soll = "Slice{events=[\nEvent{value=12.000000, ts=14, type=1}\n], sum=12.000000, count=1, start=0, end=10000}";
    assert(soll.compare(slice->to_string()) == 0);

    delete slice;
}

void slicedStateTest() {
    SlicedSlidingWindowState *state = new SlicedSlidingWindowState("key");
    std::string soll = "SlicedSlidingWindowState{slices=[], lastWatermark=0, key=key}";
    assert(soll.compare(state->to_string()) == 0);

    state->addSlice(0, 0, 1000);
    state->addValue(0, 12, 14);
    soll = "SlicedSlidingWindowState{slices=[\n"
    "Slice{events=[\nEvent{value=12.000000, ts=14, type=1}\n], sum=12.000000, count=1, start=0, end=1000}"
    "\n], lastWatermark=0, key=key}";
    assert(soll.compare(state->to_string()) == 0);

    delete state;
}

void slicedWindowOperatorTest() {
    SlicedSlidingWindowOperator *slicingOperator = new SlicedSlidingWindowOperator(0);
    std::string soll = "SlicedSlidingWindowOperator{window=[SlicedSlidingWindowState{slices=["
    "], lastWatermark=0, key=key}], start=0, size=86400000, step=300000}";
    assert(soll.compare(slicingOperator->to_string()) == 0);

    slicingOperator->processElement(new Event(12, 14));
    soll = "SlicedSlidingWindowOperator{window=[SlicedSlidingWindowState{slices=[\n"
    "Slice{events=[\nEvent{value=12.000000, ts=14, type=1}\n], sum=12.000000, count=1, start=0, end=300000}"
    "\n], lastWatermark=0, key=key}], start=0, size=86400000, step=300000}";
    assert(soll.compare(slicingOperator->to_string()) == 0);

    std::vector<Element *> out = slicingOperator->processWatermark(new Watermark(30));
    soll = "SlicedSlidingWindowOperator{window=[SlicedSlidingWindowState{slices=[\n"
    "Slice{events=[\nEvent{value=12.000000, ts=14, type=1}\n], sum=12.000000, count=1, start=0, end=300000}"
    "\n], lastWatermark=30, key=key}], start=0, size=86400000, step=300000}";
    assert(soll.compare(slicingOperator->to_string()) == 0);

    delete slicingOperator;
}

void rollingStateTest() {
    RollingSlidingWindowState *state = new RollingSlidingWindowState("key");
    std::string soll = "RollingSlidingWindowState{slices=[], lastWatermark=0, winSum=0.000000, winCount=0, key=key}";
    assert(soll.compare(state->to_string()) == 0);

    state->addSlice(0, 0, 1000);
    state->addValue(0, 12, 14);
    soll = "RollingSlidingWindowState{slices=[\n"
    "Slice{events=[\nEvent{value=12.000000, ts=14, type=1}\n], sum=12.000000, count=1, start=0, end=1000}"
    "\n], lastWatermark=0, winSum=0.000000, winCount=0, key=key}";
    assert(soll.compare(state->to_string()) == 0);

    delete state;
}

void rollingWindowOperatorTest() {
    RollingSlidingWindowOperator *rollingOperator = new RollingSlidingWindowOperator(0);
    std::string soll = "RollingSlidingWindowOperator{window=[RollingSlidingWindowState{slices=["
    "], lastWatermark=0, winSum=0.000000, winCount=0, key=key}], start=0, size=86400000, step=300000}";
    assert(soll.compare(rollingOperator->to_string()) == 0);

    rollingOperator->processElement(new Event(12, 14));
    soll = "RollingSlidingWindowOperator{window=[RollingSlidingWindowState{slices=[\n"
    "Slice{events=[\nEvent{value=12.000000, ts=14, type=1}\n], sum=12.000000, count=1, start=0, end=300000}"
    "\n], lastWatermark=0, winSum=0.000000, winCount=0, key=key}], start=0, size=86400000, step=300000}";
    assert(soll.compare(rollingOperator->to_string()) == 0);

    std::vector<Element *> out = rollingOperator->processWatermark(new Watermark(300000));
    soll = "RollingSlidingWindowOperator{window=[RollingSlidingWindowState{slices=[\n"
    "Slice{events=[\nEvent{value=12.000000, ts=14, type=1}\n], sum=12.000000, count=1, start=0, end=300000}"
    "\n], lastWatermark=300000, winSum=12.000000, winCount=1, key=key}], start=0, size=86400000, step=300000}";
    assert(soll.compare(rollingOperator->to_string()) == 0);

    delete rollingOperator;
}

void sameResultTest() {
    long wmNr   =   10000; // number of watermarks to emit
    long ePerWm =      50; // events between each pair of consecutive watermarks
    long wLen   =    1200; // window length
    long sLen   =      30; // slice length

    std::default_random_engine generatorBucket(117);
    std::default_random_engine generatorSlicing(117);
    std::default_random_engine generatorRolling(117);

    std::uniform_real_distribution<double> randomValue(0.0, 200.0);
    std::uniform_int_distribution<long> randomOutOfOrder(-10, 20);
    std::uniform_int_distribution<long> randomNrOfEventsTilWatermark(0, ePerWm);

    long iWm;
    long ts;
    long cWm = 0;
    long lWm = 0;
    std::vector<std::vector<Element *>> gt; // ground truth buffer
    std::vector<std::vector<Element *>> out; // temporary output buffer

    // bucketOperator
    iWm = randomNrOfEventsTilWatermark(generatorBucket);
    ts = 10;
    cWm = 0;
    lWm = 0;

    BucketSlidingWindowOperator *bucketOperator = new BucketSlidingWindowOperator(0, wLen, sLen);

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
        gt.push_back(bucketOperator->processWatermark(new Watermark(cWm)));
        lWm = cWm;

        iWm = randomNrOfEventsTilWatermark(generatorBucket);
    }

    delete bucketOperator;


    // slicedOperator
    iWm = randomNrOfEventsTilWatermark(generatorSlicing);
    ts = 10;
    cWm = 0;
    lWm = 0;

    SlicedSlidingWindowOperator *slicedOperator = new SlicedSlidingWindowOperator(0, wLen, sLen);

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
        out.push_back(slicedOperator->processWatermark(new Watermark(cWm)));
        lWm = cWm;

        iWm = randomNrOfEventsTilWatermark(generatorSlicing);
    }

    delete slicedOperator;

    // check if outputs are equal
    assert(gt.size() == out.size());

    for (int i = 0; i < gt.size(); i++) {
        assert(gt[i].size() == out[i].size());

        for (int j = 0; j < gt[i].size(); j++) {
            assert(gt[i][j]->getTs() == out[i][j]->getTs());
            assert(gt[i][j]->getType() == out[i][j]->getType());

            if (gt[i][j]->getType() == 1) {
                Event *eGt = (Event *) gt[i][j];
                Event *eOut = (Event *) out[i][j];

                assert(std::abs(eGt->getValue() - eOut->getValue()) < 0.000001);
            }
        }
    }

    // clear memory of out
    for (std::vector<Element *> item : out) {
        for (Element *o : item)
            delete o;

        item.clear();
        item.resize(0);
    }

    out.clear();
    out.resize(0);


    // rollingOperator
    iWm = randomNrOfEventsTilWatermark(generatorRolling);
    ts = 10;
    cWm = 0;
    lWm = 0;

    RollingSlidingWindowOperator *prototype = new RollingSlidingWindowOperator(0, wLen, sLen);

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
        out.push_back(prototype->processWatermark(new Watermark(cWm)));
        lWm = cWm;

        iWm = randomNrOfEventsTilWatermark(generatorRolling);
    }

    delete prototype;

    // check if outputs are equal
    assert(gt.size() == out.size());

    for (int i = 0; i < gt.size(); i++) {
        assert(gt[i].size() == out[i].size());

        for (int j = 0; j < gt[i].size(); j++) {
            assert(gt[i][j]->getTs() == out[i][j]->getTs());
            assert(gt[i][j]->getType() == out[i][j]->getType());

            if (gt[i][j]->getType() == 1) {
                Event *eGt = (Event *) gt[i][j];
                Event *eOut = (Event *) out[i][j];

                assert(std::abs(eGt->getValue() - eOut->getValue()) < 0.000001);
            }
        }
    }

    // clear memory of gt
    for (std::vector<Element *> item : gt) {
        for (Element *o : item)
            delete o;

        item.clear();
        item.resize(0);
    }

    gt.clear();
    gt.resize(0);

    // clear memory of out
    for (std::vector<Element *> item : out) {
        for (Element *o : item)
            delete o;

        item.clear();
        item.resize(0);
    }

    out.clear();
    out.resize(0);
}

int main() {
    bucketStateTest();
    bucketOperatorTest();
    sliceTest();
    slicedStateTest();
    slicedWindowOperatorTest();
    rollingStateTest();
    rollingWindowOperatorTest();
    sameResultTest();

    return 0;
}