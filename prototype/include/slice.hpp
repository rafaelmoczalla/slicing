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
    double windowSum;
    int windowCount;
    double sum;
    int count;
    long start;
    long end; // end of window and slice

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
     * Getter function of window and slice end.
     *
     * @return End of window and slice.
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
     * Getter function of window event sum.
     *
     * @return Sum of window.
     */
    double getWindowSum();

    /**
     * Getter function of window event count.
     *
     * @return Event count of window.
     */
    int getWindowCount();

    /**
     * Getter function for average value of a window.
     *
     * @return Average of window.
     */
    double getWindowAvg();

    /**
     * Adds elements to window.
     *
     * @param value Value of additional window sum.
     * @param count Count of additional window elements.
     */
    void addToWindow(double value, int count);

    /**
     * Adds an element to slice and pre-aggregated window.
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