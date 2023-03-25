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
    /* Perform necessary setup before computing next step. */
    setup();

    /* Get current node and dock node. */
    std::shared_ptr<Node> dock = this->houseMap[Coordinate(0, 0)];
    std::shared_ptr<Node> curr = this->houseMap[this->robotCoords];
    
    /* EXIT CONDITIONS */

    /* Return finish when no more dirt is cleanable and robot returned to charging dock. */
    if(this->uncleanedNodes.size() == 0 && this->robotCoords.x == 0 && this->robotCoords.y == 0)
        return Step::Finish;

    /* Return finish if the mission budget has been exhausted and robot returned to charging dock. */
    if((this->missionBudget - 1 == this->stepCount) && this->robotCoords.x == 0 && this->robotCoords.y == 0)
        return Step::Finish;
    
    /* After this point, a move will be made. */
    this->stepCount++;


    /* TRAVERSAL TO A PARTICULAR NODE */

    /* If the path stack is ever non-empty, traverse it. */
    if(!this->shortestPath.empty()) {
        return traverseShortestPath();
    }

    /* There are no nodes left to explore, return to dock. */
    if(this->uncleanedNodes.size() == 0) {
        findShortestPath(curr, dock);
        return traverseShortestPath();
    }

    /* Estimation indicates that that the robot may have just enough mission budget to return (upper-bounded). */
    if(dock != curr && this->missionBudget <= this->stepCount + this->distFromDock + 1) {
        findShortestPath(curr, dock);
        
        /* If actual distance aligns with estimate, return to dock, otherwise continue. */
        if(this->missionBudget <= this->stepCount + this->shortestPath.size() + 1) 
            return traverseShortestPath();
        else
            this->shortestPath = std::stack<std::shared_ptr<Node>>();
    }

    /* Estimation indicates that the robot may have just enough battery to return (upper-bounded). */
    if(dock != curr && this->batteryLeft <= this->distFromDock + 1) {
        findShortestPath(curr, dock);
        
        /* If actual distance aligns with estimate, return to dock, otherwise continue. */
        if(this->batteryLeft <= this->shortestPath.size() + 1)
            return traverseShortestPath();
        else {
            this->distFromDock = this->shortestPath.size();
            this->shortestPath = std::stack<std::shared_ptr<Node>>();
        }
    }   


    /* CHARGING AND CLEANING */

    /* If on charging dock, always fully charge. */
    if(dock == curr && this->batteryLeft != this->batteryCap) {
        this->distFromDock = 0;
        return Step::Stay;
    }

    /* If on a space with dirt, always clean.  */
    if(curr->getDirtLevel() != 0) {
        /* Delete node from uncleanedNodes set once finished cleaning. */
        if(curr->getDirtLevel() == 1) 
            this->uncleanedNodes.erase(curr);

        curr->decrementDirtLevel();
        return Step::Stay;
    }


    /* EXPLORATION */

    /* Traverse the closest neighbor to the current node, if it exists. */
    std::shared_ptr<Node> neighbor = getClosestAdjacentNode();
    if(neighbor)
        return getDirectionToNode(neighbor);

    /* Traverse the closest non-adjacent node. */
    setClosestNonAdjacentNodePath();
    return traverseShortestPath();
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
    if(this->houseMap.count(coords) == 0) {
        /* Add neighbor to house map.*/
        std::shared_ptr<Node> neighbor = std::make_shared<Node>(coords);
        this->houseMap.insert(std::make_pair(coords, neighbor));
    }

    /* Add neighbor to current node's list of neighbors, if not already present. */
    std::shared_ptr<Node> curr = this->houseMap[this->robotCoords];
    std::vector<std::shared_ptr<Node>> neighbors = curr->getNeighbors();
    std::shared_ptr<Node> neighbor = this->houseMap[coords];

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
    if(this->uncleanedNodes.count(neighbor) == 0) 
        this->uncleanedNodes.insert(neighbor);
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
    for(auto& uncleanedNode : this->uncleanedNodes) {
        findShortestPath(curr, uncleanedNode);

        if(this->shortestPath.size() < pathSize) {
            nextPath = this->shortestPath;
            pathSize = this->shortestPath.size();
        }
    }
    this->shortestPath = nextPath;
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
    if(this->shortestPath.empty()) {
        return Step::Stay;
    }
    
    std::shared_ptr<Node> node = this->shortestPath.top();
    this->shortestPath.pop();

    return getDirectionToNode(node);
}


