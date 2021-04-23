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

void BaselineOperator::processWatermark(Watermark *watermark) {
    if (bucket == nullptr) { // in case no records are processed beforehand or bucket has no slices pass through watermark
        // TODO: output watermark
        return;
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

    std::vector<Event *> events = bucket->getEvents();

    // add events from last slice that belong to "watermark bucket"
    for (int i = 0; i < bucket->getEventsNr(); i++) {
        Event * event = events[i];

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

    // TODO: output average for watermark timestamp as window end with current key and avg

    // clear bucket for this key if there are no slices anymore
    if (bucket->getEventsNr() == 0) {
        bucket->clear();
        delete bucket;
        bucket = nullptr;
    }

    return;
}

void BaselineOperator::processElement(Event *event) {
    if (bucket != nullptr && event->getTs() < bucket->getLastWatermark()) // ignore late events
        return;

    if (bucket == nullptr || bucket->getCheckpoint() < 0) {
        bucket = new Bucket("key");
    }

    // update slice by new event
    bucket->addValue(event->getValue(), event->getTs());
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