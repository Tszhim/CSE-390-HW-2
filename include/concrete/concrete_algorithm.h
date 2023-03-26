#ifndef CONCRETE_ALGORITHM_H
#define CONCRETE_ALGORITHM_H

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <memory>
#include <limits>
#include "abstract_algorithm.h"
#include "concrete_battery_meter.h"
#include "concrete_dirt_sensor.h"
#include "concrete_walls_sensor.h"
#include "coordinate.h"
#include "hash.h"
#include "node.h"


/**
 * @brief The concrete implementation of the abstract class "AbstractAlgorithm".
 * 
 * The "ConcreteAlgorithm" class provides an API to the simulator for initializing the sensors
 * it will request data from, and calculates the robot's traversal. 
 */
class ConcreteAlgorithm : public AbstractAlgorithm {
public:
    /**
     * @brief Constructs a "ConcreteAlgorithm" object.
     */
    ConcreteAlgorithm() : stepCount(0), robotCoords(Coordinate(0, 0)), distFromDock(0) {}

    /**
     * @brief Destroys a "ConcreteAlgorithm" object.
     */
    virtual ~ConcreteAlgorithm() {}
    
    /**
     * @brief Stores the number of steps allocated to the robot for the mission.
     * @param maxSteps The number of steps allocated.
     */
    void setMaxSteps(const std::size_t maxSteps);

    /**
     * @brief Stores the location of the BatteryMeter for future data requests.
     * @param batteryMeter A reference to the BatteryMeter.
     */
    void setBatteryMeter(const BatteryMeter& batteryMeter);

    /**
     * @brief Stores the location of the DirtSensor for future data requests.
     * @param dirtSensor A reference to the DirtSensor.
     */
    void setDirtSensor(const DirtSensor& dirtSensor);

    /**
     * @brief Stores the location of the WallsSensor for future data requests.
     * @param wallsSensor A reference to the WallsSensor.
     */
    void setWallsSensor(const WallsSensor& wallsSensor);
    
    /**
     * @brief Calculates the next step the robot should take based on pertinent data.
     * @return The next step the robot should take.
     */
    Step nextStep();

private:
    size_t missionBudget;                                                         // The number of steps allocated to the robot for the mission.
    const BatteryMeter* bm;
    const DirtSensor* ds;
    const WallsSensor* ws;

    int batteryLeft;
    int dirt;
    bool wallNorth, wallWest, wallSouth, wallEast;

    /* Maintained by algorithm. */
    int batteryCap;
    int stepCount;                                                                // Maintains number of steps taken.
    Coordinate robotCoords;                                                       // Maintains current robot position.
    int distFromDock;                                                             // An estimation of how far the robot is from the dock.

    std::unordered_map<Coordinate, std::shared_ptr<Node>, cHash> houseMap;        // Maps coordinates to a node object.
    std::unordered_set<std::shared_ptr<Node>, nHash> unvisitedNodes;              // Nodes to explore next.
    std::stack<std::shared_ptr<Node>> pathToDock;                                 // Empty when not in use, otherwise the robot must follow this path under any circumstance.
    std::stack<std::shared_ptr<Node>> pathToNode;                                 // Empty when not in use, otherwise the robot will follow this path if pathToDock is not set. 

    void setup();
    bool onChargingDock();
    void markSurroundings();
    void mapNeighbor(Coordinate coords);
    std::shared_ptr<Node> getClosestAdjacentNode();
    void setClosestNonAdjacentNodePath();
    Step getDirectionToNode(std::shared_ptr<Node> node);
    std::stack<std::shared_ptr<Node>> findShortestPath(std::shared_ptr<Node> start, std::shared_ptr<Node> end);
    Step returnToDock();
    Step moveToNode();
};

#endif