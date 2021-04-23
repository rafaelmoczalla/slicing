#include <sliceOperator.hpp>


SliceOperator::SliceOperator(int start) {
    this->window = nullptr;
    this->start = start;
    this->size = 86400000;
    this->step = 300000;
    this->doubleStep = 2 * step;
    this->vDeltaIdx = (int)(size / step) - 1;
}

SliceOperator::SliceOperator(int start, int size, int step) {
    this->window = nullptr;
    this->start = start;
    this->size = size;
    this->step = step;
    this->doubleStep = 2 * step;
    this->vDeltaIdx = (int)(size / step) - 1;
}

std::vector<Element *> SliceOperator::processWatermark(Watermark *watermark) {
    std::vector<Element *> out;

    if (window == nullptr || window->getCheckpoint() < 0) { // in case no records are processed beforehand or window has no slices pass through watermark
        out.push_back(watermark);
        return out;
    }

    long wm = watermark->getTs();
    long lw = window->getLastWatermark();
    window->updateLastWatermark(wm);

    double deltaWindowSum;
    int deltaWindowCount;
    int startIdx;
    long curWindowEnd;

    // go to first slice that need to emit window result
    int i = window->getCheckpoint();

    curWindowEnd = window->getEndOfSlice(i);

    // emit results of pending windows
    while (curWindowEnd < wm) {
        if (0 < i) { // use pre-aggregate if possible
            if (window->preAggregate(i)) {
                // get aggregate of previous windows
                deltaWindowSum = window->getSlice(i - 1)->getWindowSum();

                startIdx = i - 1 - vDeltaIdx; // first slice of previous window

                if (0 <= startIdx) { // check if there is a slice that we need to subtract
                    deltaWindowSum -= window->getSlice(startIdx)->getSum();
                    deltaWindowCount -= window->getSlice(startIdx)->getCount();
                }

                window->addPreAggregate(i, deltaWindowSum, deltaWindowCount);
            }
        }

        ++i;

        if (i == window->getSlicesNr()) // check if we need to add an empty slice with pre-aggregation
            window->addSlice(step);

        curWindowEnd = window->getEndOfSlice(i);
    }

    // emit "watermark window"
    deltaWindowSum = 0.0;
    deltaWindowCount = 0;

    if (0 < i) { // use pre-aggregated results of previous windows if not already done
        deltaWindowSum = window->getSlice(i - 1)->getWindowSum();
        deltaWindowCount = window->getSlice(i - 1)->getWindowCount();

        startIdx = i - 1 - vDeltaIdx; // first slice of previous window

        if (0 <= startIdx) { // check if there is a slice that we need to subtract from first 24h window
            deltaWindowSum -= window->getSlice(startIdx)->getSum();
            deltaWindowCount -= window->getSlice(startIdx)->getCount();

            long watermarkWindowStart = wm - size;

            for (Event *event : window->getSlice(startIdx)->getEvents()) {
                if (event->getTs() < watermarkWindowStart) {
                    deltaWindowSum -= event->getValue();
                    --deltaWindowCount;
                }
            }
        }

        if (1 < i && !window->getSlice(i - 2)->empty()) {
            long before10MinInSec = wm - doubleStep;

            for (Event *event : window->getSlice(i - 2)->getEvents())
                if (before10MinInSec <= event->getTs())
                    break;
        }
    }

    // add events from last slice that belong to "watermark window"
    for (Event *event : window->getSlice(i)->getEvents()) {
        if (event->getTs() < wm) {
            deltaWindowSum += event->getValue();
            ++deltaWindowCount;
        }
    }

    float avg = (float) deltaWindowSum / deltaWindowCount;

    out.push_back(new Event(avg, wm));

    // remove slices that are already emitted and disjoint with all remaining windows that will be emitted
    window->removeSlices(wm - size);

    // additionally remove all empty slices from tail
    window->removeEmptyHead();

    // clear window for this key if there are no slices anymore
    if (window->getSlicesNr() == 0) {
        delete window;
        window = nullptr;
    }

    out.push_back(watermark);
    return out;
}

void SliceOperator::processElement(Event *event) {
    if (window != nullptr && event->getTs() < window->getLastWatermark()) { // ignore late events
        delete event;
        return;
    }

    if (window == nullptr || window->getCheckpoint() < 0) {
        long newStart = (event->getTs() - start) % step;
        newStart = event->getTs() - newStart; // get correct start of window in case event is very late
        if (window != nullptr)
            delete window;
        window = new State("key", newStart, newStart + step);
    }

    // add event that actually has a matching key
    int i = window->getSlicesNr() - 1;
    int in = window->in(i, event->getTs());

    // search for correct slice
    while (in != 0) {
        if (in < 0) { // go one slice to the past
            --i;
        } else {
            window->addSlice(step); // add new slice to end

            ++i;
        }

        in = window->in(i, event->getTs());
    }

    // update slice by new event
    window->addValue(i, event->getValue(), event->getTs());

    delete event;
}

SliceOperator::~SliceOperator() {
    if (window != nullptr)
        delete window;
}

std::string SliceOperator::to_string() {
    std::stringstream out;

    out << "SliceOperator{window=[";

    if (window != nullptr) {
        out << window->to_string();
    }

    out << "], start=" << std::to_string(start) <<
            ", size=" << std::to_string(size) <<
            ", step=" << std::to_string(step) <<
            ", doubleStep=" << std::to_string(doubleStep) <<
            ", vDeltaIdx=" << std::to_string(vDeltaIdx) <<
            '}';

    return out.str();
};