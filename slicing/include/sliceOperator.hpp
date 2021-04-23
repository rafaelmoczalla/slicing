#ifndef SLICEOPERATOR_HPP
#define SLICEOPERATOR_HPP

#include <cstdlib>
#include <string>
#include <vector>

#include <watermark.hpp>
#include <state.hpp>

class SliceOperator {
  private:
    State *window;

    long start;
    long size;
    long step;
    long doubleStep;
    int vDeltaIdx;

  public:
    /**
     * Creates operator instance.
     * 
     * @param start Start timestamp of operator to process.
     */
    SliceOperator(int start);

    /**
     * Creates operator instance.
     * 
     * @param start Start timestamp of operator to process.
     * @param size Window length of punctuation windows.
     * @param step Slice length to slice input stream.
     */
    SliceOperator(int start, int size, int step);

    /**
     * Processes a watermark instance from input pipelines.
     * 
     * @param watermark Watermark to process next.
     */
    void processWatermark(Watermark *watermark);

    /**
     * Processes an event instance from input pipelines.
     * 
     * @param event Event to process next.
     */
    void processElement(Event *event);

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();
};

#endif /* SLICEOPERATOR_HPP */