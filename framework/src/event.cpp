#include <event.hpp>


Event::Event(double value, long ts) : Element(ts, 1) {
    this->value = value;
    this->ts = ts;
}

double Event::getValue() {
    return value;
}

std::string Event::to_string() {
    std::stringstream out;
    out << "Event{value=" << std::to_string(value) <<
            ", ts=" << std::to_string(ts) <<
            '}';

    return out.str();
}