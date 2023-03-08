#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include "concrete_algorithm.h"
#include "concrete_battery_meter.h"
#include "concrete_dirt_sensor.h"
#include "concrete_walls_sensor.h"
#include "house.h"
#include "robot.h"
#include "file_writer.h"

/**
 * @brief A class declaration for simulating the robot's mission.
 * 
 * The "Simulation" class provides an abstraction of all the algorithmic and state-maintaining functionality
 * of the program, such that main() can easily setup the control flow.
 */
class Simulation {
public:
    /**
     * @brief Constructs a "Simulation" object.
     */
    Simulation() {}

    /**
     * @brief Destroys a "Simulation" object.
     */
    ~Simulation() {}

    /**
     * @brief Initializes the house and robot objects to prepare for simulation start.
     * @param houseFilePath The location of the input file.
     * @return true if success, false if I/O error.
     */
    bool readHouseFile(const std::string houseFilePath);

    /**
     * @brief Initializes the algorithm to prepare for simulation start.
     * @param algorithm The algorithm object.
     */
    void setAlgorithm(ConcreteAlgorithm algorithm);

    /**
     * @brief Begin simulation of the robot's mission.
     * @return true if success, false if any type of unrecoverable error occurs.
     */
    bool run();

    /**
     * @brief Log the results of the mission to file. 
     * @return true if success, false if I/O error.
     */
    bool writeOutput();

private:
    House h;
    Robot r;
    ConcreteAlgorithm algo;

    ConcreteBatteryMeter bm;
    ConcreteDirtSensor ds;
    ConcreteWallsSensor ws;

    FileWriter fw;
};

#endif