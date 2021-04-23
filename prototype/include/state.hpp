#ifndef STATE_HPP
#define STATE_HPP

#include <cstdlib>
#include <string>
#include <vector>

#include <slice.hpp>

class State {
  private:
    std::vector<Slice *> slices;
    int lastWatermark;
    int checkpoint;
    std::string key;
    int executionMode; // 0 - pre-aggregated window slicing, 1 - slice pre-aggregation only & no window pre-aggregation

  public:
    /**
     * Creates state instance with key, start and end.
     * 
     * @param key Key of state instance.
     * @param start Start timestamp of first slice instance.
     * @param end End timestamp of first slice instance.
     */
    State(std::string key, int start, int end);

    /**
     * Getter function for checkpoint of state instance.
     * 
     * @return Checkpoint of state instance.
     */
    int getCheckpoint();

    /**
     * Getter function for key.
     * 
     * @return Key of state instance.
     */
    std::string getKey();

    /**
     * Getter function for last watermark which was processed.
     * 
     * @return Last watermark processed.
     */
    int getLastWatermark();

    /**
     * Setter function to store last watermark which was processed.
     * 
     * @param lw Watermark to store.
     */
    void updateLastWatermark(int lw);

    /**
     * Getter function to get number of slices.
     * 
     * @return Number of slices in state.
     */
    int getSlicesNr();

    /**
     * Getter function of end timestamp of back slice instances.
     *
     * @param index Index of slice instance to check.
     * 
     * @return End timestamp of back slice instance.
     */
    int getEndOfSlice(int index);

    /**
     * Getter function for slice instances.
     *
     * @param index Index of slice instance to check.
     * 
     * @return Slice instance at given index.
     */
    Slice *getSlice(int index);

    /**
     * Checks if timestamp is in slice.
     *
     * @param index Index of slice instance to check.
     * @param ts Timestamp to check.
     * 
     * @return   0 - in slice
     *         < 0 - event time < slice start
     *         > 0 - slice end <= event time.
     */
    int in(int index, int ts);

    /**
     * Check if slice instance with index was pre-aggregated.
     *
     * @param index Index of slice instance to check.
     * 
     * @return True if index was pre-aggregated and otherwise false.
     */
    bool preAggregate(int index);

    /**
     * Adds an empty slice instance with length step.
     *
     * @param step Length of slice instance.
     */
    void addSlice(int step);

    /**
     * Adds a value to a slice instance.
     *
     * @param index Index of slice instance.
     * @param value Value to add.
     * @param ts Timestamp to add.
     */
    void addValue(int index, double value, long ts);

    /**
     * Adds sum and count to pre-aggregated window instance.
     *
     * @param index Index of slice instance.
     * @param sum Sum to add.
     * @param count Count to add.
     */
    void addPreAggregate(int index, double sum, int count);

    /**
     * Remove slices before the given timestamp.
     *
     * @param ts Timestamp to remove slices with end before.
     */
    void removeSlices(long ts);

    /**
     * Removes empty slices from head.
     */
    void removeEmptyHead();

    /**
     * Clears data in instance.
     */
    ~State();

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();
};

#endif /* STATE_HPP */