#include "simulation.h"

bool Simulation::readHouseFile(const std::string houseFilePath) {
    return this->h.houseSetup(houseFilePath) && this->r.robotSetup(houseFilePath);
}

void Simulation::setAlgorithm(ConcreteAlgorithm algorithm) {
    algorithm.setMaxSteps(this->r.getMissionBudget());
    algorithm.setBatteryMeter(this->bm);
    algorithm.setDirtSensor(this->ds);
    algorithm.setWallsSensor(this->ws);
    this->algo = algorithm;
}

bool Simulation::run() {
    //TODO:
    return true;
}

bool Simulation::writeOutput() {
    int totalSteps = this->r.getStepCount();
    int dirtLeft = this->h.getRemainingDirt();
    int batteryLeft = this->r.getBatteryLeft();
    return this->fw.recordResults(totalSteps, dirtLeft, batteryLeft);
}
