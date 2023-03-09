#ifndef CONCRETE_ALGORITHM_H
#define CONCRETE_ALGORITHM_H

#include <unordered_map>
#include <unordered_set>
#include <stack>
#include "abstract_algorithm.h"
#include "coordinate.h"
#include "hash.h"

/**
 * @brief The concrete implementation of the abstract class "AbstractAlgorithm".
 * 
 * The "ConcreteAlgorithm" class provides an API to the simulator for initializing the sensors
 * it will request data from, and calculates the robot's traversal. 
 */
class ConcreteAlgorithm : public AbstractAlgorithm {
public:
    /**
     * @brief Constructs a "ConcreteAlgorithm" object.
     */
    ConcreteAlgorithm() {}

    /**
     * @brief Destroys a "ConcreteAlgorithm" object.
     */
    virtual ~ConcreteAlgorithm() {}
    
    /**
     * @brief Stores the number of steps allocated to the robot for the mission.
     * @param maxSteps The number of steps allocated.
     */
    void setMaxSteps(const std::size_t maxSteps);

    /**
     * @brief Stores the location of the BatteryMeter for future data requests.
     * @param batteryMeter A reference to the BatteryMeter.
     */
    void setBatteryMeter(const BatteryMeter& batteryMeter);

    /**
     * @brief Stores the location of the DirtSensor for future data requests.
     * @param dirtSensor A reference to the DirtSensor.
     */
    void setDirtSensor(const DirtSensor& dirtSensor);

    /**
     * @brief Stores the location of the WallsSensor for future data requests.
     * @param wallsSensor A reference to the WallsSensor.
     */
    void setWallsSensor(const WallsSensor& wallsSensor);

    /**
     * @brief Calculates the next step the robot should take based on pertinent data.
     * @return The next step the robot should take.
     */
    Step nextStep();

private:
    size_t missionBudget;  // The number of steps allocated to the robot for the mission.
    const BatteryMeter* bm;
    const DirtSensor* ds;
    const WallsSensor* ws;

    std::unordered_map<Coordinate, int, cHash> nodes;        // Algorithm view of all valid spaces in the house and their dirt levels.
    std::unordered_set<Coordinate, Coordinate, cHash> edges; // Algorithm view of all connections between valid spaces in the house.
    std::stack<Coordinate> branch_stack;                     // Maintains the list of branches yet to be traversed.
    std::stack<Coordinate> path_stack;                       // Maintains the current path the robot is on from charging dock.
};

#endif