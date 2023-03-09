#include "file_writer.h"

void FileWriter::recordStep(const Step s) {
    if(s == Step::North)
        this->steps += "N";
    if(s == Step::West)
        this->steps += "W";
    if(s == Step::South)
        this->steps += "S";
    if(s == Step::East) 
        this->steps += "E";
    if(s == Step::Stay) 
        this->steps += "s";
    if(s == Step::Finish)
        this->steps += "F";
}

bool FileWriter::recordResults(const int totalSteps, const int dirtLeft, const int batteryLeft) const {
    return setupOfile() && writeStepCount(totalSteps) && writeDirtLeft(dirtLeft) && writeRobotStatus(batteryLeft) && writeSteps();
}

bool FileWriter::setupOfile() const {
    /* Output file already exists, attempt to remove. */
    std::ifstream in = std::ifstream(OFILE);
    if(in.good() && remove(OFILE) != 0) 
        return false;

    /* Create output file.*/
    std::ofstream out = std::ofstream(OFILE, std::ios::out);
    return out.good();
}

bool FileWriter::writeStepCount(const int totalSteps) const {
    std::ofstream f = std::ofstream(OFILE);
    if(f.fail())
        return false;

    f << "NumSteps = " << totalSteps << std::endl;
    return true;
}

bool FileWriter::writeDirtLeft(const int dirtLeft) const {
    std::ofstream f = std::ofstream(OFILE);
    if(f.fail())
        return false;

    f << "DirtLeft = " << dirtLeft << std::endl;
    return true;
}

bool FileWriter::writeRobotStatus(const int batteryLeft) const {
    std::ofstream f = std::ofstream(OFILE);
    if(f.fail())
        return false;

    /* Finished if algorithm reported, otherwise, working if battery > 0 and dead if battery <= 0. */
    if(this->steps.find('F') != std::string::npos)
        f << "Status = " << "FINISHED";
    else if(batteryLeft > 0) 
        f << "Status = " << "WORKING";
    else
        f << "Status = " << "DEAD";
    
    return true;
}

bool FileWriter::writeSteps() const {
    std::ofstream f = std::ofstream(OFILE);
    if(f.fail())
        return false;
    
    f << "Steps:" << std::endl << this->steps;
    return true;
}