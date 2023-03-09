#include "file_reader.h"

int FileReader::readMaxSteps() const {
    std::ifstream f = std::ifstream(this->infilePath);
    if(f.fail()) 
        return -1;

    /* MaxSteps is on Line #2. */
    std::string str;
    for(int i = 0; i < 2; i++) {
        if(!getline(f, str)) 
            return -1;
    }
    
    return parseLine(str, "MaxSteps");
}

int FileReader::readMaxBattery() const {
    std::ifstream f = std::ifstream(this->infilePath);
    if(f.fail()) 
        return -1;

    /* MaxBattery is on Line #3. */
    std::string str;
    for(int i = 0; i < 3; i++) {
        if(!getline(f, str)) 
            return -1;
    }
    
    return parseLine(str, "MaxBattery");
}

int FileReader::readRowCount() const {
    std::ifstream f = std::ifstream(this->infilePath);
    if(f.fail()) 
        return -1;

    /* Rows is on Line #4. */
    std::string str;
    for(int i = 0; i < 4; i++) {
        if(!getline(f, str)) 
            return -1;
    }
    
    return parseLine(str, "Rows");
}

int FileReader::readColCount() const {
    std::ifstream f = std::ifstream(this->infilePath);
    if(f.fail()) 
        return -1;

    /* Cols is on Line #5. */
    std::string str;
    for(int i = 0; i < 5; i++) {
        if(!getline(f, str)) 
            return -1;
    }
    
    return parseLine(str, "Cols");
}

bool FileReader::readSpaces(std::unordered_set<Coordinate, cHash>& spaces, std::unordered_map<Coordinate, int, cHash>& dirtLevel) const {
    /* Don't read rows/cols past the bounds. */
    int row_bound = readRowCount();
    int col_bound = readColCount();
    if(row_bound == -1 || col_bound == -1)
        return false;

    /* Skip to Line #5. */
    std::ifstream f = std::ifstream(this->infilePath);
    if(f.fail()) 
        return -1;

    std::string str;
    for(int i = 0; i < 5; i++) {
        if(!getline(f, str)) 
            return -1;
    }
    
    /* House starts at line 6. Validate house and get charging dock location on first pass. */
    int dock_found = false;
    int row = 0, dock_row = 0, dock_col = 0;

    while(getline(f, str)) {
        /* Meet bound constraints. */
        if(row >= row_bound) 
            break;
        if(str.length() > col_bound) 
            str = str.substr(0, col_bound);

        /* Check for correct formatting. */
        if(str.find_first_not_of("W0123456789D ") != std::string::npos)
            return false;
        if(int temp = str.find('D'); temp != std::string::npos) {
            if(dock_found) 
                return false;
            dock_found = true;
            dock_row = row;
            dock_col = temp;
        }
        row++;
    }
    if(!dock_found)
        return false;

    /* Jump back to Line #5. */
    f.clear();
    f.seekg(0);
    for(int i = 0; i < 5; i++) {
        if(!getline(f, str)) 
            return -1;
    }

    /* Store relevant information on second pass. */
    row = 0;
    Coordinate space;
    int dirt = 0;

    while(row < row_bound) {
        /* Read next line if any, otherwise, wait for space padding. */
        if(!getline(f, str))
            str = "";

        /* Meet bound constraints. */
        if(str.length() > col_bound) 
            str = str.substr(0, col_bound);

        /* Pad out empty spaces with spaces. */
        while(str.length() < col_bound)
            str += " ";

        /* Store relevant information. */
        for(int i = 0; i < str.length(); i++) {
            if(char c = str.at(i); c != 'W') {
                space.x = i - dock_col;
                space.y = dock_row - row;
                dirt = (c == 'D' || c == ' ') ? 0 : int(c) - '0';

                spaces.insert(space);
                dirtLevel.insert({space, dirt});
            }
        }
        row++;
    }
    return true;
}   

int FileReader::parseLine(std::string line, std::string startsWith) const {
    int lineLen = line.length();
    int swLen = startsWith.length();

    /* Line does not start with the specified string or empty string after match. */
    if(lineLen <= swLen || line.substr(0, swLen) != startsWith)
        return -1;

    /* Check to see if the next few characters are "=", "= ", " =", or " = ". "*/
    line = line.substr(swLen);
    bool equalsFound = false;
    int valIdx;

    if(line.length() > 0 && line.compare(0, 1, "=") == 0) {
        equalsFound = true;
        valIdx = 1;
    }
    if(line.length() > 1 && line.compare(0, 2, "= ") == 0) {
        equalsFound = true;
        valIdx = 2;
    }
    if(line.length() > 1 && line.compare(0, 2, " =") == 0) {
        equalsFound = true;
        valIdx = 2;
    }
    if(line.length() > 2 && line.compare(0, 3, " = ") == 0) {
        equalsFound = true;
        valIdx = 3;
    }

    /* Did not match or empty string after match. */
    if(!equalsFound || valIdx == line.length())
        return -1;
    
    /* Value is not numeric. */
    line = line.substr(valIdx);
    if(line.find_first_not_of("0123456789") != std::string::npos)
        return -1;

    return std::stoi(line);
}