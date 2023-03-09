#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include <fstream>
#include <string>
#include "direction.h"
#include "step.h"
#include "coordinate.h"

#define OFILE "output.txt"

/**
 * @brief A class declaration for writing mission results to file.
 * 
 * The "FileWriter" class exposes functions to the simulator for recording mission results.
 */
class FileWriter {
public:
    /**
     * @brief Constructs a "FileWriter" object.
     */
    FileWriter() {}

    /**
     * @brief Destroys the created "FileWriter" object.
    */
    ~FileWriter() {}

    /**
     * @brief Records step for result output.
     * @param s The step the robot made.
     */
    void recordStep(const Step s);
    
    /**
     * @brief Writes results to output file.
     * @param totalSteps The number of steps the robot took throughout the mission.
     * @param dirtLeft The amount of remaining uncleaned dirt in the house at the end of the mission.
     * @param batteryLeft The amount of battery the robot has left at the end of the mission.
     * @param loc The location of the robot at the end of the mission.
     * @return true on success, false if I/O error.
     */
    bool recordResults(const int totalSteps, const int dirtLeft, const int batteryLeft) const;

private:
    std::string steps; // Maintains the list of steps the robot has taken.
    
    /**
     * @brief Sets up output file for I/O. 
     * @return true on success, false if I/O error.
     */
    bool setupOfile() const;

    /**
     * @brief Writes the total number of steps the robot took on the mission to output file.
     * @param totalSteps The number of steps the robot took throughout the mission.
     * @return true on success, false if I/O error.
     */
    bool writeStepCount(const int totalSteps) const; 

    /**
     * @brief Writes the remaining amount of dirt in the house to output file.
     * @param dirtLeft The amount of remaining uncleaned dirt in the house at the end of the mission.
     * @return true on success, false if I/O error.
     */
    bool writeDirtLeft(const int dirtLeft) const;

    /**
     * @brief Writes the final status of the robot (FINISHED/WORKING/DEAD).
     * @param batteryLeft The amount of battery the robot has left at the end of the mission.
     * @param loc The location of the robot at the end of the mission.
     * @return true on success, false if I/O error.
     */
    bool writeRobotStatus(const int batteryLeft) const;

    /**
     * @brief Writes the steps that the robot took on the mission.
     * @return true on success, false if I/O error.
     */
    bool writeSteps() const;
};

#endif