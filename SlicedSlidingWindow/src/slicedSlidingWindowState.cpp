#include <slicedSlidingWindowState.hpp>


SlicedSlidingWindowState::SlicedSlidingWindowState(std::string key) : slices() {
    this->lastWatermark = 0;
    this->key = key;
}

std::string SlicedSlidingWindowState::getKey() {
    return key;
}

long SlicedSlidingWindowState::getLastWatermark() {
    return lastWatermark;
}

void SlicedSlidingWindowState::updateLastWatermark(long lw) {
    lastWatermark = lw;
}

int SlicedSlidingWindowState::getSliceCount() {
    return slices.size();
}

Slice *SlicedSlidingWindowState::getSlice(int index) {
    return slices[index];
}

void SlicedSlidingWindowState::addSlice(int index, long start, long end) {
    slices.insert(slices.begin() + index, new Slice(start, end));
}

void SlicedSlidingWindowState::addValue(int index, double value, long ts) {
    slices[index]->add(value, ts);
}

void SlicedSlidingWindowState::removeSlices(long ts) {
    while (!slices.empty() && slices[0]->getEnd() <= ts) {
        delete slices.front();
        slices.erase(slices.begin());
    }
}

SlicedSlidingWindowState::~SlicedSlidingWindowState() {
    for (Slice *slice : slices) {
        delete slice;
    }

    slices.clear();
    slices.reserve(0);
}

std::string SlicedSlidingWindowState::to_string() {
    std::stringstream out;
    out << "SlicedSlidingWindowState{slices=[";


    if (!slices.empty()) {
        Slice *last = slices.back();
        for (Slice *it : slices) {
            out << std::endl << it->to_string();
            if (it != last)
                out << ", " << std::endl;
        }

        out << std::endl;
    }

    out << "], lastWatermark=" << std::to_string(lastWatermark) <<
            ", key=" << key <<
            '}';

    return out.str();
};