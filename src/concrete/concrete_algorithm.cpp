#include "concrete_algorithm.h"

void ConcreteAlgorithm::setMaxSteps(const std::size_t maxSteps) {
    this->missionBudget = maxSteps;
}

void ConcreteAlgorithm::setBatteryMeter(const BatteryMeter &batteryMeter) {
    this->bm = &batteryMeter;
}

void ConcreteAlgorithm::setDirtSensor(const DirtSensor &dirtSensor) {
    this->ds = &dirtSensor;
}

void ConcreteAlgorithm::setWallsSensor(const WallsSensor &wallsSensor) {
    this->ws = &wallsSensor;
}

Step ConcreteAlgorithm::nextStep() {
    //TODO:
    setup();

    /* Return finish when no more dirt cleanable with remaining mission budget and robot returned to charging dock. */
    if (this->stepCount >= this->missionBudget && this->robotCoords.x == 0 && this->robotCoords.y == 0)
        return Step::Finish;

    /* At this point, a move will be made. */
    this->stepCount++;

    return Step::North;
}

void ConcreteAlgorithm::setup() {
    /* Pull data from sensors. */   
    this->batteryLeft = dynamic_cast<const ConcreteBatteryMeter *>(this->bm)->getBatteryState();
    this->dirt = dynamic_cast<const ConcreteDirtSensor *>(this->ds)->dirtLevel();
    this->wallNorth = dynamic_cast<const ConcreteWallsSensor *>(this->ws)->isWall(Direction::North);
    this->wallWest = dynamic_cast<const ConcreteWallsSensor *>(this->ws)->isWall(Direction::West);
    this->wallSouth = dynamic_cast<const ConcreteWallsSensor *>(this->ws)->isWall(Direction::South);
    this->wallEast = dynamic_cast<const ConcreteWallsSensor *>(this->ws)->isWall(Direction::East);

    /* Initialize some class attributes on first run. */
    if (this->stepCount == 0) {
        this->batteryCap = this->batteryLeft;
        this->robotCoords = Coordinate(5, 0);
        std::shared_ptr<Node> dockPtr = std::make_shared<Node>(robotCoords);

        this->houseMap.insert(std::make_pair(robotCoords, dockPtr));
    }
}

void ConcreteAlgorithm::findShortestPath(std::shared_ptr<Node> start, std::shared_ptr<Node> end) {
    /* Assumes start and end exist, and the path stack has been cleared. */

    /* Create map to store the parent of each node for the shortest path. */
    std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<Node>> parent;

    /* Create the map to store whether each node has been visited. */
    std::unordered_set<std::shared_ptr<Node>> visited;

    /* Create queue for traversal. */
    std::queue<std::shared_ptr<Node>> queue;

    queue.push(start);
    parent[start] = nullptr;

    while(!queue.empty()) {
        // Examine next node in queue.
        std::shared_ptr<Node> node = queue.front();
        queue.pop();

        // Reached target.
        if(node == end) 
            break;
        
        // If a neighbor of the node is not visited, push it into the queue. Set the neighbor's parent to the node.
        for(auto& neighbor : node->getNeighbors()) {
            if(visited.count(neighbor) == 0) {
                visited.insert(neighbor);
                queue.push(neighbor);
                parent[neighbor] = node;
            }
        }
    }

    // No possible path exists.
    if(visited.count(end) == 0)
        return;

    // Backtrack from end to start. Add each node to stack.
    std::shared_ptr<Node> pathNode = end;
    while(parent[pathNode] != nullptr) {
        this->shortestPath.push(pathNode);
        pathNode = parent[pathNode];
    }
}

Step ConcreteAlgorithm::traverseShortestPath() {
    /* Assumes that the path stack is not empty. */
    std::shared_ptr<Node> node = this->shortestPath.top();
    this->shortestPath.pop();

    Coordinate next = node->getCoords();
    if(next.y > this->robotCoords.y) 
        return Step::North;
    if(next.x < this->robotCoords.x)
        return Step::West;
    if(next.y < this->robotCoords.y)
        return Step::South;
    if(next.x > this->robotCoords.x)
        return Step::East;
}


