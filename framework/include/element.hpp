#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <cstdlib>
#include <sstream>
#include <string>

class Element {
  protected:
    long ts;
    int type;

  public:    
    /**
     * Creates a new element instance.
     *
     * @param ts Timestamp of element instance.
     * @param type Type of element instance.
     */
    Element(long ts, int type);

    /**
     * Getter function for an element ts.
     *
     * @return Timestamp of element instance.
     */
    long getTs();

    /**
     * Getter function for an element type.
     *
     * @return Type of element instance.
     */
    int getType();

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();
};

#endif /* ELEMENT_HPP */