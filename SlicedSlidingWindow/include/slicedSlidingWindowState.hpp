#ifndef SLIDINGWINDOWSTATE_HPP
#define SLIDINGWINDOWSTATE_HPP

#include <cstdlib>
#include <string>
#include <vector>

#include <slice.hpp>

class SlicedSlidingWindowState {
  private:
    std::vector<Slice *> slices;
    long lastWatermark;
    std::string key;

  public:
    /**
     * Creates state instance with key.
     *
     * @param key Key of state instance.
     */
    SlicedSlidingWindowState(std::string key);

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
     * Getter function for slice instances.
     *
     * @param index Index of slice instance to check.
     *
     * @return Slice instance at given index.
     */
    Slice *getSlice(int index);

    /**
     * Adds an empty slice instance.
     *
     * @param index  Index of slice in buffer.
     * @param start  Start time of slice.
     * @param end    End time of slice.
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
     * Remove slices before the given timestamp.
     *
     * @param ts Timestamp to remove slices with end before.
     */
    void removeSlices(long ts);

    /**
     * Clears data in instance.
     */
    ~SlicedSlidingWindowState();

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();
};

#endif /* SLIDINGWINDOWSTATE_HPP */