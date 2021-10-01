#ifndef BUCKETSLIDINGWINDOWOPERATOR_HPP
#define BUCKETSLIDINGWINDOWOPERATOR_HPP

#include <cstdlib>
#include <string>
#include <vector>

#include <watermark.hpp>
#include <bucketSlidingWindowState.hpp>

class BucketSlidingWindowOperator {
  private:
    BucketSlidingWindowState *window;

    long start;
    long step;
    long size;

  public:
    /**
     * Creates operator instance.
     * 
     * @param start Start timestamp of operator to process.
     */
    BucketSlidingWindowOperator(long start);

    /**
     * Creates operator instance.
     * 
     * @param start Start timestamp of operator to process.
     * @param size Window length of a slide.
     * @param size Delta between two slides.
     */
    BucketSlidingWindowOperator(long start, long size, long step);

    /**
     * Processes a watermark instance from input pipelines.
     * 
     * @param watermark Watermark to process next.
     */
    std::vector<Element *> processWatermark(Watermark *watermark);

    /**
     * Processes an event instance from input pipelines.
     * 
     * @param event Event to process next.
     */
    void processElement(Event *event);

    /**
     * Clears data in instance.
     */
    ~BucketSlidingWindowOperator();

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();
};

#endif /* BUCKETSLIDINGWINDOWOPERATOR_HPP */