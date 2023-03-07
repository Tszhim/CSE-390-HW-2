#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include <utility> 
#include <string>
#include "../enums/direction.h"
#include "../enums/step.h"
#include "../utils/coordinate.h"

#define O_FILE "output.txt"

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
    void recordStep(Step s);
    
    /**
     * @brief Writes results to output file.
     * @return true on success, false if I/O error.
     */
    bool recordResults() const;

private:
    std::string steps; // Maintains the list of steps the robot has taken.
    
    /**
     * @brief Sets up output file for I/O. 
     * @return true on success, false if I/O error.
     */
    bool setupOfile() const;

    /**
     * @brief Writes the total number of steps the robot took on the mission to output file.
     * @return true on success, false if I/O error.
     */
    bool writeStepCount(const int total_steps) const; 

    /**
     * @brief Writes the remaining amount of dirt in the house to output file.
     * @return true on success, false if I/O error.
     */
    bool writeDirtLeft(const int dirt_left) const;

    /**
     * @brief Writes the final status of the robot (FINISHED/WORKING/DEAD).
     * @return true on success, false if I/O error.
     */
    bool writeRobotStatus(const int battery_left, const Coordinate loc) const;

    /**
     * @brief Writes the steps that the robot took on the mission.
     * @return true on success, false if I/O error.
     */
    bool writeSteps() const;
};

#endif