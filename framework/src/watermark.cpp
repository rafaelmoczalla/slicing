#include <watermark.hpp>


Watermark::Watermark(long ts) : Element(ts, 0) {
    this->ts = ts;
}

std::string Watermark::to_string() {
    std::stringstream out;
    out << "Watermark{ts=" << std::to_string(ts) <<
            '}';

    return out.str();
}