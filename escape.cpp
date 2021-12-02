#include <cstddef>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>
#include <iostream>
#include <functional>
#include <math.h> 
// Feel free to include additional headers here!

#include "MyGrid.hpp"

/*
This is the class Node I use this class to represent the node that we will use to search. 
The calss has 5 varialbes
    prevCost => the cost that actually happened from the inital node until we reached the current node
    estimatedCost => the heuristic function estimaition of the cost from the current node to the goal node
    totalCost => the sum of prevCost and estimatedCost this value is used to sort the nodes in the sorted queue
                  we will use later
    coordinates => the coordinates of the current node on the board. 
    prevNode => is the parent node of the current node
*/
class Node {
private:
    int prevCost = 0;
    int estimatedCost = 0;
    int totalCost = 0;

public:
    size_t coordinates[2];
    Node* prevNode;

    Node(Node* parent, size_t row, size_t col, int cost): coordinates{0, 0} {
        this->prevNode = parent;
        this->prevCost = cost; 
        coordinates[0] = row; 
        coordinates[1] = col;
    }

    Node(Node* parent, size_t row, size_t col, int prevCost, int estimatedCost, int totalCost) {
        this->prevNode = parent;
        this->prevCost = prevCost;
        this->estimatedCost = estimatedCost;
        this->totalCost = totalCost;
        coordinates[0] = row;
        coordinates[1] = col;
    }

    Node(const Node& other) {
        this->prevNode = other.prevNode;
        this->prevCost = other.prevCost;
        this->estimatedCost = other.estimatedCost;
        this->totalCost = other.totalCost;
        this->coordinates[0] = other.coordinates[0];
        this->coordinates[1] = other.coordinates[1];
    }
    
    /*Node operator&=(const Node& other) {
        this->prevCost = other.prevCost;
        this->estimatedCost = other.estimatedCost;
        this->totalCost = other.totalCost;
        this->coordinates[0] = other.coordinates[0];
        this->coordinates[1] = other.coordinates[1];
        this->prevNode = other.prevNode;
        return *this;
    }*/

    void setPrevCost(int cost) {
        this->prevCost = cost;
        this->totalCost = this->prevCost + this->estimatedCost;
    }

    void setEstimatedCost(int  cost) {
        estimatedCost = cost;
        totalCost = prevCost + cost;
    }

    int getPrevCost() {
        return prevCost;
    }

    int getEstimatedCost() {
        return estimatedCost;
    }
    
    int getTotalCost() {
        return totalCost;
    }

    friend std::ostream& operator<<(std::ostream& out, const Node& node) {
        out << "Row: " << node.coordinates[0] << ", Col: "
            << node.coordinates[1] << " ,Cost: " << node.totalCost 
            << " ,Estimated Cost: " << node.estimatedCost << " ,Previous Cost: "
            << node.prevCost;
        return out;
    }
};

/*
the heurisitc function we will use to estimate the cost for A* 
*/
int manhattanDistance( const size_t* goalIndex, const Node& node) {
    int xDiff = node.coordinates[0] - goalIndex[0];
    int yDiff = node.coordinates[1] - goalIndex[1];
    return abs(xDiff) + abs(yDiff);
}

int eculidianDistance(const size_t* goalIndex, const Node& node) {
    int xDiff = node.coordinates[0] - goalIndex[0];
    int yDiff = node.coordinates[1] - goalIndex[1];
    return sqrt(pow(xDiff, 2) + pow(yDiff, 2));
}

/*
check if the node is a goal  node by check if the node is a floor which should be on the boundaries 
           #
    #.#    .
           #
  it tries to look for patterns like this in the boundary rows and columns

*/
bool isGoal(const MyGrid& grid, size_t currentRow, size_t currentCol) {

    if (grid(currentRow, currentCol) == Floor
        && (currentRow == 0 || currentCol == 0
            || currentRow == grid.rows() - 1 || currentCol == grid.cols() - 1)) {
        return true;
    }
    return false;
}

/*
checks if the current node is a goal node or not
*/
bool goalTest(size_t* goalIndex, size_t coordinates[2]) {
    if (goalIndex[0] == coordinates[0] && goalIndex[1] == coordinates[1]) {
        return true;
    }
    return false;
}
/*
searches the whole board to find the goal node we are not trying to solve the problem here we first need 
to locate the coordinates of the goal node which we are trying to find here
*/
size_t* findGoalIndex(const MyGrid& grid) {
    size_t* goal = new size_t[2];
    for (size_t i = 0; i < grid.rows(); ++i) {
        for (size_t j = 0; j < grid.cols(); ++j) {
            if (isGoal(grid, i, j)) {
                goal[0] = i;
                goal[1] = j;
            }
        }
    }
    return goal;
}


