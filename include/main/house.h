#ifndef HOUSE_H
#define HOUSE_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include "coordinate.h"
#include "file_reader.h"
#include "hash.h"

/**
 * @brief A class declaration to represent the internal structure of a house.
 * 
 * The "House" class provides an API to the simulator for retrieving house information and state, 
 * and performs the relevant housekeeping whenever any state changes occur.
 */
class House {
public:
    /**
     * @brief Constructs a "House" object.
     */
    House() {}

    /**
     * @brief Destroys a "House" object.
     */
    ~House() {}

    /**
     * @brief Reads from input file to store information about the internal structure of the house.
     * @param infile_name The name of the input file.
     * @return true if success, false if I/O error.
     */
    bool houseSetup(const std::string infilePath);

     /**
     * @brief Checks if the specified space is valid within the house (i.e. not a wall).
     * @param space The specified space.
     * @return true if the space is valid, otherwise false.
     */
    bool isValidSpace(const Coordinate space) const;

    /**
     * @brief Checks for the amount of dirt at the specified space.
     * @param space The specified space.
     * @return The amount of dirt.
     */
    int getDirt(const Coordinate space) const;

    /**
     * @brief Checks for the amount of remaining dirt throughout the entire house.
     * @return The amount of remaining dirt.
     */
    int getRemainingDirt() const;

    /**
     * @brief Checks if the house is entirely clean (i.e. no dirt).
     * @return true if house is clean, otherwise false.
     */
    bool isHouseClean() const;

    /**
     * @brief Cleans the specified space within the house. 
     * @param space The specified space.
     */
    void cleanSpace(const Coordinate space);

private:
    std::unordered_set<Coordinate, cHash> spaces;          /* A unordered set of valid spaces that the robot can reach. Spaces are relative to the charging dock (origin). */
    std::unordered_map<Coordinate, int, cHash> dirtLevel;  /* An unordered map of valid space : dirt level. */
};

#endif