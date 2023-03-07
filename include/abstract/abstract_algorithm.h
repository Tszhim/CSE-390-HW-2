#ifndef ABSTRACT_ALGORITHM_H
#define ABSTRACT_ALGORITHM_H

#include <cstddef>
#include "../abstract/abstract_walls_sensor.h"
#include "../abstract/abstract_dirt_sensor.h"
#include "../abstract/abstract_battery_meter.h"
#include "../enums/step.h"

using std::size_t;

/*
	AbstractAlgorithm abstract class declaration, as per the requirements.
*/
class AbstractAlgorithm {
public:
	virtual ~AbstractAlgorithm() {}
	virtual void setMaxSteps(size_t maxSteps) = 0;
	virtual void setWallsSensor(const WallsSensor&) = 0;
	virtual void setDirtSensor(const DirtSensor&) = 0;
	virtual void setBatteryMeter(const BatteryMeter&) = 0;
	virtual Step nextStep() = 0;
};
    
#endif