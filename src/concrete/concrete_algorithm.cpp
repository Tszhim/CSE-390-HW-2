#include "concrete_algorithm.h"

void ConcreteAlgorithm::setMaxSteps(const std::size_t maxSteps) {
    this->missionBudget = maxSteps;
}

void ConcreteAlgorithm::setBatteryMeter(const BatteryMeter &batteryMeter) {
    this->bm = &batteryMeter;
}

void ConcreteAlgorithm::setDirtSensor(const DirtSensor &dirtSensor) {
    this->ds = &dirtSensor;
}

void ConcreteAlgorithm::setWallsSensor(const WallsSensor &wallsSensor) {
    this->ws = &wallsSensor;
}

Step ConcreteAlgorithm::nextStep() {
    //TODO:
    setup();

    /* Return finish when no more dirt cleanable with remaining mission budget and robot returned to charging dock. */
    if (this->stepCount >= this->missionBudget && this->robotCoords.x == 0 && this->robotCoords.y == 0)
        return Step::Finish;

    /* At this point, a move will be made. */
    this->stepCount++;

    

    return Step::North;
}

void ConcreteAlgorithm::setup() {
    /* Pull data from sensors. */   
    this->batteryLeft = dynamic_cast<const ConcreteBatteryMeter *>(this->bm)->getBatteryState();
    this->dirt = dynamic_cast<const ConcreteDirtSensor *>(this->ds)->dirtLevel();
    this->wallNorth = dynamic_cast<const ConcreteWallsSensor *>(this->ws)->isWall(Direction::North);
    this->wallWest = dynamic_cast<const ConcreteWallsSensor *>(this->ws)->isWall(Direction::West);
    this->wallSouth = dynamic_cast<const ConcreteWallsSensor *>(this->ws)->isWall(Direction::South);
    this->wallEast = dynamic_cast<const ConcreteWallsSensor *>(this->ws)->isWall(Direction::East);

    /* Initialize some class attributes on first run. */
    if (this->stepCount == 0) {
        this->batteryCap = this->batteryLeft;
        this->robotCoords = Coordinate(5, 0);
        std::shared_ptr<Node> dockPtr = std::make_shared<Node>(robotCoords);

        this->houseMap.insert(std::make_pair(robotCoords, dockPtr));
    }
}
