#include <bucket.hpp>


Bucket::Bucket(std::string key) : events() {
    this->lastWatermark = 0;
    this->checkpoint = 0;
    this->key = key;
}

std::vector<Event *> Bucket::getEvents() {
    return events;
}

int Bucket::getCheckpoint() {
    return checkpoint;
}

std::string Bucket::getKey() {
    return key;
}

int Bucket::getLastWatermark() {
    return lastWatermark;
}

void Bucket::updateLastWatermark(int lw) {
    lastWatermark = lw;
}

int Bucket::getEventsNr() {
    return events.size();
}

Event *Bucket::getEvent(int index) {
    return events[index];
}

void Bucket::addValue(double value, long ts) {
    events.push_back(new Event(value, ts));
}

void Bucket::removeEvent(int index) {
    if (!events.empty() && index < events.size()) {
        events.erase(events.begin());

        --checkpoint;
    }
}

void Bucket::clear() {
    for (Event *event : events)
        delete event;
}

std::string Bucket::to_string() {
    std::stringstream out;
    out << "Bucket{events=[";


    if (!events.empty()) {
        Event *last = events.back();
        for (Event *it : events) {
            out << it->to_string();
            if (it != last)
                out << ", " << std::endl;
        }
    }
    
    out << "], lastWatermark=" << std::to_string(lastWatermark) <<
            ", checkpoint=" << std::to_string(checkpoint) <<
            ", key=" << key <<
            '}';

    return out.str();
};