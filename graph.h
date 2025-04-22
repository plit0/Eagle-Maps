#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <climits>
#include <queue>
using namespace::std;
using std::string;
using std::vector;

class Node
{
public:
    string node_name;
    vector <Node*> neighbors; //list of connected nodes
    vector <double> weights; //distance between nodes
    vector <double> times;
    bool visited = false;
    double distance = numeric_limits<double>::infinity();// set distance to infinity
    double time = numeric_limits<double>::infinity();
    Node* prev = nullptr; // previous pinter

    Node(string node_name) // constructor
    {
        this->node_name = node_name;
    }
    void addNeighbor(Node* neigbhor, double weight, double time)// add a connection between nodes
    {
        neighbors.push_back(neigbhor);
        weights.push_back(weight);
        times.push_back(time);
    }
};



class Graph
{
public:
    vector <Node*> nodes;
    unordered_map<string, Node*> nameToNode;
    unordered_map<string, string> aliasMap;

    void addAlias(string alias, string actualName)
    {
        aliasMap[alias] = actualName;
    }

    Node* addNode(string building_name)
    {
        Node* newBuilding = new Node(building_name);
        nameToNode[building_name] = newBuilding;
        nodes.push_back(newBuilding);
        return newBuilding;
    }
    void addEdge(Node* building1, Node* building2, double weight,double time, bool bidirectional = true)
    { //adds a bidirectional path between buildings
        building1->addNeighbor(building2, weight,time);
        if (bidirectional)
        {
            building2->addNeighbor(building1, weight,time);
        }
    }

    void removeEdge(Node* fromNode, Node* toNode)
    {
        for (size_t i = 0; i < fromNode->neighbors.size(); i++)
        {
            if (fromNode->neighbors[i] == toNode)
            {
                // Remove the neighbor and its corresponding weight
                fromNode->neighbors.erase(fromNode->neighbors.begin() + i);
                fromNode->weights.erase(fromNode->weights.begin() + i);
                fromNode->times.erase(fromNode->times.begin() + i);
                break;
            }
        }
    }

    void removeNode(string nodeName)
    {
        if (nameToNode.find(nodeName) == nameToNode.end())
        {
            cout << "Node '" << nodeName << "' does not exist in the graph.\n";
            return;
        }

        Node* nodeToRemove = nameToNode[nodeName];

        nodes.erase(remove(nodes.begin(), nodes.end(), nodeToRemove), nodes.end());

        for (Node* node : nodes)
        {
            removeEdge(node, nodeToRemove);
        }

        nameToNode.erase(nodeName);

        delete nodeToRemove;
    }


    vector <string> getPath(Node* target)
    {
        vector <string> path;
        Node* current = target;
        while (current != nullptr)
        {
            path.push_back(current->node_name);
            current = current->prev;

        }
        reverse(path.begin(), path.end());
        return path;
    }

    void dijkstra(Node* source, bool useTime = false)
    {
        for (Node* node : nodes)
        {
            node->visited = false;
            node->distance = numeric_limits<double>::infinity();
            node->time = numeric_limits<double>::infinity();
            node->prev = nullptr;
        }

        if (useTime) source->time = 0;
        else source->distance = 0;

        while (true)
        {
            Node* current = nullptr;
            double minValue = numeric_limits<double>::infinity();

            for (Node* node : nodes)
            {
                double value = useTime ? node->time : node->distance;
                if (!node->visited && value < minValue)
                {
                    minValue = value;
                    current = node;
                }
            }

            if (current == nullptr) break;
            current->visited = true;

            for (size_t i = 0; i < current->neighbors.size(); i++)
            {
                Node* neighbor = current->neighbors[i];
                double weight = current->weights[i];
                double travelTime = current->times[i];

                if (useTime)
                {
                    if (neighbor->time > current->time + travelTime)
                    {
                        neighbor->time = current->time + travelTime;
                        neighbor->prev = current;
                    }
                }
                else
                {
                    if (neighbor->distance > current->distance + weight)
                    {
                        neighbor->distance = current->distance + weight;
                        neighbor->prev = current;
                    }
                }
            }
        }
    }


    void planSchedule(vector<string> schedule, Graph& graph, bool useTime = false)
    {
        vector<Node*> schedule_nodes;

        for (string building_name : schedule)
        {
            if (aliasMap.find(building_name) != aliasMap.end())
            {
                building_name = aliasMap[building_name];
            }
            if (nameToNode.find(building_name) != nameToNode.end())
            {
                schedule_nodes.push_back(nameToNode[building_name]);
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

            dijkstra(current, useTime);
            vector<string> path = getPath(target);

            double metric = useTime ? target->time : target->distance;
            string unit = useTime ? "minutes" : "miles";

            cout << "Shortest path from " << current->node_name << " to " << target->node_name
                << " is " << metric << " " << unit << ": ";

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
            cout << node->node_name << " is connected to: ";
            for (size_t i = 0; i < node->neighbors.size(); i++)
            {
                cout << node->neighbors[i]->node_name << " (Distance: " << node->weights[i] << "), " << "(Time: " << node->times[i] << ")";
            }
            cout << endl;
        }
    }
};

