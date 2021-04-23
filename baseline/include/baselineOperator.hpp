#ifndef BASELINEOPERATOR_HPP
#define BASELINEOPERATOR_HPP

#include <cstdlib>
#include <string>
#include <vector>

#include <watermark.hpp>
#include <bucket.hpp>

class BaselineOperator {
  private:
    Bucket *bucket;

    long start;
    long size;

  public:
    /**
     * Creates operator instance.
     * 
     * @param start Start timestamp of operator to process.
     */
    BaselineOperator(int start);

    /**
     * Creates operator instance.
     * 
     * @param start Start timestamp of operator to process.
     * @param size Window length of punctuation windows.
     */
    BaselineOperator(int start, int size);

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

#endif /* BASELINEOPERATOR_HPP */