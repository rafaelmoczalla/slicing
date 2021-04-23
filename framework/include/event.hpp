#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdlib>
#include <sstream>
#include <string>

#include <element.hpp>

class Event : public Element {
  private:
    double value;
    long ts;

  public:    
    /**
     * Creates a new event instance.
     *
     * @param value Value of event instance.
     * @param ts Timestamp of event instance.
     */
    Event(double value, long ts);
    
    /**
     * Getter function for the event value.
     *
     * @return Value of event instance.
     */
    double getValue();

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();
};

#endif /* EVENT_HPP */