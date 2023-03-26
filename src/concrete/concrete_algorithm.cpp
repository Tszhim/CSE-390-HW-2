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

bool ConcreteAlgorithm::onChargingDock() {
    return this->robotCoords.x == 0 && this->robotCoords.y == 0;
}

Step ConcreteAlgorithm::nextStep() {
    /* Perform necessary setup before computing next step. */
    setup();

    /* Get current node and dock node. */
    std::shared_ptr<Node> dock = this->houseMap[Coordinate(0, 0)];
    std::shared_ptr<Node> curr = this->houseMap[this->robotCoords];
    
    /* EXIT CONDITIONS */

    /* Return finish when no more dirt is cleanable OR the mission budget has been exhausted and robot returned to charging dock. */
    if((this->unvisitedNodes.size() == 0 || this->missionBudget - 1 == this->stepCount) && onChargingDock()) {
        return Step::Finish;
    }

    /* After this point, a move will be made. */
    this->stepCount++;


    /* TRAVERSAL TO A PARTICULAR NODE */

    /* If the path to return to dock is ever non-empty, the robot must follow the path. */
    if(!this->pathToDock.empty()) {
        return returnToDock();
    }

    /* There are no nodes left to explore, return to dock. */
    if(this->unvisitedNodes.size() == 0) {
        this->pathToDock = this->findShortestPath(curr, dock);
        return returnToDock();
    }

    /* Estimation indicates that that the robot may have just enough mission budget to return (upper-bounded). */
    if(!onChargingDock() && this->missionBudget <= this->stepCount + this->distFromDock + 1) {
        auto path = findShortestPath(curr, dock);
        
        /* If actual distance confirms the estimate, return to dock. */
        if(this->missionBudget <= this->stepCount + path.size() + 1) {
            this->pathToDock = path;
            return returnToDock();
        }
        /* If the actual distance does not confirm the estimate, continue exploration. */
        else 
            this->distFromDock = path.size();
    }

    /* Estimation indicates that the robot may have just enough battery to return (upper-bounded). */
    if(!onChargingDock() && this->batteryLeft <= this->distFromDock + 1) {
        auto path = findShortestPath(curr, dock);
        
        /* If actual distance aligns with estimate, return to dock, otherwise continue. */
        if(this->batteryLeft <= path.size() + 1){ 
            this->pathToDock = path;
            return returnToDock();
        }
        else 
            this->distFromDock = path.size();
    }   

    /* CHARGING AND CLEANING */

    /* If on charging dock, always fully charge. */
    if(onChargingDock() && this->batteryLeft != this->batteryCap) {
        this->distFromDock = 0;
        this->pathToNode = std::stack<std::shared_ptr<Node>>();
        return Step::Stay;
    }

    /* If on a space with no dirt, immediately remove it from the list of nodes to visit. */
    if(curr->getDirtLevel() == 0 && this->unvisitedNodes.count(curr) == 1) 
        this->unvisitedNodes.erase(curr);

    /* If on a space with dirt, always clean. */
    if(curr->getDirtLevel() != 0) {
        /* Immediately remove space from list of nodes to visit once dirt level is 0. */
        if(curr->getDirtLevel() == 1) 
            this->unvisitedNodes.erase(curr);

        curr->decrementDirtLevel();
        return Step::Stay;
    }


    /* EXPLORATION */

    /* If a node to get to is already determined, continue following it. */
    if(!this->pathToNode.empty()) {
        return moveToNode();
    }

    /* Traverse the closest adjacent node, if it exists. */
    std::shared_ptr<Node> neighbor = getClosestAdjacentNode();
    if(neighbor)
        return getDirectionToNode(neighbor);

    /* Traverse the closest non-adjacent node. */
    setClosestNonAdjacentNodePath();
    return moveToNode();
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
    if(this->stepCount == 0) {
        this->batteryCap = this->batteryLeft;

        std::shared_ptr<Node> dockPtr = std::make_shared<Node>(robotCoords);
        this->houseMap.insert(std::make_pair(robotCoords, dockPtr));
    }

    /* Set current node to visited. */
    std::shared_ptr<Node> curr = this->houseMap[this->robotCoords];
    curr->setVisited();
    curr->setDirtLevel(this->dirt);

    /* Map neighbors of the current node. */
    markSurroundings();
}

void ConcreteAlgorithm::markSurroundings() {
    /* If north/south/east/west neighbor not mapped, add it to house map. */
    if(!this->wallNorth)
        mapNeighbor(Coordinate(this->robotCoords.x, this->robotCoords.y + 1));
    if(!this->wallWest)
        mapNeighbor(Coordinate(this->robotCoords.x - 1, this->robotCoords.y));
    if(!this->wallSouth)
        mapNeighbor(Coordinate(this->robotCoords.x, this->robotCoords.y - 1));
    if(!this->wallEast)
        mapNeighbor(Coordinate(this->robotCoords.x + 1, this->robotCoords.y));
}

