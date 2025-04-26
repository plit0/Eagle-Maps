#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <climits>
#include <queue>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace::std;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::endl;
using std::getline;
using std::stod;
using std::cout;
using std::cin;

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

    // Save the graph to a file
    void saveToFile(const string& filename)
    {
        ofstream file(filename);
        if (!file.is_open())
        {
            cout << "Error opening file for writing.\n";
            return;
        }

        file << "# Nodes\n";
        for (Node* node : nodes)
        {
            file << node->node_name << endl;
        }

        file << "\n# Edges\n";
        for (Node* node : nodes)
        {
            for (size_t i = 0; i < node->neighbors.size(); ++i)
            {
                file << node->node_name << ", " 
                     << node->neighbors[i]->node_name << ", " 
                     << node->weights[i] << ", "
                     << node->times[i] << endl; // Added time information
            }
        }

        file << "\n# Aliases\n";
        for (const auto& pair : aliasMap)
        {
            file << pair.first << ", " << pair.second << endl;
        }

        file.close();
        cout << "Graph saved successfully to " << filename << endl;
    }

    // Load the graph from a file
    void loadFromFile(const string& filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Error opening file for reading.\n";
            return;
        }

        // Clear existing graph data
        for (Node* node : nodes)
        {
            delete node;
        }
        nodes.clear();
        nameToNode.clear();
        aliasMap.clear();

        string line;
        enum Section { NONE, NODES, EDGES, ALIASES };
        Section section = NONE;

        while (getline(file, line))
        {
            if (line == "# Nodes")
            {
                section = NODES;
                continue;
            }
            else if (line == "# Edges")
            {
                section = EDGES;
                continue;
            }
            else if (line == "# Aliases")
            {
                section = ALIASES;
                continue;
            }
            else if (line.empty())
            {
                continue;
            }

            switch (section)
            {
                case NODES:
                {
                    addNode(line);
                    break;
                }
                case EDGES:
                {
                    stringstream ss(line);
                    string building1, building2, weightStr, timeStr;
                    getline(ss, building1, ',');
                    getline(ss, building2, ',');
                    getline(ss, weightStr, ',');
                    getline(ss, timeStr);

                    // Trim spaces
                    building1.erase(0, building1.find_first_not_of(" "));
                    building1.erase(building1.find_last_not_of(" ") + 1);
                    building2.erase(0, building2.find_first_not_of(" "));
                    building2.erase(building2.find_last_not_of(" ") + 1);
                    weightStr.erase(0, weightStr.find_first_not_of(" "));
                    weightStr.erase(weightStr.find_last_not_of(" ") + 1);
                    timeStr.erase(0, timeStr.find_first_not_of(" "));
                    timeStr.erase(timeStr.find_last_not_of(" ") + 1);

                    double weight = stod(weightStr);
                    double time = stod(timeStr);

                    if (nameToNode.find(building1) != nameToNode.end() &&
                        nameToNode.find(building2) != nameToNode.end())
                    {
                        addEdge(nameToNode[building1], nameToNode[building2], weight, time);
                    }
                    break;
                }
                case ALIASES:
                {
                    stringstream ss(line);
                    string alias, actualName;
                    getline(ss, alias, ',');
                    getline(ss, actualName);

                    alias.erase(0, alias.find_first_not_of(" "));
                    alias.erase(alias.find_last_not_of(" ") + 1);
                    actualName.erase(0, actualName.find_first_not_of(" "));
                    actualName.erase(actualName.find_last_not_of(" ") + 1);

                    addAlias(alias, actualName);
                    break;
                }
                default:
                    break;
            }
        }

        file.close();
        cout << "Graph loaded successfully from " << filename << endl;
    }

    // Print all aliases for a building
    void printAliasesForBuilding(const string& buildingName)
    {
        cout << "Aliases for " << buildingName << ":\n";
        for (const auto& pair : aliasMap)
        {
            if (pair.second == buildingName)
            {
                cout << " - " << pair.first << '\n';
            }
        }
    }

    // Add aliases to a building
    void addAliasesToBuilding(const string& buildingName)
    {
        // Check if building exists
        if (nameToNode.find(buildingName) == nameToNode.end())
        {
            cout << "Building '" << buildingName << "' does not exist.\n";
            return;
        }

        cout << "Add aliases for " << buildingName << ". Enter one at a time. Type 'done' when finished:\n";

        string alias;
        cin.ignore();
        while (true)
        {
            cout << "Alias: ";
            getline(cin, alias);

            // Trim whitespace
            alias.erase(0, alias.find_first_not_of(" \t\n\r"));
            alias.erase(alias.find_last_not_of(" \t\n\r") + 1);

            if (alias == "done") break;
            if (!alias.empty())
            {
                addAlias(alias, buildingName);
                cout << "Added alias: " << alias << "\n";
            }
        }
    }
};
