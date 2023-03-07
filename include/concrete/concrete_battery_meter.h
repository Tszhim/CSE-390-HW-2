#ifndef CONCRETE_BATTERY_METER_H
#define CONCRETE_BATTERY_METER_H

#include "../abstract/abstract_battery_meter.h"

/**
 * @brief The concrete implementation of the abstract class "BatteryMeter".
 * 
 * The "ConcreteBatteryMeter" class provides an API to the algorithm and allows
 * the robot's remaining battery state to be retrieved and updated.
 */
class ConcreteBatteryMeter : public BatteryMeter {
public:
    /**
     * @brief Constructs a "ConcreteBatteryMeter" object.
     */
    ConcreteBatteryMeter() {}

    /**
     * @brief Destroys the created "ConcreteBatteryMeter" object.
     */
    virtual ~ConcreteBatteryMeter() {}

    /**
     * @brief Gets the remaining battery of the robot.
     * @return The remaining battery as a size_t.
     */
    size_t getBatteryState() const;

    /**
     * @brief Updates the remaining battery of the robot.
     * @param batteryState The remaining battery as a size_t.
     */
    void setBatteryState(size_t batteryState);

private:
    size_t batteryState; // The remaining battery of the robot.
};

#endif