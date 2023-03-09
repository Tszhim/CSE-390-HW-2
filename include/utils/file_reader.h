#ifndef FILE_READER_H
#define FILE_READER_H

#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include "coordinate.h"
#include "hash.h"

/**
 * @brief A class declaration for reading input information about the house and robot from file.
 * 
 * The "FileReader" class exposes functions to the house and robot for initialization.
 */
class FileReader {
public:
    /**
     * @brief Constructs a "FileReader" object.
     * @param infileName The input file to read from.
     */
    FileReader(std::string infilePath) {this->infilePath = infilePath;}

    /**
     * @brief Destroys the created "FileReader" object.
    */
    ~FileReader() {}

    /**
     * @brief Reads the number of allocated steps for the robot's mission from file.
     * @return The number of steps on success, -1 if invalid input or I/O error.
    */
    int readMaxSteps() const;

    /**
     * @brief Reads the battery capacity of the robot from file.
     * @return The battery capacity on success, -1 if invalid input or I/O error.
    */
    int readMaxBattery() const;

    /**
     * @brief Reads the number of rows the house will have from file.
     * @return The number of rows on success, -1 if invalid input or I/O error.
    */
    int readRowCount() const;

    /**
     * @brief Reads the number of columns the house will have from file.
     * @return The number of columns on success, -1 if invalid input or I/O error.
    */
    int readColCount() const;

    /**
     * @brief Reads the house structure from file and stores the data into provided data structure references.
     * @return true on success, false if invalid input or I/O error.
    */
    bool readSpaces(std::unordered_set<Coordinate, cHash>& spaces, std::unordered_map<Coordinate, int, cHash>& dirtLevel) const;

private:
    std::string infilePath; // The path to the input file.

    /**
     * @brief Parses out the value stored in Lines 2-5 if possible.
     * @return The numeric value stored in the line on success, -1 if invalid input.
    */
    int parseLine(std::string line, std::string startsWith) const;
};

#endif