#ifndef WATERMARK_HPP
#define WATERMARK_HPP

#include <cstdlib>
#include <sstream>
#include <string>

#include <element.hpp>

class Watermark : public Element {
  public:    
    /**
     * Creates a new watermark instance.
     *
     * @param ts Timestamp of watermark instance.
     */
    Watermark(long ts);

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();
};

#endif /* WATERMARK_HPP */