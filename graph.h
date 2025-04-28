#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <climits>
#include <queue>
#include <algorithm>
using namespace::std;
using std::string;
using std::vector;

class Node
{
private:
    string node_name;
    vector<Node*> neighbors; // list of connected nodes
    vector<double> weights; // distance between nodes
    bool visited;
    double distance;
    Node* prev;

public:
    Node(string node_name) // constructor
    {
        this->node_name = node_name;
        this->visited = false;
        this->distance = INT_MAX; // set distance to infinity
        this->prev = nullptr; // previous pointer
    }

    // Accessor methods
    string getName() const { return node_name; }
    bool isVisited() const { return visited; }
    double getDistance() const { return distance; }
    Node* getPrev() const { return prev; }
    const vector<Node*>& getNeighbors() const { return neighbors; }
    const vector<double>& getWeights() const { return weights; }

    // Mutator methods
    void setVisited(bool value) { visited = value; }
    void setDistance(double value) { distance = value; }
    void setPrev(Node* node) { prev = node; }

    void addNeighbor(Node* neighbor, double weight) // add a connection between nodes
    {
        neighbors.push_back(neighbor);
        weights.push_back(weight);
    }

    void removeNeighbor(Node* neighbor)
    {
        for (size_t i = 0; i < neighbors.size(); i++)
        {
            if (neighbors[i] == neighbor)
            {
                neighbors.erase(neighbors.begin() + i);
                weights.erase(weights.begin() + i);
                break;
            }
        }
    }

    // For accessing a neighbor and its weight at a specific index
    Node* getNeighborAt(size_t index) const
    {
        if (index < neighbors.size())
            return neighbors[index];
        return nullptr;
    }

    double getWeightAt(size_t index) const
    {
        if (index < weights.size())
            return weights[index];
        return 0.0;
    }

    size_t getNeighborCount() const
    {
        return neighbors.size();
    }
};

class Graph
{
private:
    vector<Node*> nodes;
    unordered_map<string, Node*> nameToNode;
    unordered_map<string, string> aliasMap;

public:
    ~Graph() {
        // Destructor to free memory
        for (Node* node : nodes) {
            delete node;
        }
    }

    void addAlias(string alias, string actualName)
    {
        aliasMap[alias] = actualName;
    }

    string resolveAlias(const string& name) const
    {
        auto it = aliasMap.find(name);
        if (it != aliasMap.end())
            return it->second;
        return name;
    }

    Node* addNode(string building_name)
    {
        Node* newBuilding = new Node(building_name);
        nameToNode[building_name] = newBuilding;
        nodes.push_back(newBuilding);
        return newBuilding;
    }

    Node* getNode(const string& name) const
    {
        string resolvedName = resolveAlias(name);
        auto it = nameToNode.find(resolvedName);
        if (it != nameToNode.end())
            return it->second;
        return nullptr;
    }

    const vector<Node*>& getNodes() const
    {
        return nodes;
    }

    void addEdge(Node* building1, Node* building2, double weight, bool bidirectional = true)
    {
        building1->addNeighbor(building2, weight);
        if (bidirectional)
        {
            building2->addNeighbor(building1, weight);
        }
    }

    void removeEdge(Node* fromNode, Node* toNode)
    {
        fromNode->removeNeighbor(toNode);
    }

    vector<string> getPath(Node* target)
    {
        vector<string> path;
        Node* current = target;
        while (current != nullptr)
        {
            path.push_back(current->getName());
            current = current->getPrev();
        }
        reverse(path.begin(), path.end());
        return path;
    }

    void dijkstra(Node* source)
    {
        // Reset the visited status and distances before running Dijkstra
        for (Node* node : nodes) {
            node->setVisited(false);
            node->setDistance(INT_MAX);
            node->setPrev(nullptr);
        }

        source->setDistance(0);
        while (true)
        {
            Node* current = nullptr;
            double minimum_distance = INT_MAX;
            // Find the unvisited node with the smallest distance
            for (Node* node : nodes)
            {
                if (!node->isVisited() && node->getDistance() < minimum_distance)
                {
                    minimum_distance = node->getDistance();
                    current = node;
                }
            }
            if (current == nullptr) { break; }
            current->setVisited(true);

            for (size_t i = 0; i < current->getNeighborCount(); i++)
            {
                Node* neighbor = current->getNeighborAt(i);
                double weight = current->getWeightAt(i);

                if (neighbor->getDistance() > current->getDistance() + weight)
                {
                    neighbor->setDistance(current->getDistance() + weight);
                    neighbor->setPrev(current);
                }
            }
        }
    }

    void planSchedule(vector<string> schedule, Graph& graph)
    {
        vector<Node*> schedule_nodes;

        for (string building_name : schedule)
        {
            building_name = resolveAlias(building_name);
            Node* node = getNode(building_name);
            
            if (node != nullptr)
            {
                schedule_nodes.push_back(node);
            }
            else
            {
                cout << "Error: Building '" << building_name << "' not found on campus map.\n";
                return;
            }
        }

        for (size_t i = 0; i < schedule_nodes.size() - 1; i++)
        {
            Node* current = schedule_nodes[i];
            Node* target = schedule_nodes[i + 1];

            dijkstra(current);
            vector<string> path = getPath(target);

            // Output the shortest path with distance
            cout << "Shortest path from " << current->getName() << " to " << target->getName()
                << " is " << target->getDistance() << " miles: ";
            for (const string& building_name : path)
            {
                cout << building_name << " ";
            }
            cout << endl;
        }
    }

    void displayGraph()
    {
        for (Node* node : nodes)
        {
            cout << node->getName() << " is connected to: ";
            for (size_t i = 0; i < node->getNeighborCount(); i++)
            {
                cout << node->getNeighborAt(i)->getName() << " (Distance: " 
                     << node->getWeightAt(i) << "), ";
            }
            cout << endl;
        }
    }

    void removeNode(string nodeName)
    {
        string resolvedName = resolveAlias(nodeName);
        auto it = nameToNode.find(resolvedName);
        if (it == nameToNode.end())
        {
            cout << "Node '" << nodeName << "' does not exist in the graph.\n";
            return;
        }

        Node* nodeToRemove = it->second;

        // Remove this node from all other nodes' neighbor lists
        for (Node* node : nodes)
        {
            node->removeNeighbor(nodeToRemove);
        }

        // Remove from nodes vector
        nodes.erase(remove(nodes.begin(), nodes.end(), nodeToRemove), nodes.end());

        // Remove from nameToNode map
        nameToNode.erase(resolvedName);

        // Delete the node
        delete nodeToRemove;
    }
};