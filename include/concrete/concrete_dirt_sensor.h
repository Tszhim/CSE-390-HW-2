#ifndef CONCRETE_DIRT_SENSOR_H
#define CONCRETE_DIRT_SENSOR_H

#include "../abstract/abstract_dirt_sensor.h"

/**
 * @brief The concrete implementation of the abstract class "DirtSensor".
 * 
 * The "ConcreteDirtSensor" class provides an API to the algorithm and allows
 * the dirt level of the robot's current location to be retrieved and updated.
 */
class ConcreteDirtSensor : public DirtSensor {
public:
    /**
     * @brief Constructs a "ConcreteDirtSensor" object.
     */
    ConcreteDirtSensor() {}

    /**
     * @brief Destroys the created "ConcreteDirtSensor" object.
     */
    virtual ~ConcreteDirtSensor() {}

    /**
     * @brief Gets the dirt level at the robot's current location.
     * @return The dirt level as an int.
     */
    int dirtLevel() const;

    /**
     * @brief Updates the dirt level at the robot's current location.
     * @param dirtLevel The dirt level as an int.
     */
    void setDirtLevel(int dirtLevel);

private:
    int dirtLevel; // The amount of dirt at the location of the robot.
};

#endif