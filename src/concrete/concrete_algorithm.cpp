#include "concrete_algorithm.h"

void ConcreteAlgorithm::setMaxSteps(const std::size_t maxSteps) {
    this->missionBudget = maxSteps;
    this->stepCount = 0;
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
    setup();

    /* Return finish when no more dirt cleanable with remaining mission budget and robot returned to charging dock. */
    if (this->stepCount >= this->missionBudget && this->space.x == 0 && this->space.y == 0)
        return Step::Finish;

    /* At this point, a move will be made. */
    this->stepCount++;

    /* If on charging dock, charge until full capacity. */
    if(this->space.x == 0 && this->space.y == 0 && this->batteryLeft < this->batteryCap) 
        return Step::Stay;

    /* Robot cannot continue traversal with remaining mission budget. */
    if (this->missionBudget <= this->stepCount + this->pathStack.size() + 1)
        return backtrack();

    /* Robot cannot continue traversal with remaining battery, must return to dock. */
    if (this->batteryLeft <= this->pathStack.size() + 1)
        return backtrack();

    /* If phase #1 is finished, return to dock, then continue with phase #2. */
    if (this->pathStack.empty() && (this->branchQueue.empty() || this->branchLevel > this->batteryCap / 2))
        this->mapFinished = true;

    return !this->mapFinished ? phase1() : phase2();
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
    if (stepCount == 0) {
        this->batteryCap = this->batteryLeft;
        this->branchQueue.push(std::queue<Coordinate>({Coordinate()}));
    }
}

Step ConcreteAlgorithm::makeStepToNext(Coordinate next) {
    if(next.y > this->space.y) {
        this->space.y++;
        return Step::North;
    }
    if(next.x < this->space.x) {
        this->space.x--;
        return Step::West;
    }
    if(next.y < this->space.y) {
        this->space.y--;
        return Step::South;
    }
    if(next.x > this->space.x) {
        this->space.x++;
        return Step::East;
    }

    /* Unexpected. */
    return Step::Stay;
}

Step ConcreteAlgorithm::backtrack() {
    /* Unexpected error. */
    if (this->pathStack.empty())
        return Step::Stay;
    
    Coordinate prev = this->pathStack.back();
    this->pathStack.pop_back();

    return makeStepToNext(prev);
}

Step ConcreteAlgorithm::moveTo() {
    /* Get back to the cluster. */
    if(auto it = paths.find(this->nextSpaces.front()); it != paths.end()) {
        auto path = it->second;
        while(path.front() != space)
            path.pop_front();
        
        if(!path.empty()) {
            Coordinate front = path.front();
            path.pop_front();
            return makeStepToNext(front);
        }
    }

    /* Backtrack one step after each visit to prepare for next move in cluster. */
    if(this->nextSpaceBacktrack) {
        this->nextSpaceBacktrack = false;
        return backtrack();
    }

    /* Move to next node in cluster. */
    Coordinate nextSpace = this->nextSpaces.front();
    this->nextSpaces.pop();

    Step s = makeStepToNext(nextSpace);
    this->nextSpaceBacktrack = true;

    /* Delete next space in cluster. */
    if(this->nextSpaces.empty()) {
        this->nextSpacesSet = false;
    }
    return s;
}

Step ConcreteAlgorithm::phase1() {
    /* Return back to dock once a cluster has been visited. */
    if(!this->nextSpacesSet && (this->space.x != 0 || this->space.y != 0))
        return backtrack();

    /* Housekeeping. */
    this->pathStack.push_back(this->space);
    this->nodes.insert({this->space, this->dirt}); 
    this->visitedNodes.insert(this->space);

    /* Pop out next cluster of nodes to traverse to if not determined yet.  */
    if(!this->nextSpacesSet) {
        this->nextSpaces = this->branchQueue.front();
        this->branchQueue.pop();
        this->nextSpacesSet = true;
    }

    /* Push available nodes in the order NWSE. */
    Coordinate north = Coordinate(this->space.x, this->space.y + 1);
    Coordinate west = Coordinate(this->space.x - 1, this->space.y);
    Coordinate south = Coordinate(this->space.x, this->space.y - 1);
    Coordinate east = Coordinate(this->space.x + 1, this->space.y);
    std::queue<Coordinate> nextBranch;

    if(this->wallNorth && !this->visitedNodes.count(north)) {
        nextBranch.push(north);
        this->paths.insert({north, pathStack});
    }
    if(this->wallWest && !this->visitedNodes.count(west)) {
        nextBranch.push(west);
        this->paths.insert({west, pathStack});
    }
    if(this->wallSouth && !this->visitedNodes.count(south)) {
        nextBranch.push(south);
        this->paths.insert({south, pathStack});
    }
    if(this->wallEast && !this->visitedNodes.count(east)) {
        nextBranch.push(east);
        this->paths.insert({east, pathStack});
    }
    if(!this->visitedNodes.count(this->space) && !nextBranch.empty())
        this->nextQueue.push(nextBranch);
        
    /* Reached next BFS depth level. */
    if(this->branchQueue.empty()) {
        this->branchQueue = this->nextQueue;
        this->nextQueue = std::queue<std::queue<Coordinate>>();
        this->branchLevel++;
    }

    /* Move to next node. */    
    return moveTo();
}

Step ConcreteAlgorithm::phase2() {
    return Step::Finish;
}