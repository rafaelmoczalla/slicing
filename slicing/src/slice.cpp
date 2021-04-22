#include <slice.hpp>

Slice::Slice() {
    state = "slice";
}

void Slice::finish() {
    state = "done";
}

std::string Slice::get() {
    return state;
}