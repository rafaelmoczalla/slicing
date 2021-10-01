#include <bucketSlidingWindowState.hpp>


BucketSlidingWindowState::BucketSlidingWindowState(std::string key) : buckets() {
    this->lastWatermark = 0;
    this->key = key;
}

std::string BucketSlidingWindowState::getKey() {
    return key;
}

long BucketSlidingWindowState::getLastWatermark() {
    return lastWatermark;
}

void BucketSlidingWindowState::updateLastWatermark(long lw) {
    lastWatermark = lw;
}

int BucketSlidingWindowState::getBucketCount() {
    return buckets.size();
}

Slice *BucketSlidingWindowState::getBucket(int index) {
    return buckets[index];
}

void BucketSlidingWindowState::addBucket(int index, long start, long end) {
    buckets.insert(buckets.begin() + index, new Slice(start, end));
}

void BucketSlidingWindowState::addValue(int index, double value, long ts) {
    buckets[index]->add(value, ts);
}

void BucketSlidingWindowState::removeBucket(long ts) {
    while (!buckets.empty() && buckets[0]->getEnd() <= ts) {
        delete buckets.front();
        buckets.erase(buckets.begin());
    }
}

BucketSlidingWindowState::~BucketSlidingWindowState() {
    for (Slice *bucket : buckets)
        delete bucket;

    buckets.clear();
    buckets.resize(0);
}

std::string BucketSlidingWindowState::to_string() {
    std::stringstream out;
    out << "BucketSlidingWindowState{buckets=[";


    if (!buckets.empty()) {
        Slice *last = buckets.back();
        for (Slice *it : buckets) {
            out << std::endl << it->to_string();
            if (it != last)
                out << ", ";
        }

        out << std::endl;
    }

    out << "], lastWatermark=" << std::to_string(lastWatermark) <<
            ", key=" << key <<
            '}';

    return out.str();
};