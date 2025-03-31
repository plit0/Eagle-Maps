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
public:
	string node_name;
	vector <Node*> neighbors; //list of connected nodes
	vector <double> weights; //distance between nodes
	bool visited = false;
	double distance = INT_MAX;// set distance to infinity
	Node* prev = nullptr; // previous pinter

	Node(string node_name) // constructor
	{
		this->node_name = node_name;
	}
	void addNeighbor(Node* neigbhor, double weight)// add a connection between nodes
	{
		neighbors.push_back(neigbhor);
		weights.push_back(weight);
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
	void addEdge(Node* building1, Node* building2, double weight, bool bidirectional = true)
	{ //adds a bidirectional path between buildings
		building1->addNeighbor(building2, weight);
		if (bidirectional)
		{
			building2->addNeighbor(building1, weight);
		}
	}

    // Added this missing function to remove an edge between two nodes
    void removeEdge(Node* fromNode, Node* toNode)
    {
        for (size_t i = 0; i < fromNode->neighbors.size(); i++)
        {
            if (fromNode->neighbors[i] == toNode)
            {
                // Remove the neighbor and its corresponding weight
                fromNode->neighbors.erase(fromNode->neighbors.begin() + i);
                fromNode->weights.erase(fromNode->weights.begin() + i);
                break;
            }
        }
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

    void dijkstra(Node* source)
    {
        // Reset the visited status and distances before running Dijkstra
        for (Node* node : nodes) {
            node->visited = false;
            node->distance = INT_MAX;
            node->prev = nullptr;
        }

        source->distance = 0;
        while (true)
        {
            Node* current = nullptr;
            double minimum_distance = INT_MAX;
            // Find the unvisited node with the smallest distance
            for (Node* node : nodes)
            {
                if (!node->visited && node->distance < minimum_distance)
                {
                    minimum_distance = node->distance;
                    current = node;
                }
            }
            if (current == nullptr) { break; }
            current->visited = true;

            
            for (size_t i = 0; i < current->neighbors.size(); i++)
            {
                Node* neighbor = current->neighbors[i];
                double weight = current->weights[i];

                if (neighbor->distance > current->distance + weight)
                {
                    neighbor->distance = current->distance + weight;
                    neighbor->prev = current;
                }
            }
        }
    }

    void planSchedule(vector<string> schedule, Graph& graph)
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

            dijkstra(current);
            vector<string> path = getPath(target);

            // Output the shortest path with distance
            cout << "Shortest path from " << current->node_name << " to " << target->node_name
                << " is " << target->distance << " miles: ";
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
				cout << node->neighbors[i]->node_name << " (Distance: " << node->weights[i] << "), ";
			}
			cout << endl;
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

};