/*
tries to calculate the possible move the agent at the current position it checks south, north, west, east to
check if we have a floor and the we are still on the boundaries of the grid. it retuns a vector of nodes that 
are possible to move to. the node carries values likes the current coordinates, cost and it works as
the parent node. 
*/
vector<Node> getPossibleNodes(MyGrid& grid, Node* node) {
    size_t* coordinates = node->coordinates;
    int currentCost = node->getPrevCost();
    vector<Node> possibleNodes;
    
    int newCost = currentCost + 1;

    size_t leftIndex = coordinates[0] - 1;
    if (leftIndex > 0 && grid(leftIndex, coordinates[1]) == Floor) {
        Node left = Node(node, leftIndex, coordinates[1], newCost);
        possibleNodes.push_back(left);
    }

    size_t rightIndex = coordinates[0] + 1;
    if (rightIndex < grid.rows() && grid(rightIndex, coordinates[1]) == Floor) {
        Node right = Node(node, rightIndex, coordinates[1], newCost);
        possibleNodes.push_back(right);
    }


    size_t topIndex = coordinates[1] - 1;
    if (topIndex > 0 && grid(coordinates[0], topIndex) == Floor) {
        Node top = Node(node, coordinates[0], topIndex, newCost);
        possibleNodes.push_back(top);
    }

    size_t downIndex = coordinates[1] + 1;
    if (downIndex < grid.cols() && grid(coordinates[0], downIndex) == Floor) {
        Node down = Node(node, coordinates[0], downIndex, newCost);
        possibleNodes.push_back(down);
    }

    return possibleNodes;
}


/*
checks if we are on square (1, 1) this is used to stop path reconstruction when we reacht he initial node
*/
bool isInitalNode(Node* node) {
    return node->coordinates[0] == 1 && node->coordinates[1] == 1;
}

/*
when we reach the goal we tries to find the path 
back by using the parent node and keep doing this until we reach the initial node
*/
stack<Node> reconstructPath(Node* node) {
    stack<Node> path;
    path.push(*node);
    Node* currentNode = node;
    while (!isInitalNode(currentNode)) {
        path.push(*currentNode);
        currentNode = currentNode->prevNode;
    }
    cout << "Hello WOrld";
    return path;
}
template<typename A> void print_queue(A& pq)
{
    while (!pq.empty())
    {
        cout << pq.top() << endl;
        pq.pop();
    }
}

// Feel free to define auxiliary functions here!
MyGrid escape(MyGrid grid) {
    assert(grid(1,1) == Floor); // Check that the initial tile is valid.

    // TODO implement some path finding algorithm find a correct path to an
    // exit tile, and then write that path to the grid.
    size_t* goalIndex = findGoalIndex(grid);

    Node initialNode = Node(nullptr, 1, 1, 0);
    initialNode.setEstimatedCost(manhattanDistance(goalIndex, initialNode));

    auto cmp = [](Node left, Node right) { 
        return (left.getTotalCost() ^ 1) > (right.getTotalCost() ^ 1); 
    };
    priority_queue<Node, std::vector<Node>, decltype(cmp)> fringe(cmp);

    fringe.push(initialNode);

    while (fringe.size() > 0) {
        Node* currentNode = new Node(fringe.top());
        fringe.pop();
        if (goalTest(goalIndex, currentNode->coordinates)) {
            stack<Node>  path = reconstructPath(currentNode);
            while (!path.empty()) {
                Node node = path.top();
                grid(node.coordinates[0], node.coordinates[1]) = Path;
                path.pop();
            }
            grid(1, 1) = Path;
            cout << grid << endl;
            break;
        }
        vector<Node> possibleNodes = getPossibleNodes(grid, currentNode);
        for (size_t i = 0; i != possibleNodes.size(); ++i) {
            possibleNodes[i].setEstimatedCost(manhattanDistance(goalIndex, possibleNodes[i]));
            fringe.push(possibleNodes[i]);
        }
        possibleNodes.clear();
    }

    return grid;
}


