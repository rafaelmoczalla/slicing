#ifndef BUCKET_HPP
#define BUCKET_HPP

#include <cstdlib>
#include <string>
#include <vector>

#include <event.hpp>

class Bucket {
  private:
    std::vector<Event *> events;
    int lastWatermark;
    int checkpoint;
    std::string key;

  public:
    /**
     * Creates bucket instance with key.
     * 
     * @param key Key of bucket instance.
     */
    Bucket(std::string key);

    /**
     * Getter function of events in a bucket instance.
     *
     * @return Events in a bucket instance.
     */
    std::vector<Event *> getEvents();

    /**
     * Getter function for checkpoint of bucket instance.
     * 
     * @return Checkpoint of bucket instance.
     */
    int getCheckpoint();

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
    int getLastWatermark();

    /**
     * Setter function to store last watermark which was processed.
     * 
     * @param lw Watermark to store.
     */
    void updateLastWatermark(int lw);

    /**
     * Getter function to get number of events in a bucket instance.
     * 
     * @return Number of events in bucket.
     */
    int getEventsNr();

    /**
     * Getter function for event instances.
     *
     * @param index Index of event instance to check.
     * 
     * @return Event instance at given index.
     */
    Event *getEvent(int index);

    /**
     * Adds a value to a slice instance.
     *
     * @param value Value to add.
     * @param ts Timestamp to add.
     */
    void addValue(double value, long ts);

    /**
     * Remove event instance at given index.
     *
     * @param index Index of event instance to remove.
     */
    void removeEvent(int index);

    /**
     * Clears data in instance.
     */
    void clear();

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();
};

#endif /* BUCKET_HPP */