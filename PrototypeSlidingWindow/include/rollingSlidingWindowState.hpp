#ifndef ROLLINGSLIDINGWINDOWSTATE_HPP
#define ROLLINGSLIDINGWINDOWSTATE_HPP

#include <cstdlib>
#include <string>
#include <vector>

#include <slice.hpp>

class RollingSlidingWindowState {
  private:
    std::vector<Slice *> slices;
    long lastWatermark;
    double winSum;
    int winCount;
    std::string key;

  public:
    /**
     * Creates state instance with key.
     *
     * @param key Key of state instance.
     */
    RollingSlidingWindowState(std::string key);

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
    long getLastWatermark();

    /**
     * Setter function to store last watermark which was processed.
     *
     * @param lw Watermark to store.
     */
    void updateLastWatermark(long lw);

    /**
     * Getter function to get number of slices.
     *
     * @return Number of slices in state.
     */
    int getSliceCount();

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
     * @return RollingSlice instance at given index.
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
    int in(int index, long ts);

    /**
     * Adds an empty slice instance.
     *
     * @param index Index of slice instance in buffer.
     * @param start Start time of slice instance.
     * @param end   End time of slice instance.
     */
    void addSlice(int index, long start, long end);

    /**
     * Adds a value to a slice instance.
     *
     * @param index Index of slice instance.
     * @param value Value to add.
     * @param ts Timestamp to add.
     */
    void addValue(int index, double value, long ts);

    /**
     * Adds sum and count to pre-aggregated rolling window.
     *
     * @param sum Sum to add.
     * @param count Count to add.
     */
    void addToWindow(double sum, int count);

    /**
     * Removes sum and count from pre-aggregated rolling window.
     *
     * @param sum Sum to remove.
     * @param count Count to remove.
     */
    void removeFromWindow(double sum, int count);

    /**
     * Getter function for aggregate of the rolling window.
     *
     * @return Aggregated average value of window.
     */
    double getWindowAvg();

    /**
     * Remove slices before the given timestamp.
     *
     * @param ts Timestamp to remove slices with end before.
     */
    void removeSlices(long ts);

    /**
     * Clears data in instance.
     */
    ~RollingSlidingWindowState();

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();
};

#endif /* ROLLINGSLIDINGWINDOWSTATE_HPP */