#include <rollingSlidingWindowOperator.hpp>


RollingSlidingWindowOperator::RollingSlidingWindowOperator(long start) {
    this->start = start;
    this->size = 86400000;
    this->step = 300000;
    this->window = new RollingSlidingWindowState("key");
}

RollingSlidingWindowOperator::RollingSlidingWindowOperator(long start, long size, long step) {
    this->start = start;
    this->size = size;
    this->step = step;
    this->window = new RollingSlidingWindowState("key");
}

std::vector<Element *> RollingSlidingWindowOperator::processWatermark(Watermark *watermark) {
    std::vector<Element *> out;

    long wm = watermark->getTs();
    long lwm = window->getLastWatermark();

    // get correct end of first window that need an emit
    long nextToEmitEnd = (lwm - start) % step;
    nextToEmitEnd = lwm - nextToEmitEnd + step;

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

    //if (1410 < wm && wm == 1769)
    //    throw std::runtime_error(to_string());

    window->updateLastWatermark(wm);

    // geet first slice that is a possible candidate to be removed
    // from window avg
    int minus = 0;
    Slice *sliceMinus = window->getSlice(minus);

    // go to last slice of the window that need to emit result
    int preRoll = window->getSliceCount() - 1;
    Slice *slice = window->getSlice(preRoll);

    while (0 < preRoll && nextToEmitEnd <= window->getSlice(preRoll - 1)->getEnd())
        slice = window->getSlice(--preRoll);

    // roll and emit results of pending windows
    while (nextToEmitEnd <= wm) {
        // in case the next slice is in our window we roll the window
        if (slice->getEnd() == nextToEmitEnd) {
            window->addToWindow(slice->getSum(), slice->getCount());

            if (preRoll + 1 < window->getSliceCount())
                slice = window->getSlice(++preRoll);
        }

        // in case there is a slice that we need to remove from the window
        // aggregate substract it
        if (nextToEmitEnd - size == sliceMinus->getEnd()) {
            window->removeFromWindow(sliceMinus->getSum(), sliceMinus->getCount());

            if (minus + 1 < window->getSliceCount())
                sliceMinus = window->getSlice(++minus);
        }

        // add window aggregate to output
        out.push_back(new Event(window->getWindowAvg(), nextToEmitEnd));

        nextToEmitEnd += step;
    }

    // remove slices that are already emitted and disjoint with all remaining windows that will be emitted
    window->removeSlices(wm - size);

    out.push_back(watermark);
    return out;
}

void RollingSlidingWindowOperator::processElement(Event *event) {
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

RollingSlidingWindowOperator::~RollingSlidingWindowOperator() {
    if (window != nullptr)
        delete window;
}

std::string RollingSlidingWindowOperator::to_string() {
    std::stringstream out;

    out << "RollingSlidingWindowOperator{window=[";

    if (window != nullptr) {
        out << window->to_string();
    }

    out << "], start=" << std::to_string(start) <<
            ", size=" << std::to_string(size) <<
            ", step=" << std::to_string(step) <<
            '}';

    return out.str();
};