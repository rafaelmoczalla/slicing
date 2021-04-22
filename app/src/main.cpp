#include <iostream>
#include <cstdlib>
#include <slice.hpp>

int main() {
    std::cout << "Hello, World!" << std::endl;

    Slice *slice = new Slice();
    std::cout << slice->get() << std::endl;
    slice->finish();
    std::cout << slice->get() << std::endl;

    return 0;
}