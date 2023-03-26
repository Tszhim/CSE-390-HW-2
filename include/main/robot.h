#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include "step.h"
#include "coordinate.h"
#include "file_reader.h"

/**
 * @brief A class declaration to represent the state of the cleaning robot.
 * 
 * The "Robot class provides an API to the simulator for retrieving robot information and state,
 * and performs the relevant housekeeping whenever any state changes occur.
 */
class Robot {
public:
    /**
     * @brief Constructs a "Robot" object.
     */
    Robot() : stepCount(0) {}

    /**
     * @brief Destroys a "Robot" object.
     */
    ~Robot() {}

    /**
     * @brief Reads from input file to store information about the robot.
     * @param infile_name The name of the input file.
     * @return true if success, false if I/O error.
     */
    bool robotSetup(const std::string infilePath);

    /**
     * @brief Checks for the number of steps allocated to the robot for the mission.
     * @return The number of allocated steps.
    */
    int getMissionBudget() const;
    
    /** 
     * @brief Checks for the total number of steps the robot has taken.
     * @return The number of steps.
     */
    int getStepCount() const;

    /**
     * @brief Checks for the amount of remaining battery the robot has left.
     * @return The amount of battery.
     */
    int getBatteryLeft() const;

    /**
     * @brief Checks for the current location of the robot.
     * @return The location of the robot.
     */
    Coordinate getLoc() const;

    /**
     * @brief Checks if the robot is currently on the charging dock.
     * @return true if the robot is on the charging dock, otherwise false.
     */
    bool onChargingDock() const;

    /**
     * @brief Checks if the robot has taken enough steps to meet or exceed the budget.
     * @return true if the robot is on the charging dock, otherwise false.
     */
    bool budgetExceeded() const;

    /**
     * @brief Moves the robot in the specified manner. 
     * @param s The next step the robot should take. 
     */
    void move(const Step s);

private:
    int batteryCap;      // The battery capacity of the robot.
    int missionBudget;   // The number of steps allocated to the robot for the mission.

    int stepCount;       // The total number of steps made by the robot.
    int batteryLeft;     // The remaining amount of battery left in the robot.
    Coordinate space;    // The current location of the robot.
};

#endif