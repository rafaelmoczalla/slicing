#ifndef BUCKETSLIDINGWINDOWSTATE_HPP
#define BUCKETSLIDINGWINDOWSTATE_HPP

#include <cstdlib>
#include <string>
#include <vector>

#include <slice.hpp>

class BucketSlidingWindowState {
  private:
    std::vector<Slice *> buckets;
    long lastWatermark;
    std::string key;

  public:
    /**
     * Creates bucket instance with key.
     *
     * @param key Key of bucket instance.
     */
    BucketSlidingWindowState(std::string key);

    /**
     * Getter function for key.
     *
     * @return Key of bucket instance.
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
     * Getter function to get number of events in a bucket instance.
     *
     * @return Number of events in bucket.
     */
    int getBucketCount();

    /**
     * Getter function for bucket instances.
     *
     * @param index Index of bucket instance to check.
     *
     * @return Bucket instance at given index.
     */
    Slice *getBucket(int index);

    /**
     * Adds an empty bucket instance.
     *
     * @param index Index of bucket instance in buffer.
     * @param start Start time of bucket.
     * @param end   End time of bucket.
     */
    void addBucket(int index, long start, long end);

    /**
     * Adds a value to a bucket instance.
     *
     * @param index Index of bucket instance.
     * @param value Value to add.
     * @param ts Timestamp to add.
     */
    void addValue(int index, double value, long ts);

    /**
     * Remove buckets before the given timestamp.
     *
     * @param ts Timestamp to remove buckets with end before.
     */
    void removeBucket(long ts);

    /**
     * Clears data in instance.
     */
    ~BucketSlidingWindowState();

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();
};

#endif /* BUCKETSLIDINGWINDOWSTATE_HPP */