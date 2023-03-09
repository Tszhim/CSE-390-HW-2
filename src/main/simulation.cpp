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
    /* Iterate until maxSteps is reached. */
    while(!this->r.budgetExceeded()) {
        Coordinate currLoc = this->r.getLoc();
        bool northWall = this->h.isValidSpace(Coordinate(currLoc.x, currLoc.y + 1));
        bool westWall = this->h.isValidSpace(Coordinate(currLoc.x - 1, currLoc.y));
        bool southWall = this->h.isValidSpace(Coordinate(currLoc.x, currLoc.y - 1));
        bool eastWall = this->h.isValidSpace(Coordinate(currLoc.x + 1, currLoc.y));

        /* Update sensors. */
        this->bm.setBatteryState(this->r.getBatteryLeft());
        this->ds.setDirtLevel(this->h.getDirt(currLoc));
        this->ws.setWall(northWall, Direction::North);
        this->ws.setWall(westWall, Direction::West);
        this->ws.setWall(southWall, Direction::South);
        this->ws.setWall(eastWall, Direction::East);

        /* Get next algorithm move. */
        Step nextStep = this->algo.nextStep();
        this->fw.recordStep(nextStep);

        /* Exit early if finished. */
        if(nextStep == Step::Finish)
            break;

        this->r.move(nextStep);

        /* Clean spot if stayed. */
        if(nextStep == Step::Stay)
            this->h.cleanSpace(this->r.getLoc());
    }
    return writeOutput();
}

bool Simulation::writeOutput() {
    int totalSteps = this->r.getStepCount();
    int dirtLeft = this->h.getRemainingDirt();
    int batteryLeft = this->r.getBatteryLeft();
    return this->fw.recordResults(totalSteps, dirtLeft, batteryLeft);
}
