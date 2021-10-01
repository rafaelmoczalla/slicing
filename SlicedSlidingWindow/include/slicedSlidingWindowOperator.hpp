#ifndef SLICEDSLIDINGWINDOWOPERATOR_HPP
#define SLICEDSLIDINGWINDOWOPERATOR_HPP

#include <cstdlib>
#include <string>
#include <vector>

#include <watermark.hpp>
#include <slicedSlidingWindowState.hpp>

class SlicedSlidingWindowOperator {
  private:
    SlicedSlidingWindowState *window;

    long start;
    long size;
    long step;

  public:
    /**
     * Creates operator instance.
     *
     * @param start Start timestamp of operator to process.
     */
    SlicedSlidingWindowOperator(long start);

    /**
     * Creates operator instance.
     *
     * @param start Start timestamp of operator to process.
     * @param size Window length of punctuation windows.
     * @param step Slice length to slice input stream.
     */
    SlicedSlidingWindowOperator(long start, long size, long step);

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
    ~SlicedSlidingWindowOperator();

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();
};

#endif /* SLICEDSLIDINGWINDOWOPERATOR_HPP */