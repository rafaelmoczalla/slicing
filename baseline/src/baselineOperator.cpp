#include <baselineOperator.hpp>


BaselineOperator::BaselineOperator(int start) {
    this->bucket = nullptr;
    this->start = start;
    this->size = 86400000;
}

BaselineOperator::BaselineOperator(int start, int size) {
    this->bucket = nullptr;
    this->start = start;
    this->size = size;
}

std::vector<Element *> BaselineOperator::processWatermark(Watermark *watermark) {
    std::vector<Element *> out;

    if (bucket == nullptr) { // in case no records are processed beforehand or bucket has no slices pass through watermark
        out.push_back(watermark);
        return out;
    }

    long wm = watermark->getTs();
    long lw = bucket->getLastWatermark();
    bucket->updateLastWatermark(wm);

    double deltaWindowSum;
    int deltaWindowCount;
    int startIdx;
    long windowStart = wm - size;
    long windowEnd = wm;

    // go to first slice that need to emit bucket result
    int i = bucket->getCheckpoint();

    // emit "watermark bucket"
    deltaWindowSum = 0.0;
    deltaWindowCount = 0;

    // add events from last slice that belong to "watermark bucket"
    for (int i = 0; i < bucket->getEventsNr(); i++) {
        Event *event = bucket->getEvent(i);

        // add value to aggregate if in window
        if (windowStart <= event->getTs() && event->getTs() < windowEnd) {
            deltaWindowSum += event->getValue();
            ++deltaWindowCount;
        }

        // remove if not within window anymore
        if (event->getTs() < windowStart) {
            bucket->removeEvent(i);
            i--;
        }
    }

    float avg = (float) deltaWindowSum / deltaWindowCount;

    out.push_back(new Event(avg, wm));

    // clear bucket for this key if there are no slices anymore
    if (bucket->getEventsNr() == 0) {
        delete bucket;
        bucket = nullptr;
    }

    out.push_back(watermark);
    return out;
}

void BaselineOperator::processElement(Event *event) {
    if (bucket != nullptr && event->getTs() < bucket->getLastWatermark()) { // ignore late events
        delete event;
        return;
    }

    if (bucket == nullptr || bucket->getCheckpoint() < 0) {
        if (bucket != nullptr)
            delete bucket;
        bucket = new Bucket("key");
    }

    // update slice by new event
    bucket->addValue(event->getValue(), event->getTs());

    delete event;
}

BaselineOperator::~BaselineOperator() {
    if (bucket != nullptr)
        delete bucket;
}

std::string BaselineOperator::to_string() {
    std::stringstream out;

    out << "BaselineOperator{bucket=[";

    if (bucket != nullptr) {
        out << bucket->to_string();
    }

    out << "], start=" << std::to_string(start) <<
            ", size=" << std::to_string(size) <<
            '}';

    return out.str();
};