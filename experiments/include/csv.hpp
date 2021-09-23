#ifndef CSV_HPP
#define CSV_HPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "filesystem.hpp"

class Csv {
  private:
    std::ofstream csv;
    std::filesystem::path path;
    std::string delim;
    std::string nl;
    bool fol;
  public:
    /**
     * Creates a csv instance and file.
     */
    Csv();

    /**
     * Creates a csv instance and file.
     */
    Csv(std::string dir, std::string name);

    /**
     * Creates a csv instance and file.
     */
    Csv(std::string dir, std::string name, std::vector<std::string> head);

    /**
     * Destroys a csv instance properly and closes file.
     */
    ~Csv();

    /**
     * Known operator to directly stream string into csv files.
     *
     * @return Csv file representation of an instance.
     */
    Csv& operator<<(const std::string value);

    /**
     * Known operator to directly stream char into csv files.
     *
     * @return Csv file representation of an instance.
     */
    Csv& operator<<(char value);

    /**
     * Known operator to directly stream char array into csv files.
     *
     * @return Csv file representation of an instance.
     */
    Csv& operator<<(const char *value);

    /**
     * Known operator to directly stream bool into csv files.
     *
     * @return Csv file representation of an instance.
     */
    Csv& operator<<(bool value);

    /**
     * Known operator to directly stream int into csv files.
     *
     * @return Csv file representation of an instance.
     */
    Csv& operator<<(int value);

    /**
     * Known operator to directly stream double into csv files.
     *
     * @return Csv file representation of an instance.
     */
    Csv& operator<<(double value);

    /**
     * Known operator to directly stream functions like std::endl into csv files.
     *
     * @return Csv file representation of an instance.
     */
    Csv& operator<<(std::ostream &(*fct)(std::ostream &));

    /**
     * Debug function to get a string representation of an instance.
     *
     * @return Text representation of an instance.
     */
    std::string to_string();

    /**
     * Debug function to get a stream representation of an instance.
     *
     * @return Stream representation of an instance.
     */
    friend std::ostream& operator<<(std::ostream& os, const Csv& pl);
};

extern std::ostream& operator<<(std::ostream& os, const Csv& pl);

#endif /* CSV_HPP */