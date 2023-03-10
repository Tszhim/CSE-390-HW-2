#include "concrete_algorithm.h"

void ConcreteAlgorithm::setMaxSteps(const std::size_t maxSteps) {
    this->missionBudget = maxSteps;
}

void ConcreteAlgorithm::setBatteryMeter(const BatteryMeter& batteryMeter) {
    this->bm = &batteryMeter;
}

void ConcreteAlgorithm::setDirtSensor(const DirtSensor& dirtSensor) {
    this->ds = &dirtSensor;
}

void ConcreteAlgorithm::setWallsSensor(const WallsSensor& wallsSensor) {
    this->ws = &wallsSensor;
}

Step ConcreteAlgorithm::nextStep() {
    //TODO:
    return Step::North;
}

