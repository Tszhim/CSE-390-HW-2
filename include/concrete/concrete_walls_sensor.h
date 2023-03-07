#ifndef CONCRETE_WALLS_SENSOR_H
#define CONCRETE_WALLS_SENSOR_H

#include "../abstract/abstract_walls_sensor.h"

/**
 * @brief The concrete implementation of the abstract class "WallsSensor".
 * 
 * The "ConcreteWallsSensor" class provides an API to the algorithm and allows
 * the presence of walls near the robot to be retrieved and updated.
 */
class ConcreteWallsSensor : public WallsSensor {
public:
    /**
     * @brief Constructs a "ConcreteWallsSensor" object.
     */
    ConcreteWallsSensor() {}

    /**
     * @brief Destroys the created "ConcreteWallsSensor" object.
     */
    virtual ~ConcreteWallsSensor() {}

    /**
     * @brief Checks if there is a wall in the specified direction from the robot.
     * @param d The specified direction to check.
     * @return Whether or not there is a wall. 
     */
    bool isWall(Direction d) const;

    /**
     * @brief Updates the presence of a wall in the specified direction from the robot.
     * @param isWall Whether or not there is a wall.
     * @param d The specified direction to update.
     */
    void setWall(bool isWall, Direction d);

private:
    bool wallNorth, wallWest, wallSouth, wallEast; // Whether there is a wall in each of the directions from the robot.
};

#endif