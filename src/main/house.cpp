#include "house.h"

House::House() {}

House::~House() {}

bool House::houseSetup(const std::string infilePath) {
    FileReader fr = FileReader(infilePath); 

    /* I/O error or Line 4/5/6+ invalid. */
    if(!fr.readSpaces(this->spaces, this->dirtLevel))
        return false;
    
    return true;
}

bool House::isValidSpace(const Coordinate space) const {
    return this->spaces.find(space) != spaces.end();
}

int House::getDirt(const Coordinate space) const {
    std::unordered_map<Coordinate, int>::const_iterator it = this->dirtLevel.find(space);

    /* If space exists. */
    if(this->dirtLevel.find(space) != this->dirtLevel.end()) 
    {
        return it->second;
    }
    return 0;
}

int House::getRemainingDirt() const {
    int sum = 0;
    std::unordered_map<Coordinate, int>::const_iterator it = this->dirtLevel.begin();

    /* Iterate through spaces. */
    for(it; it != this->dirtLevel.end(); it++) {
        sum += it->second;
    }
    return sum;
}

bool House::isHouseClean() const {   
    return getRemainingDirt() == 0;
}

void House::cleanSpace(const Coordinate space) {
    std::unordered_map<Coordinate, int>::iterator it = this->dirtLevel.find(space);

    /* If space exists and dirt level of space > 0. */
    if(it != this->dirtLevel.end() && it->second > 0) {
        it->second -= 1;
    }
}