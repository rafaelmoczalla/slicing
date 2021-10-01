#include <bucketSlidingWindowOperator.hpp>


BucketSlidingWindowOperator::BucketSlidingWindowOperator(long start) {
    this->start = start;
    this->size = 86400000;
    this->step = 300000;
    this->window = new BucketSlidingWindowState("key");
}

BucketSlidingWindowOperator::BucketSlidingWindowOperator(long start, long size, long step) {
    this->start = start;
    this->size = size;
    this->step = step;
    this->window = new BucketSlidingWindowState("key");
}

std::vector<Element *> BucketSlidingWindowOperator::processWatermark(Watermark *watermark) {
    std::vector<Element *> out;

    long wm = watermark->getTs();
    long lwm = window->getLastWatermark();

    // get correct end of first window that need an emit
    long nextToEmitEnd = (lwm - start) % step;
    nextToEmitEnd = lwm - nextToEmitEnd + step;

    // in case no events were processed or window has no buckets
    // return NaN values and pass through watermark
    if (window->getBucketCount() < 1) {
        while (nextToEmitEnd < wm) {
            out.push_back(new Event(-1.0, nextToEmitEnd));
            nextToEmitEnd += step;
        }

        out.push_back(watermark);
        return out;
    }

    window->updateLastWatermark(wm);

    // go to first bucket candidate that may emit a result
    int nextBucket = 0;

    Slice *bucket = window->getBucket(nextBucket);
    long nextBucketEnd = bucket->getEnd();

    // summarize and emit results of pending windows
    while (nextToEmitEnd <= wm) {
        // in case the bucket matches the window to emit use bucket data
        if (nextBucket < window->getBucketCount() && nextToEmitEnd == nextBucketEnd) {
            // add window aggregate to output
            out.push_back(new Event(bucket->getSum() / bucket->getCount(), nextBucketEnd));

            // in case there is another bucket move window to next bucket
            if (nextBucket + 1 < window->getBucketCount()) {
                bucket = window->getBucket(++nextBucket);
                nextBucketEnd = bucket->getEnd();
            }
        // in case window has no bucket
        // return NaN values and pass through watermark
        } else {
            out.push_back(new Event(-1.0, nextToEmitEnd));
        }

        nextToEmitEnd += step;
    }

    // remove slices that are already emitted and disjoint
    // with all remaining windows that will be emitted
    window->removeBucket(wm);

    out.push_back(watermark);
    return out;
}

void BucketSlidingWindowOperator::processElement(Event *event) {
    // ignore late events
    if (event->getTs() < window->getLastWatermark()) {
        delete event;
        return;
    }

    // get correct start of first bucket containing event
    long firstStart = (event->getTs() - start) % step;
    firstStart = event->getTs() - firstStart - size + step;
    int i = 0;

    // in case no buckets are in state, e.g. no events processed since a long time
    if (window->getBucketCount() < 1) {
        // create all buckets containing the objective event and add it
        while (firstStart <= event->getTs()) {
            window->addBucket(i, firstStart, firstStart + size);
            window->addValue(i, event->getValue(), event->getTs());
            firstStart += step;
            i++;
        }
    } else {
        // add event to all matching buckets
        i = window->getBucketCount() - 1; // get index of last bucket
        long curStart = window->getBucket(i)->getStart();

        // go to first matching bucket starting from tail
        while (firstStart < curStart && 0 <= --i) {
            curStart = window->getBucket(i)->getStart();
        }

        // in case we need to add bucket(s) at the front
        if (i < 0 && firstStart < curStart) {
            i = 0;

            // add new missing buckets at front of buffer
            while (firstStart <= event->getTs() && firstStart < curStart) {
                window->addBucket(i, firstStart, firstStart + size);
                window->addValue(i, event->getValue(), event->getTs());
                firstStart += step;
                i++;
            }
        }

        // add event to remaining matching buckets in buffer
        while (i < window->getBucketCount() && firstStart <= event->getTs() && event->getTs() < firstStart + size) {
            // check if there is a "hole" between two buckets that we need to fill
            if (firstStart < window->getBucket(i)->getStart())
                window->addBucket(i, firstStart, firstStart + size);

            window->addValue(i, event->getValue(), event->getTs());
            firstStart += step;
            i++;
        }

        // in case there are missing buckets at tail of buffer append them
        while (firstStart <= event->getTs()) {
            i = window->getBucketCount();
            window->addBucket(i, firstStart, firstStart + size);
            window->addValue(i, event->getValue(), event->getTs());
            firstStart += step;
        }
    }

    delete event;
}

BucketSlidingWindowOperator::~BucketSlidingWindowOperator() {
    if (window != nullptr)
        delete window;
}

std::string BucketSlidingWindowOperator::to_string() {
    std::stringstream out;

    out << "BucketSlidingWindowOperator{window=[";

    if (window != nullptr) {
        out << window->to_string();
    }

    out << "], start=" << std::to_string(start) <<
            ", size=" << std::to_string(size) <<
            ", step=" << std::to_string(step) <<
            '}';

    return out.str();
};