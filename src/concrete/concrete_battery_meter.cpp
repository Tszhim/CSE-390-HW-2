#include "concrete_battery_meter.h"

size_t ConcreteBatteryMeter::getBatteryState() const {
    return batteryState;
}

void ConcreteBatteryMeter::setBatteryState(size_t batteryState) {
    this->batteryState = batteryState;
}