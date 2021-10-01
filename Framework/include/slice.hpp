#ifndef SLICE_HPP
#define SLICE_HPP

#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>

#include <event.hpp>

class Slice {
  private:
    std::vector<Event *> events;
    double sum;
    int count;
    long start; // start of slice time range
    long end; // end of slice time range

  public:
    /**
     * Creates a slice instance.
     *
     * @param start Start timestamp of slice instance.
     * @param end End timestamp of slice instance.
     */
    Slice(long start, long end);

    /**
     * Check if sum is empty.
     *
     * @return true if events list is empty and otherwise false.
     */
    bool empty();

    /**
     * Determines if event belongs to slice is in slice or not.
     *
     * @param ts Timestamp of the event.
     *
     * @return   0 - in slice
     *         < 0 - event time < slice start
     *         > 0 - slice end <= event time.
     */
    int in(long ts);

    /**
     * Getter function of events in a slice instance.
     *
     * @return Events in a slice instance.
     */
    std::vector<Event *> getEvents();

    /**
     * Getter function of slice start.
     *
     * @return Start of slice.
     */
    long getStart();

    /**
     * Getter function of slice end.
     *
     * @return End of slice.
     */
    long getEnd();

    /**
     * Getter function of slice event sum.
     *
     * @return Sum of slice.
     */
    double getSum();

    /**
     * Getter function of slice event count.
     *
     * @return Event count of slice.
     */
    int getCount();

    /**
     * Adds an element to slice.
     *
     * @param value Value of the event.
     * @param timestamp Timestamp of the event.
     */
    void add(double value, long timestamp);

    /**
     * Clears data in instance.
     */
    ~Slice();

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();
};

#endif /* SLICE_HPP */