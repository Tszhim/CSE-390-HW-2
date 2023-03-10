#ifndef CONCRETE_ALGORITHM_H
#define CONCRETE_ALGORITHM_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <deque>
#include "abstract_algorithm.h"
#include "concrete_battery_meter.h"
#include "concrete_dirt_sensor.h"
#include "concrete_walls_sensor.h"
#include "coordinate.h"
#include "hash.h"

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
    ConcreteAlgorithm() : stepCount(0), branchLevel(0), nextSpaceBacktrack(false), nextSpacesSet(false), mapFinished(false) {}

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
    /* Set before algorithm runs. */
    size_t missionBudget;  
    const BatteryMeter* bm;
    const DirtSensor* ds;
    const WallsSensor* ws;

    /* Holds sensor data when pulled. */
    int batteryLeft;
    int dirt;
    bool wallNorth, wallWest, wallSouth, wallEast;
    
    /* Maintained by algorithm. */
    int batteryCap;
    int stepCount;                                               // Maintains number of steps taken.
    Coordinate space;                                            // Maintains current robot position.
    std::queue<Coordinate> nextSpaces;                           // Maintains the cluster of coordinates the robot should move to next for a particular branch.
    bool nextSpaceBacktrack;                                     // For each cluster of coordinates, the robot should backtrack once after visiting each one.
    bool nextSpacesSet;                                          // Whether the next set of spaces for branching is determined.
    bool mapFinished;                                            // Whether the house mapping is completed.
    int branchLevel;                                             // BFS level.

    std::unordered_map<Coordinate, int, cHash> nodes;                     // Algorithm view of all valid spaces in the house and their dirt levels.
    std::unordered_set<Coordinate, cHash> visitedNodes;                   // Maintains the list of nodes already visited.
    std::unordered_map<Coordinate, std::deque<Coordinate>, cHash> paths;  // Maintains the path to reach a particular node.
    std::queue<std::queue<Coordinate>> branchQueue;                       // Maintains the list of branches yet to be traversed for the current level.
    std::queue<std::queue<Coordinate>> nextQueue;                         // Maintains the list of branches yet to be traversed for the next level.
    std::deque<Coordinate> pathStack;                                     // Maintains the current path the robot is on from charging dock.
    
    /**
     * @brief Prepares class attributes before next algorithm iteration.
    */
    void setup();

    /**
     * @brief Makes the next step in order to get from the current coordinate to the next.
     * @param next The next coordinate.
     * @return The next step made.
    */
    Step makeStepToNext(Coordinate next);
    /**
     * @brief Move back to charging dock using path stack.
     * @return The next step to make to backtrack.
     */
    Step backtrack();

    /**
     * @brief Move to the next space determined by BFS traversal.
     * @return The next step to make to reach next space.
     */
    Step moveTo();

     /**
     * @brief Continue phase #1 of the algorithm: mapping the entire house. 
     * @return The next step to make to map the house. 
     */
    Step phase1();

    /**
     * @brief Continue phase #2 of the algorithm: cleaning the mapped house.
     * @return The next step to make to clean the house.
     */
    Step phase2();
    
};

#endif