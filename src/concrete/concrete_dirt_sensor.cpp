#include "concrete_dirt_sensor.h"

ConcreteDirtSensor::ConcreteDirtSensor() {}

ConcreteDirtSensor::~ConcreteDirtSensor() {}

int ConcreteDirtSensor::dirtLevel() const {
    return this->dirt;
}

void ConcreteDirtSensor::setDirtLevel(int dirtLevel) {
    this->dirt = dirtLevel;
}