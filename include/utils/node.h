#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "coordinate.h"


class Node {
public:
    Node(Coordinate coords) : coords(coords), euclidianDist(coords.x * coords.x + coords.y * coords.y), 
        neighbors{}, dirtLevel(0), visited(false) {}

    virtual ~Node() {}

    /* Setter methods */
    inline void addNeighbor(std::shared_ptr<Node> neighbor) { this->neighbors.push_back(neighbor); }
    inline void setDirtLevel(int dirtLevel) { this->dirtLevel = dirtLevel; }
    inline void decrementDirtLevel() { this->dirtLevel--; }
    inline void setVisited() { this->visited = true; }

    /* Getter methods */
    inline Coordinate getCoords() const { return this->coords; }
    inline double getEuclidianDist() const { return this->euclidianDist; }
    inline std::vector<std::shared_ptr<Node>> getNeighbors() const { return this->neighbors; }
    inline int getDirtLevel() const { return this->dirtLevel; }
    inline bool isVisited() const { return this->visited; }

    friend std::ostream& operator<<(std::ostream& os, const Node& node) {
        os << "Coords: {" << node.getCoords().x << ", " << node.getCoords().y << 
            "} | EuclidianDist: " << node.getEuclidianDist() << " | DirtLevel: " << node.getDirtLevel() <<
            " | Visited: " << node.isVisited();
        return os;
    }

private:
    Coordinate coords;
    double euclidianDist;
    std::vector<std::shared_ptr<Node>> neighbors;
    int dirtLevel;
    bool visited;
};

#endif