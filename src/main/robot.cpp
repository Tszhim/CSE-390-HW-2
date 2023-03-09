#include "robot.h"

bool Robot::robotSetup(const std::string infilePath) {
    FileReader fr = FileReader(infilePath);
    
    /* I/O error or Line 3 invalid. */
    int temp = fr.readMaxBattery();
    if (temp == -1)
        return false;
    this->batteryCap = this->batteryLeft = temp;
    
    /* I/O error or Line 2 invalid. */
    temp = fr.readMaxSteps();
    if (temp == -1)
        return false;
    this->missionBudget = temp;

    return true;
}

int Robot::getMissionBudget() const {
    return this->missionBudget;
}
int Robot::getStepCount() const {
    return this->stepCount;
}

int Robot::getBatteryLeft() const {
    return this->batteryLeft;
}

Coordinate Robot::getLoc() const {
    return this->space;
}

bool Robot::onChargingDock() const {
    return this->space.x == 0 && this->space.y == 0;
}

bool Robot::budgetExceeded() const {
    return this->stepCount >= this->missionBudget;
}

void Robot::move(const Step s) {
    this->stepCount++;

    /* Sanity check. */
    if(this->batteryLeft <= 0 || budgetExceeded())
        return;

    if(s == Step::North)
        this->space.y += 1;
    if(s == Step::West)
        this->space.x -= 1;
    if(s == Step::South)
        this->space.y -= 1;
    if(s == Step::East)
        this->space.x += 1;
    // Do nothing otherwise.

    /* Recompute battery. */
    if(onChargingDock()) {
        int chargedBattery = this->batteryLeft + (this->batteryCap / 20);
        this->batteryLeft = chargedBattery <= this->batteryCap ? chargedBattery : this->batteryCap;
    }   
    else
        this->batteryLeft--; 
}
