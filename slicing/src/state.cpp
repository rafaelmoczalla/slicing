#include <state.hpp>


State::State(std::string key, int start, int end) : slices() {
    this->slices.push_back(new Slice(start, end));
    this->lastWatermark = start;
    this->checkpoint = 0;
    this->key = key;
    this->executionMode = 0;
}

int State::getCheckpoint() {
    return checkpoint;
}

std::string State::getKey() {
    return key;
}

int State::getLastWatermark() {
    return lastWatermark;
}

void State::updateLastWatermark(int lw) {
    lastWatermark = lw;
}

int State::getSlicesNr() {
    return slices.size();
}

int State::getEndOfSlice(int index) {
    return slices[index]->getEnd();
}

Slice *State::getSlice(int index) {
    return slices[index];
}

int State::in(int index, int ts) {
    return slices[index]->in(ts);
}

bool State::preAggregate(int index) {
    return checkpoint < index;
}

void State::addSlice(int step) {
    int lastEnd = slices.back()->getEnd();
    
    slices.push_back(new Slice(lastEnd, lastEnd + step));
}

void State::addValue(int index, double value, long ts) {
    slices[index]->add(value, ts);
}

void State::addPreAggregate(int index, double sum, int count) {
    slices[index]->addToWindow(sum, count);

    ++checkpoint;
}

void State::removeSlices(long ts) {
    while (!slices.empty() && slices[0]->getEnd() <= ts) {
        slices.erase(slices.begin());

        --checkpoint;
    }
}

void State::removeEmptyHead() {
    while (!slices.empty() && slices[0]->empty()) {
        slices.erase(slices.begin());

        --checkpoint;
    }
}

void State::clear() {
    for (Slice *slice : slices) {
        slice->clear();
        delete slice;
    }
}

std::string State::to_string() {
    std::stringstream out;
    out << "State{slices=[";


    if (!slices.empty()) {
        Slice *last = slices.back();
        for (Slice *it : slices) {
            out << it->to_string();
            if (it != last)
                out << ", " << std::endl;
        }
    }
    
    out << "], lastWatermark=" << std::to_string(lastWatermark) <<
            ", checkpoint=" << std::to_string(checkpoint) <<
            ", key=" << key <<
            ", executionMode=" << std::to_string(executionMode) <<
            '}';

    return out.str();
};