void ConcreteAlgorithm::mapNeighbor(Coordinate coords) {
    /* Add neighbor to house map, if not already mapped. */
    if(this->houseMap.count(coords) == 0) {
        std::shared_ptr<Node> neighbor = std::make_shared<Node>(coords);
        this->houseMap.insert(std::make_pair(coords, neighbor));
    }

    /* Add neighbor to current node's list of neighbors, if not already present. */
    std::shared_ptr<Node> curr = this->houseMap[this->robotCoords];
    std::shared_ptr<Node> neighbor = this->houseMap[coords];
    auto neighbors = curr->getNeighbors();

    bool found = false;
    for(int i = 0; i < neighbors.size(); i++) {
        if(neighbors[i]->getCoords() == coords) {
            found = true;
            break;
        }    
    }
    if(!found)
        curr->addNeighbor(neighbor);
    
    /* Add neighbor to list of nodes to clean. */
    if(this->unvisitedNodes.count(neighbor) == 0 && !neighbor->isVisited()) 
        this->unvisitedNodes.insert(neighbor);
}

std::shared_ptr<Node> ConcreteAlgorithm::getClosestAdjacentNode() {
    std::shared_ptr<Node> currNode = this->houseMap[this->robotCoords];
    std::vector<std::shared_ptr<Node>> neighbors = currNode->getNeighbors();

    int shortestDistance = std::numeric_limits<int>::max();
    std::shared_ptr<Node> closestNode = nullptr;

    /* Find the neighbor of the current node with the lowest Euclidean distance to dock. */
    for(int i = 0; i < neighbors.size(); i++) {
        std::shared_ptr<Node> adjacentNode = neighbors[i];
        
        /* Skip visited nodes. */
        if(adjacentNode->isVisited())
            continue;
        
        if(adjacentNode->getEuclidianDist() < shortestDistance) {
            shortestDistance = adjacentNode->getEuclidianDist();
            closestNode = adjacentNode;
        }
    }
    return closestNode;
}

void ConcreteAlgorithm::setClosestNonAdjacentNodePath() {
    std::shared_ptr<Node> curr = this->houseMap[this->robotCoords];

    /* Maintain the shortest path to an unvisited node. */
    std::stack<std::shared_ptr<Node>> nextPath;
    int pathSize = std::numeric_limits<int>::max();

    /* Determine the unvisited node to traverse based on proximity to current node. */
    for(auto& unvisitedNode : this->unvisitedNodes) {
        auto path = findShortestPath(curr, unvisitedNode);

        if(path.size() < pathSize) {
            nextPath = path;
            pathSize = path.size();
        }
    }
    this->pathToNode = nextPath;
}

Step ConcreteAlgorithm::getDirectionToNode(std::shared_ptr<Node> node) {
    /* Given a node that is directly adjacent to the robot, get the direction to that node. */
    Coordinate goToCoords = node->getCoords();

    Step s = Step::Stay;

    if(this->robotCoords.y + 1 == goToCoords.y)
        s = Step::North;
    if(this->robotCoords.x - 1 == goToCoords.x) 
        s = Step::West;
    if(this->robotCoords.y - 1 == goToCoords.y)
        s = Step::South;
    if(this->robotCoords.x + 1 == goToCoords.x) 
        s = Step::East;

    /* Update robot's location after movement. */
    this->robotCoords = goToCoords;

    /* Assume always moving away from dock in estimation. */
    this->distFromDock++;

    return s;
}

std::stack<std::shared_ptr<Node>> ConcreteAlgorithm::findShortestPath(std::shared_ptr<Node> start, std::shared_ptr<Node> end) {
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
        visited.insert(node);

        // Reached target.
        if(node == end) 
            break;
        
        // If a neighbor of the node is not visited, push it into the queue. Set the neighbor's parent to the node.
        for(auto& neighbor : node->getNeighbors()) {
            if(visited.count(neighbor) == 0) {
                queue.push(neighbor);
                parent[neighbor] = node;
            }
        }
    }

    std::stack<std::shared_ptr<Node>> path;

    // No possible path exists.
    if(visited.count(end) == 0)
        return path;

    // Backtrack from end to start. Add each node to stack.
    std::shared_ptr<Node> pathNode = end;
    while(parent[pathNode] != nullptr) {
        path.push(pathNode);
        pathNode = parent[pathNode];
    }

    return path;
}

Step ConcreteAlgorithm::returnToDock() {
    /* Sanity check. */
    if(this->pathToDock.empty()) 
        return Step::Stay;

    auto node = this->pathToDock.top();
    this->pathToDock.pop();
    return getDirectionToNode(node);
}

Step ConcreteAlgorithm::moveToNode() {
    /* Sanity check. */
    if(this->pathToNode.empty()) 
        return Step::Stay;

    auto node = this->pathToNode.top();
    this->pathToNode.pop();
    return getDirectionToNode(node);
}


