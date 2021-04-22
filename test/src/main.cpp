#include <cassert>
#include <cstdlib>
#include <cstring>
#include <slice.hpp>

int main() {
    assert(0 == 0);

    Slice *slice = new Slice();
    std::string soll = "slice";
    assert(soll.compare(slice->get()) == 0);
    slice->finish();
    soll = "done";
    assert(soll.compare(slice->get()) == 0);

    return 0;
}