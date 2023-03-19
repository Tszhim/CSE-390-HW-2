#ifndef CONCRETE_ALGORITHM_H
#define CONCRETE_ALGORITHM_H

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "abstract_algorithm.h"
#include "concrete_battery_meter.h"
#include "concrete_dirt_sensor.h"
#include "concrete_walls_sensor.h"
#include "coordinate.h"
#include "hash.h"
#include "node.h"


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
    ConcreteAlgorithm() : stepCount(0), mapFinished(false) {}

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

    int batteryLeft;
    int dirt;
    bool wallNorth, wallWest, wallSouth, wallEast;

    /* Maintained by algorithm. */
    int batteryCap;
    int stepCount;                                               // Maintains number of steps taken.
    Coordinate robotCoords;                                      // Maintains current robot position.
    bool mapFinished;                                            // Whether the house mapping is completed.

    std::unordered_map<Coordinate, std::shared_ptr<Node>, cHash> houseMap;

    void setup();
};

#endif