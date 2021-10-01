#include <slice.hpp>

Slice::Slice(long start, long end) : events() {
    this->sum = 0.0;
    this->count = 0;
    this->start = start;
    this->end = end;
}

bool Slice::empty() {
    return events.empty();
}

int Slice::in(long ts) {
    if (ts < start)
        return -1;

    if (ts < end)
        return 0;

    return 1;
}

std::vector<Event *> Slice::getEvents() {
    return events;
}

long Slice::getStart() {
    return start;
}

long Slice::getEnd() {
    return end;
}

double Slice::getSum() {
    return sum;
}

int Slice::getCount() {
    return count;
}

void Slice::add(double value, long timestamp) {
    events.push_back(new Event(value, timestamp));
    sum += value;
    ++count;
}

Slice::~Slice() {
    for (Event *event : events)
        delete event;

    events.clear();
    events.reserve(0);
}

std::string Slice::to_string() {
    std::stringstream out;
    out << "Slice{events=[";

    if (!events.empty()) {
        Event *last = events.back();

        for (Event *it : events) {
            out << std::endl << (it->to_string());
            if (it != last)
                out << ", ";
        }

        out << std::endl;
    }

    out << "], sum=" << std::to_string(sum) <<
            ", count=" << std::to_string(count) <<
            ", start=" << std::to_string(start) <<
            ", end=" << std::to_string(end) <<
            '}';

    return out.str();
}