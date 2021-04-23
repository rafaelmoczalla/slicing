#include <cassert>
#include <cstdlib>
#include <cstring>

#include <slice.hpp>
#include <state.hpp>
#include <bucket.hpp>
#include <baselineOperator.hpp>
#include <sliceOperator.hpp>

void sliceTest() {
    Slice *slice = new Slice(0, 10000);
    std::string soll = "Slice{events=[], sum=0.000000, count=0, start=0, end=10000}";
    assert(soll.compare(slice->to_string()) == 0);

    slice->add(12, 14);
    soll = "Slice{events=[Event{value=12.000000, ts=14}], sum=12.000000, count=1, start=0, end=10000}";
    assert(soll.compare(slice->to_string()) == 0);
}

void stateTest() {
    State *state = new State("key", 0, 1000);
    std::string soll = "State{slices=[Slice{events=[], sum=0.000000, count=0, start=0, end=1000}], lastWatermark=0, checkpoint=0, key=key, executionMode=0}";
    assert(soll.compare(state->to_string()) == 0);
    
    state->addValue(0, 12, 14);
    soll = "State{slices=[Slice{events=[Event{value=12.000000, ts=14}], sum=12.000000, count=1, start=0, end=1000}], lastWatermark=0, checkpoint=0, key=key, executionMode=0}";
    assert(soll.compare(state->to_string()) == 0);
}

void bucketTest() {
    Bucket *bucket = new Bucket("key");
    std::string soll = "Bucket{events=[], lastWatermark=0, checkpoint=0, key=key}";
    assert(soll.compare(bucket->to_string()) == 0);

    bucket->addValue(12, 14);
    soll = "Bucket{events=[Event{value=12.000000, ts=14}], lastWatermark=0, checkpoint=0, key=key}";
    assert(soll.compare(bucket->to_string()) == 0);
}

void baselineTest() {
    BaselineOperator *baseline = new BaselineOperator(0);
    std::string soll = "BaselineOperator{bucket=[], start=0, size=86400000}";
    assert(soll.compare(baseline->to_string()) == 0);

    baseline->processElement(new Event(12, 14));
    soll = "BaselineOperator{bucket=[Bucket{events=[Event{value=12.000000, ts=14}], lastWatermark=0, checkpoint=0, key=key}], start=0, size=86400000}";
    assert(soll.compare(baseline->to_string()) == 0);
}

void prototypeTest() {
    SliceOperator *prototype = new SliceOperator(0);
    std::string soll = "SliceOperator{window=[], start=0, size=86400000, step=300000, doubleStep=600000, vDeltaIdx=287}";
    assert(soll.compare(prototype->to_string()) == 0);

    prototype->processElement(new Event(12, 14));
    soll = "SliceOperator{window=[State{slices=[Slice{events=[Event{value=12.000000, ts=14}], sum=12.000000, count=1, start=0, end=300000}], lastWatermark=0, checkpoint=0, key=key, executionMode=0}], start=0, size=86400000, step=300000, doubleStep=600000, vDeltaIdx=287}";
    assert(soll.compare(prototype->to_string()) == 0);
}

int main() {
    sliceTest();
    stateTest();
    bucketTest();
    baselineTest();
    prototypeTest();

    return 0;
}