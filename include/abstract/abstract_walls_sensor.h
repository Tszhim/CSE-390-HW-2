#ifndef ABSTRACT_WALLS_SENSOR_H
#define ABSTRACT_WALLS_SENSOR_H

#include "../enums/direction.h"

/* 
	WallsSensor abstract class declaration, as per the requirements.
*/
class WallsSensor {
public:
	virtual ~WallsSensor() {}
	virtual bool isWall(Direction d) const = 0;
};

#endif
