#ifndef ABSTRACT_DIRT_SENSOR_H
#define ABSTRACT_DIRT_SENSOR_H

/* 
	DirtSensor abstract class declaration, as per the requirements.
*/
class DirtSensor {
public:
	virtual ~DirtSensor() {}
	virtual int dirtLevel() const = 0;
};

#endif
