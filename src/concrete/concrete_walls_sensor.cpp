#include "concrete_walls_sensor.h"

bool ConcreteWallsSensor::isWall(Direction d) const {
    if(d == Direction::North) 
        return this->wallNorth;
    if(d == Direction::West)
        return this->wallWest;
    if(d == Direction::South)
        return this->wallSouth;
    if(d == Direction::East) 
        return this->wallEast;
}

void ConcreteWallsSensor::setWall(bool isWall, Direction d) {
    if(d == Direction::North) 
        this->wallNorth = isWall;
    if(d == Direction::West)
        this->wallWest = isWall;
    if(d == Direction::South)
        this->wallSouth = isWall;
    if(d == Direction::East) 
        this->wallEast = isWall;
}

