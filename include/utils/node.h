#ifndef NODE_H
#define NODE_H

#include <vector>
#include "coordinate.h"


class Node {
public:
    Node(Coordinate coords) : coords(coords), euclidianDist(coords.x * coords.x + coords.y * coords.y), 
        neighbors{}, dirtLevel(0), visited(false) {}

    virtual ~Node() {}

    /* Setter methods */
    inline void addNeighbor(Node* neighbor) { this->neighbors.push_back(neighbor); }
    inline void setDirtLevel(int dirtLevel) { this->dirtLevel = dirtLevel; }
    inline void setVisited() { this->visited = true; }

    /* Getter methods */
    inline Coordinate getCoords() { return this->coords; }
    inline double getEuclidianDist() { return this->euclidianDist; }
    inline std::vector<Node*> getNeighbors() { return this->neighbors; }
    inline int getDirtLevel() { return this->dirtLevel; }
    inline bool isVisited() { return this->visited; }

private:
    Coordinate coords;
    double euclidianDist;
    std::vector<Node*> neighbors;
    int dirtLevel;
    bool visited;
};

#endif