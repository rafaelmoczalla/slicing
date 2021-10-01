#include <rollingSlidingWindowState.hpp>


RollingSlidingWindowState::RollingSlidingWindowState(std::string key) : slices() {
    this->lastWatermark = 0;
    this->winSum = 0.0;
    this->winCount = 0;
    this->key = key;
}

std::string RollingSlidingWindowState::getKey() {
    return key;
}

long RollingSlidingWindowState::getLastWatermark() {
    return lastWatermark;
}

void RollingSlidingWindowState::updateLastWatermark(long lw) {
    lastWatermark = lw;
}

int RollingSlidingWindowState::getSliceCount() {
    return slices.size();
}

int RollingSlidingWindowState::getEndOfSlice(int index) {
    return slices[index]->getEnd();
}

Slice *RollingSlidingWindowState::getSlice(int index) {
    return slices[index];
}

int RollingSlidingWindowState::in(int index, long ts) {
    return slices[index]->in(ts);
}

void RollingSlidingWindowState::addSlice(int index, long start, long end) {
    slices.insert(slices.begin() + index, new Slice(start, end));
}

void RollingSlidingWindowState::addValue(int index, double value, long ts) {
    slices[index]->add(value, ts);
}

void RollingSlidingWindowState::addToWindow(double sum, int count) {
    winSum += sum;
    winCount += count;
}

void RollingSlidingWindowState::removeFromWindow(double sum, int count) {
    winSum -= sum;
    winCount -= count;
}

double RollingSlidingWindowState::getWindowAvg() {
    if (winCount < 1)
        return -1.0;

    return winSum / winCount;
}

void RollingSlidingWindowState::removeSlices(long ts) {
    while (!slices.empty() && slices.front()->getEnd() <= ts) {
        delete slices.front();
        slices.erase(slices.begin());
    }
}

RollingSlidingWindowState::~RollingSlidingWindowState() {
    for (Slice *slice : slices) {
        delete slice;
    }

    slices.clear();
    slices.reserve(0);
}

std::string RollingSlidingWindowState::to_string() {
    std::stringstream out;
    out << "RollingSlidingWindowState{slices=[";


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
            ", winSum=" << std::to_string(winSum) <<
            ", winCount=" << std::to_string(winCount) <<
            ", key=" << key <<
            '}';

    return out.str();
};