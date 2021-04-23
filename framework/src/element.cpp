#include <element.hpp>


Element::Element(long ts, int type) {
    this->ts = ts;
    this->type = type;
}

long Element::getTs() {
    return ts;
}

int Element::getType() {
    return type;
}

std::string Element::to_string() {
    std::stringstream out;
    out << "Element{ts=" << std::to_string(ts) <<
            '}';

    return out.str();
}