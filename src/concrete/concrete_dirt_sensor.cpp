#include "concrete_dirt_sensor.h"

int ConcreteDirtSensor::dirtLevel() const {
    return this->dirt;
}

void ConcreteDirtSensor::setDirtLevel(int dirtLevel) {
    this->dirt = dirtLevel;
}