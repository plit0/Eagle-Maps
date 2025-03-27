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
	bool visited = false;
	double distance = INT_MAX;// set distance to infinity

	Node(string node_name)
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

	Node* addNode(string building_name)
	{
		Node* newBuilding = new Node(building_name);
		nodes.push_back(newBuilding);
		return newBuilding;
	}
	void addEdge(Node* building1, Node* building2, double weight, bool bidirectional = true)
	{
		building1->addNeighbor(building2, weight);
		if (bidirectional)
		{
			building2->addNeighbor(building1, weight);
		}
	}
	void dijkstra(Node* source)
	{
		source->distance = 0;
		while (true)
		{
			Node* current = nullptr;
			double minimum_distance = INT_MAX;
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
				}

			}
		}
	}
	void displayGraph()
	{
		for (Node* node : nodes)
		{
			cout << node->node_name << "-> Distance to " << node->node_name << ": ";
			if (node->distance != INT_MAX)
			{
				cout << node->distance << endl;
			}
		}
	}
};
