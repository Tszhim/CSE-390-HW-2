#include <iostream>
#include <string>
#include "simulation.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        std::cerr << "Too few arguments. USAGE: ./robot <houseFilePath>" << std::endl;
        return 1;
    }
    std::string houseFilePath = argv[1];
    
    Simulation s;
    if(!s.readHouseFile(houseFilePath)) {
        std::cerr << "Unable to read house file due to I/O error or invalid input." << std::endl;
        return 1;
    }

    ConcreteAlgorithm a;
    s.setAlgorithm(a);
    if(!s.run()) {
        std::cerr << "Unable to write to output file due to I/O error." << std::endl;
        return 1;
    }

    return 0;
}