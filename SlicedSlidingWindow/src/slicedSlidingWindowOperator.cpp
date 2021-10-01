#include <slicedSlidingWindowOperator.hpp>


SlicedSlidingWindowOperator::SlicedSlidingWindowOperator(long start) {
    this->start = start;
    this->size = 86400000;
    this->step = 300000;
    this->window = new SlicedSlidingWindowState("key");
}

SlicedSlidingWindowOperator::SlicedSlidingWindowOperator(long start, long size, long step) {
    this->start = start;
    this->size = size;
    this->step = step;
    this->window = new SlicedSlidingWindowState("key");
}

std::vector<Element *> SlicedSlidingWindowOperator::processWatermark(Watermark *watermark) {
    std::vector<Element *> out;

    long wm = watermark->getTs();
    long lwm = window->getLastWatermark();

    // get correct end of first window that need an emit
    long nextToEmitStart = (lwm - start) % step;
    nextToEmitStart = lwm - nextToEmitStart - size + step;
    long nextToEmitEnd = nextToEmitStart + size;

    // in case no events were processed or window has no slices
    // return NaN values and pass through watermark
    if (window->getSliceCount() < 1) {
        while (nextToEmitEnd < wm) {
            out.push_back(new Event(-1.0, nextToEmitEnd));
            nextToEmitEnd += step;
        }

        out.push_back(watermark);
        return out;
    }

    window->updateLastWatermark(wm);

    double sum = 0;
    double count = 0;

    // go to first slice candidate that may emit result
    int start = 0;
    int j = 0;

    Slice *slice = window->getSlice(start);

    // emit results of pending windows
    while (nextToEmitEnd <= wm) {
        // in case there are slices that belong to the next window to emit
        // use there pre-aggregates
        if (start < window->getSliceCount()) {
            // summarize result of current window and emit
            sum = slice->getSum();
            count = slice->getCount();
            j = start;

            // sum pre-aggregated slices for window
            while (++j < window->getSliceCount() && window->getSlice(j)->getEnd() <= nextToEmitEnd) {
                slice = window->getSlice(j);

                sum += slice->getSum();
                count += slice->getCount();
            }

            // add window aggregate to output
            out.push_back(new Event((double) sum / count, nextToEmitEnd));
        } else {
            out.push_back(new Event(-1.0, nextToEmitEnd));
        }

        nextToEmitStart += step;
        nextToEmitEnd += step;

        if (start < window->getSliceCount() - 1 && window->getSlice(start)->getStart() < nextToEmitStart)
            start++;

        slice = window->getSlice(start);
    }

    // remove slices that are already emitted and disjoint with all remaining windows that will be emitted
    window->removeSlices(wm - size + step);

    out.push_back(watermark);
    return out;
}

void SlicedSlidingWindowOperator::processElement(Event *event) {
    // ignore late events
    if (event->getTs() < window->getLastWatermark()) {
        delete event;
        return;
    }

    // get correct start of slice containing event
    long firstStart = (event->getTs() - start) % step;
    firstStart = event->getTs() - firstStart;
    int i = 0;

    // in case no slices are in state, e.g. no events processed since a long time
    if (window->getSliceCount() < 1) {
        // add new slice containing the event
        window->addSlice(i, firstStart, firstStart + step);
        window->addValue(i, event->getValue(), event->getTs());
    } else {
        // add event to matching slice
        i = window->getSliceCount() - 1; // get index of last slice
        long curStart = window->getSlice(i)->getStart();

        // go to matching slice starting from tail
        while (firstStart < curStart && 0 <= --i)
            curStart = window->getSlice(i)->getStart();

        // in case we need to add slice at the front
        if (i < 0 && firstStart < curStart) {
            i = 0;

            // add new missing slice at front of buffer
            window->addSlice(i, firstStart, firstStart + step);
            window->addValue(i, event->getValue(), event->getTs());
        // add event to slice within buffer
        } else if (curStart <= event->getTs() && event->getTs() < curStart + step) {
            // check if matches slice and add
            window->addValue(i, event->getValue(), event->getTs());
        // in case there is a missing slice add it and add event
        } else {
            window->addSlice(++i, firstStart, firstStart + step);
            window->addValue(i, event->getValue(), event->getTs());
        }
    }

    delete event;
}

SlicedSlidingWindowOperator::~SlicedSlidingWindowOperator() {
    if (window != nullptr)
        delete window;
}

std::string SlicedSlidingWindowOperator::to_string() {
    std::stringstream out;

    out << "SlicedSlidingWindowOperator{window=[";

    if (window != nullptr) {
        out << window->to_string();
    }

    out << "], start=" << std::to_string(start) <<
            ", size=" << std::to_string(size) <<
            ", step=" << std::to_string(step) <<
            '}';

    return out.str();
};