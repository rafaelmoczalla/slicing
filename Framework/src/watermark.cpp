#include <watermark.hpp>


Watermark::Watermark(long ts) : Element(ts, 0) {
}

std::string Watermark::to_string() {
    std::stringstream out;
    out << "Watermark{ts=" << std::to_string(ts) <<
                   ", type=" << std::to_string(type) <<
                   '}';

    return out.str();
}