#ifndef ABSTRACT_BATTERY_METER_H
#define ABSTRACT_BATTERY_METER_H

#include <cstddef>

using std::size_t;

/* 
	BatteryMeter abstract class declaration, as per the requirements.
*/
class BatteryMeter {
public:
	virtual ~BatteryMeter() {}
	virtual size_t getBatteryState() const = 0;
};

#endif
